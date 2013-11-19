//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#include "audio_pch.h"
#include "snd_mp3_source.h"
#include "utlsymbol.h"
#include "checksum_crc.h"
#include "../../host.h"
#include "xwvfile.h"
#include "filesystem/IQueuedLoader.h"
#include "tier1/lzmaDecoder.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// This determines how much data to pre-cache (will invalidate per-map caches if changed).
#define SND_ASYNC_LOOKAHEAD_SECONDS		( 0.125f )
#define SOUND_DIRECTORY_LENGTH			6		// i.e., Q_strlen( "sound/" )
#define	OTHER_REBUILD_CACHE_NAME		"_other_rebuild"
// If a sound is in 50% of the maps, put in precacheshared
#define SOUND_PRECACHESHARED_THRESHOLD	( 0.5f )

extern ConVar snd_async_spew_blocking;
extern float realtime;
ConVar snd_async_minsize("snd_async_minsize", "262144");

// #define DEBUG_CHUNKS

//-----------------------------------------------------------------------------
// Purpose: Report chunk error
// Input  : id - chunk FOURCC
//-----------------------------------------------------------------------------
void ChunkError( unsigned int id )
{
#if defined( DEBUG_CHUNKS ) && defined( _DEBUG )
	if ( id == WAVE_LIST || id == WAVE_FACT )
	{
		// unused chunks, not an error
		return;
	}

	char tmp[256];
	char idname[5];
	idname[4] = 0;
	memcpy( idname, &id, 4 );

	Q_snprintf( tmp, sizeof( tmp ), "Unhandled chunk %s\n", idname );
	Plat_DebugString( tmp );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Determine a true sample count for an ADPCM blob
//-----------------------------------------------------------------------------
int ADPCMSampleCount( ADPCMWAVEFORMAT *pFormat, int length )
{
	// determine a true sample count
	int nChannels = LittleWord( pFormat->wfx.nChannels );
	int wSamplesPerBlock = LittleWord( pFormat->wSamplesPerBlock );

	int blockSize = (( wSamplesPerBlock - 2) * nChannels ) / 2;
	blockSize += 7 * nChannels;

	int blockCount = length / blockSize;
	int blockRem = length % blockSize;
	
	// total samples in complete blocks
	int sampleCount = blockCount * wSamplesPerBlock;

	// add remaining in a short block
	if ( blockRem )
	{
		sampleCount += wSamplesPerBlock - (((blockSize - blockRem) * 2) / nChannels );
	}

	return sampleCount;
}

//-----------------------------------------------------------------------------
// Purpose: Init to empty wave
//-----------------------------------------------------------------------------
CAudioSourceWave::CAudioSourceWave( CSfxTable *pSfx )
{
	m_format = 0;
	m_pHeader = NULL;
	m_nHeaderSize = 0;

	// no looping
	m_loopStart = -1;

	m_sampleSize = 1;
	m_sampleCount = 0;
	m_bits = 0;
	m_channels = 0;
	m_dataStart = 0;
	m_dataSize = 0;
	m_rate = 0;

	m_refCount = 0;

	m_pSfx = pSfx;
#ifdef _DEBUG
	if ( m_pSfx )
		m_pDebugName = strdup( m_pSfx->getname() );
#endif

	m_bNoSentence = false;
	m_pTempSentence = NULL;
	m_nCachedDataSize = 0;
	m_bIsPlayOnce = false;
	m_bIsSentenceWord = false;

	m_numDecodedSamples = 0;
}

CAudioSourceWave::CAudioSourceWave( CSfxTable *pSfx, CAudioSourceCachedInfo *info )
{
	m_pSfx = pSfx;
#ifdef _DEBUG
	if ( m_pSfx )
		m_pDebugName = strdup( m_pSfx->getname() );
#endif

	m_refCount = 0;

	m_pHeader = NULL;
	m_nHeaderSize = 0;

	if ( info->HeaderData() )
	{
		m_pHeader = new char[ info->HeaderSize() ];
		Assert( m_pHeader );
		Q_memcpy( m_pHeader, info->HeaderData(), info->HeaderSize() );
		m_nHeaderSize = info->HeaderSize();
	}
	
	m_bits = info->Bits();
	m_channels = info->Channels();
	m_sampleSize = info->SampleSize();
	m_format = info->Format();
	m_dataStart = info->DataStart();
	m_dataSize = info->DataSize();
	m_rate = info->SampleRate();
	m_loopStart = info->LoopStart();
	m_sampleCount = info->SampleCount();
	m_numDecodedSamples = m_sampleCount;

	if ( m_format == WAVE_FORMAT_ADPCM && m_pHeader )
	{
		m_numDecodedSamples = ADPCMSampleCount( (ADPCMWAVEFORMAT *)m_pHeader, m_sampleCount );
	}

	m_bNoSentence = false;
	m_pTempSentence = NULL;
	m_nCachedDataSize = 0;
	m_bIsPlayOnce = false;
	m_bIsSentenceWord = false;
}


CAudioSourceWave::~CAudioSourceWave( void )
{
#if _DEBUG
	if ( !CanDelete() )
		Assert(0);
#endif

	// for non-standard waves, we store a copy of the header in RAM
	delete[] m_pHeader;
	delete m_pTempSentence;
}

int CAudioSourceWave::GetType( void )
{
	return AUDIO_SOURCE_WAV;
}

void CAudioSourceWave::GetCacheData( CAudioSourceCachedInfo *info )
{
	Assert( info->Type() == CAudioSource::AUDIO_SOURCE_WAV );

	byte tempbuf[ 32768 ];
	int datalen = 0;
	if ( GetStartupData( tempbuf, sizeof( tempbuf ), datalen ) &&
		datalen > 0 && 
		info->s_bIsPrecacheSound )
	{
		byte *data = new byte[ datalen ];
		Q_memcpy( data, tempbuf, datalen );
		info->SetCachedDataSize(  datalen );
		info->SetCachedData( data );
	}

	info->SetBits( m_bits );
	info->SetChannels( m_channels );
	info->SetSampleSize( m_sampleSize );
	info->SetFormat( m_format );
	info->SetDataStart( m_dataStart );	// offset of wave data chunk
	info->SetDataSize( m_dataSize );		// size of wave data chunk
	info->SetSampleRate( m_rate );
	info->SetLoopStart( m_loopStart );
	info->SetSampleCount( m_sampleCount );

	if ( m_pTempSentence )
	{
		CSentence *scopy = new CSentence;
		*scopy = *m_pTempSentence;
		info->SetSentence( scopy );

		// Wipe it down to basically nothing
		delete m_pTempSentence;
		m_pTempSentence = NULL;
	}

	if ( m_pHeader && m_nHeaderSize > 0 )
	{
		byte *data = new byte[ m_nHeaderSize ];
		Q_memcpy( data, m_pHeader, m_nHeaderSize );
		info->SetHeaderSize( m_nHeaderSize );
		info->SetHeaderData( data );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Output : char const
//-----------------------------------------------------------------------------
char const *CAudioSourceWave::GetFileName()
{
	return m_pSfx ? m_pSfx->GetFileName() : "NULL m_pSfx";
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceWave::IsAsyncLoad()
{
	VPROF("CAudioSourceWave::IsAsyncLoad");

	if ( ( IsPC() || !IsX360() ) && !m_AudioCacheHandle.IsValid() )
	{
		m_AudioCacheHandle.Get( GetType(), m_pSfx->IsPrecachedSound(), m_pSfx, &m_nCachedDataSize );
	}

	// If there's a bit of "cached data" then we don't have to lazy/async load (we still async load the remaining data,
	//  but we run from the cache initially)
	if ( m_dataSize > snd_async_minsize.GetInt() )
		return true;
	return ( m_nCachedDataSize > 0 ) ? false : true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceWave::CheckAudioSourceCache()
{
	if ( IsX360() )
	{
		// 360 does not use audio cache files
		return;
	}

	Assert( m_pSfx );

	if ( !m_pSfx->IsPrecachedSound() )
	{
		return;
	}

	// This will "re-cache" this if it's not in this level's cache already
	m_AudioCacheHandle.Get( GetType(), true, m_pSfx, &m_nCachedDataSize );
}

//-----------------------------------------------------------------------------
// Purpose: Init the wave data.
// Input  : *pHeaderBuffer - the RIFF fmt chunk
//			headerSize - size of that chunk
//-----------------------------------------------------------------------------
void CAudioSourceWave::Init( const char *pHeaderBuffer, int headerSize )
{
	const WAVEFORMATEX *pHeader = (const WAVEFORMATEX *)pHeaderBuffer;

	// copy the relevant header data
	m_format = LittleWord( pHeader->wFormatTag );
	m_bits = LittleWord( pHeader->wBitsPerSample );
	m_rate = LittleDWord( pHeader->nSamplesPerSec );
	m_channels = LittleWord( pHeader->nChannels );
	m_sampleSize = (m_bits * m_channels)/8;
	
	// this can never be zero -- other functions divide by this. 
	// this should never happen, but avoid crashing
	if ( m_sampleSize <= 0 )
	{
		m_sampleSize = 1;
	}

	if ( m_format == WAVE_FORMAT_ADPCM )
	{
		// For non-standard waves (like ADPCM) store the header, it has the decoding coefficients
		m_pHeader = new char[headerSize];
		memcpy( m_pHeader, pHeader, headerSize );
		m_nHeaderSize = headerSize;
		
		// treat ADPCM sources as a file of bytes.  They are decoded by the mixer
		m_sampleSize = 1;
	}
}


int	CAudioSourceWave::SampleRate( void ) 
{ 
	return m_rate; 
}


//-----------------------------------------------------------------------------
// Purpose: Size of each sample
// Output : 
//-----------------------------------------------------------------------------
int	CAudioSourceWave::SampleSize( void ) 
{ 
	return m_sampleSize; 
}

//-----------------------------------------------------------------------------
// Purpose: Total number of samples in this source
// Output : int
//-----------------------------------------------------------------------------
int CAudioSourceWave::SampleCount( void ) 
{
	// caller wants real samples 
	return m_numDecodedSamples;
}

int	CAudioSourceWave::Format( void ) 
{ 
	return m_format; 
}

int	CAudioSourceWave::DataSize( void ) 
{ 
	return m_dataSize; 
}

bool CAudioSourceWave::IsVoiceSource()
{
	if ( GetSentence() )
	{
		if ( GetSentence()->GetVoiceDuck() )
			return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Do any sample conversion
//			For 8 bit PCM, convert to signed because the mixing routine assumes this
// Input  : *pData - pointer to sample data
//			sampleCount - number of samples
//-----------------------------------------------------------------------------
void CAudioSourceWave::ConvertSamples( char *pData, int sampleCount )
{
	if ( m_format == WAVE_FORMAT_PCM )
	{
		if ( m_bits == 8 )
		{
			for ( int i = 0; i < sampleCount*m_channels; i++ )
			{
				*pData = (unsigned char)((int)((unsigned)*pData) - 128);
				pData++;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Parse base chunks
// Input  : &walk - riff file to parse
//		  : chunkName - name of the chunk to parse
//-----------------------------------------------------------------------------
// UNDONE: Move parsing loop here and drop each chunk into a virtual function
//			instead of this being virtual.
void CAudioSourceWave::ParseChunk( IterateRIFF &walk, int chunkName )
{
	switch( chunkName )
	{
		case WAVE_CUE:
			ParseCueChunk( walk );
			break;
		case WAVE_SAMPLER:
			ParseSamplerChunk( walk );
			break;
		case WAVE_VALVEDATA:
			ParseSentence( walk );
			break;
		default:
			// unknown and don't care
			ChunkError( walk.ChunkName() );
			break;
	}
}

bool CAudioSourceWave::IsLooped( void ) 
{ 
	return (m_loopStart >= 0) ? true : false; 
}

bool CAudioSourceWave::IsStereoWav( void ) 
{ 
	return (m_channels == 2) ? true : false;
}

bool CAudioSourceWave::IsStreaming( void ) 
{ 
	return false; 
}


int CAudioSourceWave::GetCacheStatus( void )
{
	return AUDIO_IS_LOADED;
}

void CAudioSourceWave::CacheLoad( void )
{
}

void CAudioSourceWave::CacheUnload( void )
{
}

int	CAudioSourceWave::ZeroCrossingBefore( int sample )
{
	return sample;
}


int	CAudioSourceWave::ZeroCrossingAfter( int sample )
{
	return sample;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &walk - 
//-----------------------------------------------------------------------------
void CAudioSourceWave::ParseSentence( IterateRIFF &walk )
{
	CUtlBuffer buf( 0, 0, CUtlBuffer::TEXT_BUFFER );

	buf.EnsureCapacity( walk.ChunkSize() );
	walk.ChunkRead( buf.Base() );
	buf.SeekPut( CUtlBuffer::SEEK_HEAD, walk.ChunkSize() );

	m_pTempSentence = new CSentence();
	Assert( m_pTempSentence );

	m_pTempSentence->InitFromDataChunk( buf.Base(), buf.TellPut() );

	// Throws all phonemes into one word, discards sentence memory, etc.
	m_pTempSentence->MakeRuntimeOnly();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : CSentence
//-----------------------------------------------------------------------------
CSentence *CAudioSourceWave::GetSentence( void )
{
	if ( IsX360() )
	{
		return m_pTempSentence;
	}

	// Already checked and this wav doesn't have sentence data...
	if ( m_bNoSentence == true )
	{
		return NULL;
	}

	// Look up sentence from cache
	CAudioSourceCachedInfo *info = m_AudioCacheHandle.FastGet();
	if ( !info )
	{
		info = m_AudioCacheHandle.Get( CAudioSource::AUDIO_SOURCE_WAV, m_pSfx->IsPrecachedSound(), m_pSfx, &m_nCachedDataSize );
	}
	Assert( info );
	if ( !info )
	{
		m_bNoSentence = true;
		return NULL;
	}

	CSentence *sentence = info->Sentence();
	if ( !sentence )
	{
		m_bNoSentence = true;
		return NULL;
	}

	if ( sentence->m_bIsValid )
	{
		return sentence;
	}

	m_bNoSentence = true;

	return NULL;
}

const char *CAudioSourceWave::GetName()
{ 
	return m_pSfx ? m_pSfx->getname() : NULL; 
}

//-----------------------------------------------------------------------------
// Load a native xaudio or legacy wav
//-----------------------------------------------------------------------------
bool CAudioSourceWave::GetXboxAudioStartupData()
{
	CUtlBuffer	buf;
	char		fileName[MAX_PATH];
	char		tempFileName[MAX_PATH];
	
	MEM_ALLOC_CREDIT();

	// try native optimal xma wav file first
	Q_StripExtension( m_pSfx->GetFileName(), tempFileName, sizeof( tempFileName ) );
	Q_snprintf( fileName, sizeof( fileName ), "sound\\%s.360.wav", tempFileName );
	if ( !g_pFullFileSystem->ReadFile( fileName, "GAME", buf, sizeof( xwvHeader_t ) ) )
	{
		// not found, not supported
		return false;
	}
	else
	{
		xwvHeader_t* pHeader = (xwvHeader_t *)buf.Base();
		if ( pHeader->id != XWV_ID || pHeader->version != XWV_VERSION )
		{
			return false;
		}

		if ( pHeader->format == XWV_FORMAT_XMA )
		{
			m_format = WAVE_FORMAT_XMA;
		}
		else if ( pHeader->format == XWV_FORMAT_PCM )
		{
			m_format = WAVE_FORMAT_PCM;
		}
		else
		{
			// unknown
			return false;
		}

		m_rate = pHeader->GetSampleRate();
		m_channels = pHeader->channels;
		m_dataStart = pHeader->dataOffset;
		m_dataSize = pHeader->dataSize;

		m_loopStart = pHeader->loopStart;
		m_loopBlock = pHeader->loopBlock;
		m_numLeadingSamples	= pHeader->numLeadingSamples;
		m_numTrailingSamples = pHeader->numTrailingSamples;

		if ( m_format == WAVE_FORMAT_XMA )
		{
			// xma is compressed blocks, trick to fool system to treat data as bytes, not samples
			// unfortunate, but callers must know xma context and provide offsets in samples or bytes
			m_bits = 16;
			m_sampleSize = 1;
			m_sampleCount = m_dataSize;
		}
		else
		{
			m_bits = 16;
			m_sampleSize = sizeof( short ) * m_channels;
			m_sampleCount = m_dataSize / m_sampleSize;
		}

		// keep true decoded samples because cannot be easily determined
		m_numDecodedSamples = pHeader->numDecodedSamples;

		m_bNoSentence = true;

		CUtlBuffer fileBuffer;
		if ( pHeader->staticDataSize )
		{
			// get optional data
			if ( !g_pFullFileSystem->ReadFile( fileName, "GAME", fileBuffer, pHeader->staticDataSize, sizeof( xwvHeader_t ) ) )
			{
				return false;
			}

			unsigned char *pData = (unsigned char *)fileBuffer.Base() + sizeof( xwvHeader_t );
			if ( pHeader->GetSeekTableSize() )
			{
				// store off the seek table
				m_nHeaderSize = pHeader->GetSeekTableSize();
				m_pHeader = new char[m_nHeaderSize];
				V_memcpy( m_pHeader, pData, m_nHeaderSize );

				// advance past optional seek table
				pData += m_nHeaderSize;
			}

			if ( pHeader->vdatSize )
			{
				m_pTempSentence = new CSentence();
				Assert( m_pTempSentence );
				m_bNoSentence = false;

				// vdat is precompiled into minimal binary format and possibly compressed
				CLZMA lzma;
				
				if ( lzma.IsCompressed( pData ) )
				{
					// uncompress binary vdat and restore
					CUtlBuffer targetBuffer;
					int originalSize = lzma.GetActualSize( pData );
					targetBuffer.EnsureCapacity( originalSize );
					lzma.Uncompress( pData, (unsigned char *)targetBuffer.Base() );
					targetBuffer.SeekPut( CUtlBuffer::SEEK_HEAD, originalSize );
					m_pTempSentence->CacheRestoreFromBuffer( targetBuffer );
				}
				else
				{
					m_pTempSentence->CacheRestoreFromBuffer( fileBuffer );
				}
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Bastardized construction routine.  This is just to avoid complex
//			constructor functions so code can be shared more easily by sub-classes
// Input  : *pFormatBuffer - RIFF header
//			formatSize - header size
//			&walk - RIFF file
//-----------------------------------------------------------------------------
void CAudioSourceWave::Setup( const char *pFormatBuffer, int formatSize, IterateRIFF &walk )
{
	Init( pFormatBuffer, formatSize );

	while ( walk.ChunkAvailable() )
	{
		ParseChunk( walk, walk.ChunkName() );
		walk.ChunkNext();
	}
}


bool CAudioSourceWave::GetStartupData( void *dest, int destsize, int& bytesCopied )
{
	bytesCopied = 0;

	char formatBuffer[1024];
	const char *pName = m_pSfx->GetFileName();
	InFileRIFF riff( pName, *g_pSndIO );

	if ( riff.RIFFName() != RIFF_WAVE )
	{
		return false;
	}

	// set up the iterator for the whole file (root RIFF is a chunk)
	IterateRIFF walk( riff, riff.RIFFSize() );

	int format = 0;
	int formatSize = 0;

	// This chunk must be first as it contains the wave's format
	// break out when we've parsed it
	while ( walk.ChunkAvailable() && format == 0 )
	{
		switch( walk.ChunkName() )
		{
		case WAVE_FMT:
			{
				if ( walk.ChunkSize() <= sizeof( formatBuffer ) )
				{
					walk.ChunkRead( formatBuffer );
					formatSize = walk.ChunkSize();
					format = ((WAVEFORMATEX *)formatBuffer)->wFormatTag;
					if( ((WAVEFORMATEX *)formatBuffer)->wBitsPerSample > 16)
					{
						Warning("Unsupported %d-bit wave file %s\n", (int)((WAVEFORMATEX *)formatBuffer)->wBitsPerSample, pName);
					}
				}
			}
			break;
		default:
			{
				ChunkError( walk.ChunkName() );
			}
			break;
		}
		walk.ChunkNext();
	}

	// Not really a WAVE file or no format chunk, bail
	if ( !format )
	{
		return false;
	}

	Setup( formatBuffer, formatSize, walk );

	if ( !m_dataStart || !m_dataSize )
	{
		// failed during setup
		return false;
	}

	// requesting precache snippet as leader for streaming startup latency
	if ( destsize )
	{
		int file = g_pSndIO->open( m_pSfx->GetFileName() );
		if ( !file )
		{
			return false;
		}

		int bytesNeeded = m_channels * ( m_bits >> 3 ) * m_rate * SND_ASYNC_LOOKAHEAD_SECONDS;

		// Round to multiple of 4
		bytesNeeded = ( bytesNeeded + 3 ) & ~3;

		bytesCopied = min( destsize, m_dataSize );
		bytesCopied = min( bytesNeeded, bytesCopied );

		g_pSndIO->seek( file, m_dataStart );
		g_pSndIO->read( dest, bytesCopied, file );
		g_pSndIO->close( file );

		// some samples need to be converted
		ConvertSamples( (char *)dest, ( bytesCopied / m_sampleSize ) );
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: parses loop information from a cue chunk
// Input  : &walk - RIFF iterator
// Output : int loop start position
//-----------------------------------------------------------------------------
void CAudioSourceWave::ParseCueChunk( IterateRIFF &walk )
{
	// Cue chunk as specified by RIFF format
	// see $/research/jay/sound/riffnew.htm
	struct 
	{
		unsigned int dwName; 
		unsigned int dwPosition;
		unsigned int fccChunk;
		unsigned int dwChunkStart;
		unsigned int dwBlockStart; 
		unsigned int dwSampleOffset;
	} cue_chunk;

	int cueCount;

	// assume that the cue chunk stored in the wave is the start of the loop
	// assume only one cue chunk, UNDONE: Test this assumption here?
	cueCount = walk.ChunkReadInt();
	if ( cueCount > 0 )
	{
		walk.ChunkReadPartial( &cue_chunk, sizeof(cue_chunk) );
		m_loopStart = LittleLong( cue_chunk.dwSampleOffset );
	}
}

//-----------------------------------------------------------------------------
// Purpose: parses loop information from a 'smpl' chunk
// Input  : &walk - RIFF iterator
// Output : int loop start position
//-----------------------------------------------------------------------------
void CAudioSourceWave::ParseSamplerChunk( IterateRIFF &walk )
{
	// Sampler chunk for MIDI instruments
	// Parse loop info from this chunk too
	struct SampleLoop
	{
		unsigned int	dwIdentifier;
		unsigned int	dwType;
		unsigned int	dwStart;
		unsigned int	dwEnd;
		unsigned int	dwFraction;
		unsigned int	dwPlayCount;
	};

	struct 
	{
		unsigned int	dwManufacturer;
		unsigned int	dwProduct;
		unsigned int	dwSamplePeriod;
		unsigned int	dwMIDIUnityNote;
		unsigned int	dwMIDIPitchFraction;
		unsigned int	dwSMPTEFormat;
		unsigned int	dwSMPTEOffset;
		unsigned int	cSampleLoops;
		unsigned int	cbSamplerData;
		struct SampleLoop Loops[1];
	} samplerChunk;

	// assume that the loop end is the sample end
	// assume that only the first loop is relevant

	walk.ChunkReadPartial( &samplerChunk, sizeof(samplerChunk) );
	if ( LittleLong( samplerChunk.cSampleLoops ) > 0 )
	{
		// only support normal forward loops
		if ( LittleLong( samplerChunk.Loops[0].dwType ) == 0 )
		{
			m_loopStart = LittleLong( samplerChunk.Loops[0].dwStart );
		}
#ifdef _DEBUG
		else
		{
			Msg("Unknown sampler chunk type %d on %s\n", LittleLong( samplerChunk.Loops[0].dwType ), m_pSfx->GetFileName() );
		}
#endif
	}
	// else discard - this is some other non-loop sampler data we don't support
}


//-----------------------------------------------------------------------------
// Purpose: get the wave header
//-----------------------------------------------------------------------------
void *CAudioSourceWave::GetHeader( void )
{
	return m_pHeader;
}

//-----------------------------------------------------------------------------
// Gets the looping information. Some parameters are interpreted based on format
//-----------------------------------------------------------------------------
int CAudioSourceWave::GetLoopingInfo( int *pLoopBlock, int *pNumLeadingSamples, int *pNumTrailingSamples )
{
	if ( pLoopBlock )
	{
		// for xma, the block that contains the loop point
		*pLoopBlock = m_loopBlock;
	}

	if ( pNumLeadingSamples )
	{
		// for xma, the number of leading samples at the loop block to discard
		*pNumLeadingSamples = m_numLeadingSamples;
	}

	if ( pNumTrailingSamples )
	{
		// for xma, the number of trailing samples at the final block to discard
		*pNumTrailingSamples = m_numTrailingSamples;
	}

	// the loop point in samples
	return m_loopStart;
}

//-----------------------------------------------------------------------------
// Purpose: wrap the position wrt looping
// Input  : samplePosition - absolute position
// Output : int - looped position
//-----------------------------------------------------------------------------
int CAudioSourceWave::ConvertLoopedPosition( int samplePosition )
{
	if ( m_format == WAVE_FORMAT_XMA )
	{
		// xma mixer interprets loops and *always* sends a corrected position
		return samplePosition;
	}
	
	// if the wave is looping and we're past the end of the sample
	// convert to a position within the loop
	// At the end of the loop, we return a short buffer, and subsequent call
	// will loop back and get the rest of the buffer
	if ( m_loopStart >= 0 && samplePosition >= m_sampleCount )
	{
		// size of loop
		int loopSize = m_sampleCount - m_loopStart;
		// subtract off starting bit of the wave
		samplePosition -= m_loopStart;
		
		if ( loopSize )
		{
			// "real" position in memory (mod off extra loops)
			samplePosition = m_loopStart + (samplePosition % loopSize);
		}
		// ERROR? if no loopSize
	}

	return samplePosition;
}

//-----------------------------------------------------------------------------
// Purpose: remove the reference for the mixer getting deleted
// Input  : *pMixer - 
//-----------------------------------------------------------------------------
void CAudioSourceWave::ReferenceRemove( CAudioMixer *pMixer )
{
	m_refCount--;

	if ( m_refCount == 0 && ( ( IsPC() && IsPlayOnce() ) || ( IsX360() && IsStreaming() ) ) )
	{
		SetPlayOnce( false ); // in case it gets used again
		CacheUnload();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Add a mixer reference
// Input  : *pMixer - 
//-----------------------------------------------------------------------------
void CAudioSourceWave::ReferenceAdd( CAudioMixer *pMixer )
{
	m_refCount++;
}


//-----------------------------------------------------------------------------
// Purpose: return true if no mixers reference this source
//-----------------------------------------------------------------------------
bool CAudioSourceWave::CanDelete( void )
{
	if ( m_refCount > 0 )
		return false;

	return true;
}


// CAudioSourceMemWave is a bunch of wave data that is all in memory.
// To use it:
// - derive from CAudioSourceMemWave
// - call CAudioSourceWave::Init with a WAVEFORMATEX
// - set m_sampleCount.
// - implement GetDataPointer
class CAudioSourceMemWave : public CAudioSourceWave
{
public:
							CAudioSourceMemWave();
							CAudioSourceMemWave( CSfxTable *pSfx );
							CAudioSourceMemWave( CSfxTable *pSfx, CAudioSourceCachedInfo *info );
	virtual					~CAudioSourceMemWave();

	// These are all implemented by CAudioSourceMemWave.
	virtual CAudioMixer*	CreateMixer( int initialStreamPosition = 0 );
	virtual int				GetOutputData( void **pData, int samplePosition, int sampleCount, char copyBuf[AUDIOSOURCE_COPYBUF_SIZE] );
	virtual int				ZeroCrossingBefore( int sample );
	virtual int				ZeroCrossingAfter( int sample );

	virtual int				GetCacheStatus( void );
	virtual void			CacheLoad( void );
	virtual void			CacheUnload( void );

	// by definition, should already be in memory
	virtual void			Prefetch() {}

	virtual void			ParseChunk( IterateRIFF &walk, int chunkName );
	void					ParseDataChunk( IterateRIFF &walk );

protected:								   

	// Whoeover derives must implement this.
	virtual char			*GetDataPointer( void );

	memhandle_t				m_hCache;
	StreamHandle_t			m_hStream;

private:
	CAudioSourceMemWave( const CAudioSourceMemWave & ); // not implemented, not accessible
};


CAudioSourceMemWave::CAudioSourceMemWave() :
	CAudioSourceWave( NULL )
{
	m_hCache = 0;
	m_hStream = INVALID_STREAM_HANDLE;
}

CAudioSourceMemWave::CAudioSourceMemWave( CSfxTable *pSfx ) : 
	CAudioSourceWave( pSfx )
{
	m_hCache = 0;
	m_hStream = INVALID_STREAM_HANDLE;

	if ( IsX360() )
	{
		bool bValid = GetXboxAudioStartupData();
		if ( !bValid )
		{
			// failed, substitute placeholder
			pSfx->m_bUseErrorFilename = true;
			bValid = GetXboxAudioStartupData();
			if ( bValid )
			{
				DevWarning( "Failed to load sound \"%s\", substituting \"%s\"\n", pSfx->getname(), pSfx->GetFileName() );
			}
		}
	
		if ( bValid )
		{
			// a 360 memory wave is a critical resource kept locked in memory, load its data now
			CacheLoad();
		}
	}
}

CAudioSourceMemWave::CAudioSourceMemWave( CSfxTable *pSfx, CAudioSourceCachedInfo *info ) :
	CAudioSourceWave( pSfx, info )
{
	m_hCache = 0;
	m_hStream = INVALID_STREAM_HANDLE;
}

CAudioSourceMemWave::~CAudioSourceMemWave() 
{
}

//-----------------------------------------------------------------------------
// Purpose: Creates a mixer and initializes it with an appropriate mixer
//-----------------------------------------------------------------------------
CAudioMixer *CAudioSourceMemWave::CreateMixer( int initialStreamPosition )
{
	CAudioMixer *pMixer = CreateWaveMixer( CreateWaveDataMemory(*this), m_format, m_channels, m_bits, initialStreamPosition );
	if ( pMixer )
	{
		ReferenceAdd( pMixer );
	}

	return pMixer;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : **pData - output pointer to samples
//			samplePosition - position (in samples not bytes) 
//			sampleCount - number of samples (not bytes)
// Output : int - number of samples available
//-----------------------------------------------------------------------------
int CAudioSourceMemWave::GetOutputData( void **pData, int samplePosition, int sampleCount, char copyBuf[AUDIOSOURCE_COPYBUF_SIZE] )
{ 
	// handle position looping
	samplePosition = ConvertLoopedPosition( samplePosition );

	// how many samples are available (linearly not counting looping)
	int totalSampleCount = m_sampleCount - samplePosition;

	// may be asking for a sample out of range, clip at zero
	if ( totalSampleCount < 0 )
	{
		totalSampleCount = 0;
	}

	// clip max output samples to max available
	if ( sampleCount > totalSampleCount )
	{
		sampleCount = totalSampleCount;
	}

	// byte offset in sample database
	samplePosition *= m_sampleSize;

	// if we are returning some samples, store the pointer
	if ( sampleCount )
	{
		// Starting past end of "preloaded" data, just use regular cache
		if ( samplePosition >= m_nCachedDataSize )
		{
			*pData = GetDataPointer();
		}
		else
		{
			if ( IsPC() || !IsX360() )
			{
				// Start async loader if we haven't already done so
				CacheLoad();

				// Return less data if we are about to run out of uncached data
				if ( samplePosition + ( sampleCount * m_sampleSize ) >= m_nCachedDataSize )
				{
					sampleCount = ( m_nCachedDataSize - samplePosition ) / m_sampleSize;
				}

				// Point at preloaded/cached data from .cache file for now
				*pData = GetCachedDataPointer();
			}
			else
			{
				// for 360, memory wave data should have already been loaded and locked in cache
				Assert( 0 );
			}
		}

		if ( *pData )
		{
			*pData = (char *)*pData + samplePosition;
		}
		else
		{
			// End of data or some other problem
			sampleCount = 0;
		}
	}

	return sampleCount;
}


// Hardcoded macros to test for zero crossing
#define ZERO_X_8(b)		((b)<2 && (b)>-2)
#define ZERO_X_16(b)	((b)<512 && (b)>-512)

//-----------------------------------------------------------------------------
// Purpose: Search backward for a zero crossing starting at sample
// Input  : sample - starting point
// Output : position of zero crossing
//-----------------------------------------------------------------------------
int	CAudioSourceMemWave::ZeroCrossingBefore( int sample )
{
	char *pWaveData = GetDataPointer();

	if ( m_format == WAVE_FORMAT_PCM )
	{
		if ( m_bits == 8 )
		{
			char *pData = pWaveData + sample * m_sampleSize;
			bool zero = false;

			if ( m_channels == 1 )
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_8(*pData) )
						zero = true;
					else
					{
						sample--;
						pData--;
					}
				}
			}
			else
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_8(*pData) && ZERO_X_8(pData[1]) )
						zero = true;
					else
					{
						sample--;
						pData--;
					}
				}
			}
		}
		else
		{
			short *pData = (short *)(pWaveData + sample * m_sampleSize);
			bool zero = false;

			if ( m_channels == 1 )
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_16(*pData) )
						zero = true;
					else
					{
						pData--;
						sample--;
					}
				}
			}
			else
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_16(*pData) && ZERO_X_16(pData[1]) )
						zero = true;
					else
					{
						sample--;
						pData--;
					}
				}
			}
		}
	}
	return sample;
}


//-----------------------------------------------------------------------------
// Purpose: Search forward for a zero crossing
// Input  : sample - starting point
// Output : position of found zero crossing
//-----------------------------------------------------------------------------
int	CAudioSourceMemWave::ZeroCrossingAfter( int sample )
{
	char *pWaveData = GetDataPointer();

	if ( m_format == WAVE_FORMAT_PCM )
	{
		if ( m_bits == 8 )
		{
			char *pData = pWaveData + sample * m_sampleSize;
			bool zero = false;

			if ( m_channels == 1 )
			{
				while ( sample < SampleCount() && !zero )
				{
					if ( ZERO_X_8(*pData) )
						zero = true;
					else
					{
						sample++;
						pData++;
					}
				}
			}
			else
			{
				while ( sample < SampleCount() && !zero )
				{
					if ( ZERO_X_8(*pData) && ZERO_X_8(pData[1]) )
						zero = true;
					else
					{
						sample++;
						pData++;
					}
				}
			}
		}
		else
		{
			short *pData = (short *)(pWaveData + sample * m_sampleSize);
			bool zero = false;

			if ( m_channels == 1 )
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_16(*pData) )
						zero = true;
					else
					{
						pData++;
						sample++;
					}
				}
			}
			else
			{
				while ( sample > 0 && !zero )
				{
					if ( ZERO_X_16(*pData) && ZERO_X_16(pData[1]) )
						zero = true;
					else
					{
						sample++;
						pData++;
					}
				}
			}
		}
	}
	return sample;
}

//-----------------------------------------------------------------------------
// Purpose: parse chunks with unique processing to in-memory waves
// Input  : &walk - RIFF file
//-----------------------------------------------------------------------------
void CAudioSourceMemWave::ParseChunk( IterateRIFF &walk, int chunkName )
{
	switch( chunkName )
	{
		// this is the audio data
		case WAVE_DATA:
			ParseDataChunk( walk );
			return;
	}

	CAudioSourceWave::ParseChunk( walk, chunkName );
}


//-----------------------------------------------------------------------------
// Purpose: reads the actual sample data and parses it
// Input  : &walk - RIFF file
//-----------------------------------------------------------------------------
void CAudioSourceMemWave::ParseDataChunk( IterateRIFF &walk )
{	
	m_dataStart = walk.ChunkFilePosition() + 8;
	m_dataSize  = walk.ChunkSize();

	// 360 streaming model loads data later, but still needs critical member setup
	char *pData = NULL;
	if ( IsPC() || !IsX360() )
	{
		pData = GetDataPointer();
		if ( !pData )
		{
			Error( "CAudioSourceMemWave (%s): GetDataPointer() failed.", m_pSfx ? m_pSfx->GetFileName() : "m_pSfx = NULL" );
		}

		// load them into memory (bad!!, this is a duplicate read of the data chunk)
		walk.ChunkRead( pData );
	}

	if ( m_format == WAVE_FORMAT_PCM )
	{
		// number of samples loaded
		m_sampleCount = m_dataSize / m_sampleSize;
		m_numDecodedSamples = m_sampleCount;
	}
	else if ( m_format == WAVE_FORMAT_ADPCM )
	{
		// The ADPCM mixers treat the wave source as a flat file of bytes.
		// Since each "sample" is a byte (this is a flat file), the number of samples is the file size
		m_sampleCount = m_dataSize;
		m_sampleSize = 1;

		// file says 4, output is 16
		m_bits = 16;

		m_numDecodedSamples = ADPCMSampleCount( (ADPCMWAVEFORMAT *)m_pHeader, m_dataSize );
	}

	// some samples need to be converted
	if ( pData )
	{
		ConvertSamples( pData, m_sampleCount );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
int CAudioSourceMemWave::GetCacheStatus( void )
{
	VPROF("CAudioSourceMemWave::GetCacheStatus");

	if ( IsPC() || !IsX360() )
	{
		// NOTE: This will start the load if it isn't started
		bool bCacheValid;
		bool bCompleted = wavedatacache->IsDataLoadCompleted( m_hCache, &bCacheValid );
		if ( !bCacheValid )
		{
			wavedatacache->RestartDataLoad( &m_hCache, m_pSfx->GetFileName(), m_dataSize, m_dataStart );
		}
		if ( bCompleted )
			return AUDIO_IS_LOADED;
		if ( wavedatacache->IsDataLoadInProgress( m_hCache ) )
			return AUDIO_LOADING;
	}
	else
	{
		return wavedatacache->IsStreamedDataReady( m_hStream ) ? AUDIO_IS_LOADED : AUDIO_NOT_LOADED;
	}

	return AUDIO_NOT_LOADED;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceMemWave::CacheLoad( void )
{
	if ( IsPC() || !IsX360() )
	{
		// Commence lazy load?
		if ( m_hCache != 0 )
		{
			bool bCacheValid;
			wavedatacache->IsDataLoadCompleted( m_hCache, &bCacheValid );
			if ( !bCacheValid )
			{
				wavedatacache->RestartDataLoad( &m_hCache, m_pSfx->GetFileName(), m_dataSize, m_dataStart );
			}
			return;
		}

		m_hCache = wavedatacache->AsyncLoadCache( m_pSfx->GetFileName(), m_dataSize, m_dataStart );
	}
	else
	{
		if ( m_hStream == INVALID_STREAM_HANDLE )
		{
			// memory wave is resident
			const char *pFilename = m_pSfx->GetFileName();
			streamFlags_t streamFlags = STREAMED_FROMDVD;
			char szFilename[MAX_PATH];
			if ( m_format == WAVE_FORMAT_XMA || m_format == WAVE_FORMAT_PCM )
			{
				strcpy( szFilename, pFilename );
				V_SetExtension( szFilename, ".360.wav", sizeof( szFilename ) );
				pFilename = szFilename;

				// memory resident xma waves use the queued loader
				// restricting to XMA due to not correctly running a post ConvertSamples, which is not an issue for XMA
				if ( g_pQueuedLoader->IsMapLoading() )
				{
					// hint the wave data cache
					streamFlags |= STREAMED_QUEUEDLOAD;
				}
			}

			// open stream to load as a single monolithic buffer
			m_hStream = wavedatacache->OpenStreamedLoad( pFilename, m_dataSize, m_dataStart, 0, -1, m_dataSize, 1, streamFlags );
			if ( m_hStream != INVALID_STREAM_HANDLE && !( streamFlags & STREAMED_QUEUEDLOAD ) )
			{
				// block and finish load, convert data once right now
				char *pWaveData = (char *)wavedatacache->GetStreamedDataPointer( m_hStream, true );
				if ( pWaveData )
				{
					ConvertSamples( pWaveData, m_dataSize/m_sampleSize );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceMemWave::CacheUnload( void )
{
	if ( IsPC() || !IsX360() )
	{
		if ( m_hCache != 0 )
		{
			wavedatacache->Unload( m_hCache );
		}
	}
	else
	{
		if ( m_hStream != INVALID_STREAM_HANDLE )
		{
			wavedatacache->CloseStreamedLoad( m_hStream );
			m_hStream = INVALID_STREAM_HANDLE;
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Output : char
//-----------------------------------------------------------------------------
char *CAudioSourceMemWave::GetDataPointer( void )
{
	char *pWaveData = NULL;

	if ( IsPC() || !IsX360() )
	{
		bool bSamplesConverted = false;

		if ( m_hCache == 0 )
		{
			// not in cache, start loading
			CacheLoad();
		}

		// mount the requested data, blocks if necessary
		wavedatacache->GetDataPointer( 
			m_hCache, 
			m_pSfx->GetFileName(), 
			m_dataSize, 
			m_dataStart, 
			(void **)&pWaveData, 
			0, 
			&bSamplesConverted );

		// If we have reloaded data from disk (async) and we haven't converted the samples yet, do it now
		// FIXME:  Is this correct for stereo wavs?
		if ( pWaveData && !bSamplesConverted )
		{
			ConvertSamples( pWaveData, m_dataSize/m_sampleSize );
			wavedatacache->SetPostProcessed( m_hCache, true );
		}
	}
	else
	{
		if ( m_hStream != INVALID_STREAM_HANDLE )
		{
			// expected to be valid, unless failure during setup
			pWaveData = (char *)wavedatacache->GetStreamedDataPointer( m_hStream, true );
		}
	}

	return pWaveData;
}

//-----------------------------------------------------------------------------
// Purpose: Wave source for streaming wave files
// UNDONE: Handle looping
//-----------------------------------------------------------------------------
class CAudioSourceStreamWave : public CAudioSourceWave, public IWaveStreamSource
{
public:
	CAudioSourceStreamWave( CSfxTable *pSfx );
	CAudioSourceStreamWave( CSfxTable *pSfx, CAudioSourceCachedInfo *info );
	~CAudioSourceStreamWave();

	CAudioMixer		*CreateMixer( int initialStreamPosition = 0 );
	int				GetOutputData( void **pData, int samplePosition, int sampleCount, char copyBuf[AUDIOSOURCE_COPYBUF_SIZE] );
	void			ParseChunk( IterateRIFF &walk, int chunkName );
	bool			IsStreaming( void ) { return true; }

	virtual int		GetCacheStatus( void );

	// IWaveStreamSource
	virtual int UpdateLoopingSamplePosition( int samplePosition )
	{
		return ConvertLoopedPosition( samplePosition );
	}
	virtual void UpdateSamples( char *pData, int sampleCount )
	{
		ConvertSamples( pData, sampleCount );
	}
	virtual int GetLoopingInfo( int *pLoopBlock, int *pNumLeadingSamples, int *pNumTrailingSamples )
	{
		return CAudioSourceWave::GetLoopingInfo( pLoopBlock, pNumLeadingSamples, pNumTrailingSamples );
	}

	virtual void Prefetch();

	virtual int	SampleToStreamPosition( int samplePosition );
	virtual int	StreamToSamplePosition( int streamPosition );

private:
	CAudioSourceStreamWave( const CAudioSourceStreamWave & ); // not implemented, not accessible
};

//-----------------------------------------------------------------------------
// Purpose: Save a copy of the file name for instances to open later
// Input  : *pFileName - filename
//-----------------------------------------------------------------------------
CAudioSourceStreamWave::CAudioSourceStreamWave( CSfxTable *pSfx ) : CAudioSourceWave( pSfx )
{
	m_pSfx = pSfx;
	m_dataStart = -1;
	m_dataSize = 0;
	m_sampleCount = 0;

	if ( IsX360() )
	{
		bool bValid = GetXboxAudioStartupData();
		if ( !bValid )
		{
			// failed, substitute placeholder
			pSfx->m_bUseErrorFilename = true;
			bValid = GetXboxAudioStartupData();
			if ( bValid )
			{
				DevWarning( "Failed to load sound \"%s\", substituting \"%s\"\n", pSfx->getname(), pSfx->GetFileName() );
			}
		}
	}
}

CAudioSourceStreamWave::CAudioSourceStreamWave( CSfxTable *pSfx, CAudioSourceCachedInfo *info ) : 
	CAudioSourceWave( pSfx, info )
{
	m_pSfx = pSfx;
	m_dataStart = info->DataStart();
	m_dataSize = info->DataSize();

	m_sampleCount = info->SampleCount();
}

//-----------------------------------------------------------------------------
// Purpose: free the filename buffer
//-----------------------------------------------------------------------------
CAudioSourceStreamWave::~CAudioSourceStreamWave( void )
{
}


//-----------------------------------------------------------------------------
// Purpose: Create an instance (mixer & wavedata) of this sound
// Output : CAudioMixer * - pointer to the mixer
//-----------------------------------------------------------------------------
CAudioMixer *CAudioSourceStreamWave::CreateMixer( int initialStreamPosition )
{
	char fileName[MAX_PATH];
	const char *pFileName = m_pSfx->GetFileName();
	if ( IsX360() && ( m_format == WAVE_FORMAT_XMA || m_format == WAVE_FORMAT_PCM ) )
	{
		strcpy( fileName, pFileName );
		V_SetExtension( fileName, ".360.wav", sizeof( fileName ) );
		pFileName = fileName;

		// for safety, validate the initial stream position
		// not trusting save/load
		if ( m_format == WAVE_FORMAT_XMA )
		{
			if ( ( initialStreamPosition % XBOX_DVD_SECTORSIZE ) || 
				( initialStreamPosition % XMA_BLOCK_SIZE ) ||
				( initialStreamPosition >= m_dataSize ) )
			{
				initialStreamPosition = 0;
			}
		}
	}

	// BUGBUG: Source constructs the IWaveData, mixer frees it, fix this?
	IWaveData *pWaveData = CreateWaveDataStream( *this, static_cast<IWaveStreamSource *>(this), pFileName, m_dataStart, m_dataSize, m_pSfx, initialStreamPosition );
	if ( pWaveData )
	{
		CAudioMixer *pMixer = CreateWaveMixer( pWaveData, m_format, m_channels, m_bits, initialStreamPosition );
		if ( pMixer )
		{
			ReferenceAdd( pMixer );
			return pMixer;
		}

		// no mixer, delete the stream buffer/instance
		delete pWaveData;
	}

	return NULL;
}

void CAudioSourceStreamWave::Prefetch()
{
	PrefetchDataStream( m_pSfx->GetFileName(), m_dataStart, m_dataSize );
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CAudioSourceStreamWave::SampleToStreamPosition( int samplePosition )
{
	if ( IsPC() )
	{
		// not for PC
		Assert( 0 );
		return 0;
	}

	if ( m_format != WAVE_FORMAT_XMA || !m_nHeaderSize )
	{
		// not in the expected format or lacking the seek table
		return 0;
	}

	// Run through the seek table to find the block closest to the desired sample. 
	// Each seek table entry is the index (counting from the beginning of the file) 
	// of the first sample in the corresponding block, but there's no entry for the 
	// first block (since the index would always be zero).
	int *pSeekTable = (int*)m_pHeader;
	int packet = 0;
	for ( int i = 0; i < m_nHeaderSize/(int)sizeof( int ); ++i )
	{
		if ( samplePosition < pSeekTable[i] )
		{
			packet = i;
			break;
		}
	}

	int streamPosition = ( packet == 0 ) ? 0 : ( packet - 1 ) * 2048;
	return streamPosition;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int CAudioSourceStreamWave::StreamToSamplePosition( int streamPosition )
{
	if ( IsPC() )
	{
		// not for PC
		Assert( 0 );
		return 0;
	}

	if ( m_format != WAVE_FORMAT_XMA || !m_nHeaderSize )
	{
		// not in the expected format or lacking the seek table
		return 0;
	}

	int packet = streamPosition/2048;
	if ( packet <= 0 )
	{
		return 0;
	}
	if ( packet > m_nHeaderSize/(int)sizeof( int ) )
	{
		return m_numDecodedSamples;
	}

	return ((int*)m_pHeader)[packet - 1];
}

//-----------------------------------------------------------------------------
// Purpose: Parse a stream wave file chunk
//			unlike the in-memory file, don't load the data, just get a reference to it.
// Input  : &walk - RIFF file
//-----------------------------------------------------------------------------
void CAudioSourceStreamWave::ParseChunk( IterateRIFF &walk, int chunkName )
{
	// NOTE: It would be nice to break out of parsing once we have the data start and
	//		save seeking over the whole file.  But to do so, the other needed chunks must occur
	//		before the DATA chunk.  But, that is not standard and breaks most other wav parsers.

	switch( chunkName )
	{
	case WAVE_DATA:
		// data starts at chunk + 8 (chunk name, chunk size = 2*4=8 bytes)
		// don't load the data, just know where it is so each instance 
		// can load it later
		m_dataStart = walk.ChunkFilePosition() + 8;
		m_dataSize = walk.ChunkSize();
		m_sampleCount = m_dataSize / m_sampleSize;
		return;
	}
	CAudioSourceWave::ParseChunk( walk, chunkName );
}

//-----------------------------------------------------------------------------
// Purpose: This is not implemented here.  This source has no data.  It is the
//			WaveData's responsibility to load/serve the data
//-----------------------------------------------------------------------------
int CAudioSourceStreamWave::GetOutputData( void **pData, int samplePosition, int sampleCount, char copyBuf[AUDIOSOURCE_COPYBUF_SIZE] )
{
	return 0;
}

int CAudioSourceStreamWave::GetCacheStatus( void )
{
	if ( !m_dataSize || !m_dataStart )
	{
		// didn't get precached properly
		return AUDIO_NOT_LOADED;
	}

	return AUDIO_IS_LOADED;
}


//-----------------------------------------------------------------------------
// Purpose: Create a wave audio source (streaming or in memory)
// Input  : *pName - file name (NOTE: CAUDIOSOURCE KEEPS A POINTER TO pSfx)
//			streaming - if true, don't load, stream each instance
// Output : CAudioSource * - a new source
//-----------------------------------------------------------------------------
CAudioSource *CreateWave( CSfxTable *pSfx, bool bStreaming )
{
	Assert( pSfx );

#if defined( _DEBUG )
	// For some reason you can't usually do pSfx->getname() in the dev studio debugger, so for convenience we'll grab the name
	// here in debug builds at least...
	char const *pName = pSfx->getname();
	NOTE_UNUSED( pName );
#endif

	CAudioSourceWave *pWave = NULL;

	if ( IsPC() || !IsX360() )
	{
		// Caching should always work, so if we failed to cache, it's a problem reading the file data, etc.
		bool bIsMapSound = pSfx->IsPrecachedSound();
		CAudioSourceCachedInfo *pInfo = audiosourcecache->GetInfo( CAudioSource::AUDIO_SOURCE_WAV, bIsMapSound, pSfx );

		if ( pInfo && pInfo->Type() != CAudioSource::AUDIO_SOURCE_UNK )
		{
			// create the source from this file
			if ( bStreaming )
			{
				pWave = new CAudioSourceStreamWave( pSfx, pInfo );
			}
			else
			{
				pWave = new CAudioSourceMemWave( pSfx, pInfo );
			}
		}
	}
	else
	{
		// 360 does not use audio cache system
		// create the desired type
		if ( bStreaming )
		{
			pWave = new CAudioSourceStreamWave( pSfx );
		}
		else
		{
			pWave = new CAudioSourceMemWave( pSfx );
		}
	}

	if ( pWave && !pWave->Format() )
	{
		// lack of format indicates failure
		delete pWave;
		pWave = NULL;
	}

	return pWave;
}


//-----------------------------------------------------------------------------
// Purpose: Wrapper for CreateWave()
//-----------------------------------------------------------------------------
CAudioSource *Audio_CreateStreamedWave( CSfxTable *pSfx )
{
#if defined( MP3_SUPPORT )
	if ( Audio_IsMP3( pSfx->GetFileName() ) )
	{
		return Audio_CreateStreamedMP3( pSfx );
	}
#endif

	return CreateWave( pSfx, true );
}


//-----------------------------------------------------------------------------
// Purpose: Wrapper for CreateWave()
//-----------------------------------------------------------------------------
CAudioSource *Audio_CreateMemoryWave( CSfxTable *pSfx )
{
#if defined( MP3_SUPPORT )
	if ( Audio_IsMP3( pSfx->GetFileName() ) )
	{
		return Audio_CreateMemoryMP3( pSfx );
	}
#endif

	return CreateWave( pSfx, false );
}

float GetMP3Duration_Helper( char const *filename );
static float Audio_GetMP3Duration( char const *pName )
{
	// Deduce from file
	return GetMP3Duration_Helper( pName );
}


void MaybeReportMissingWav( char const *wav )
{
	static CUtlSymbolTable wavErrors;

	CUtlSymbol sym;
	sym = wavErrors.Find( wav );
	if ( UTL_INVAL_SYMBOL == sym )
	{
		// See if file exists
		if ( g_pFullFileSystem->FileExists( wav ) )
		{
			DevWarning( "Bad Audio file '%s'\n", wav );
		}
		else
		{
			DevWarning( "Missing wav file '%s'\n", wav );
		}
		wavErrors.AddString( wav );
	}
}

static float Audio_GetWaveDuration( char const *pName )
{
	if ( IsX360() )
	{
		// should have precached
		return 0;
	}

	char formatBuffer[1024];
	WAVEFORMATEX *pfmt = (WAVEFORMATEX *)formatBuffer;

	InFileRIFF riff( pName, *g_pSndIO );

	if ( riff.RIFFName() != RIFF_WAVE )
	{
		MaybeReportMissingWav( pName );
		return 0.0f;
	}

	// set up the iterator for the whole file (root RIFF is a chunk)
	IterateRIFF walk( riff, riff.RIFFSize() );

	int format = 0;
	int formatSize = 0;
	int sampleCount = 0;

	// This chunk must be first as it contains the wave's format
	// break out when we've parsed it
	while ( walk.ChunkAvailable() && ( format == 0 || sampleCount == 0 ) )
	{
		switch( walk.ChunkName() )
		{
		case WAVE_FMT:
			if ( walk.ChunkSize() <= sizeof( formatBuffer ) )
			{
				walk.ChunkRead( formatBuffer );
				formatSize = walk.ChunkSize();
				format = LittleWord( pfmt->wFormatTag );
			}
			break;
		case WAVE_DATA:
			if ( format != 0 )
			{
				int dataSize = walk.ChunkSize();
				if ( format == WAVE_FORMAT_ADPCM )
				{
					// Dummy size for now
					sampleCount = dataSize;
				}
				else
				{
					sampleCount = dataSize / ( LittleWord( pfmt->wBitsPerSample ) >> 3 );
				}
			}
			break;
		default:
			ChunkError( walk.ChunkName() );
			break;
		}
		walk.ChunkNext();
	}

	// Not really a WAVE file or no format chunk, bail
	if ( !format || !sampleCount )
		return 0.0f;

	float sampleRate = LittleDWord( pfmt->nSamplesPerSec );

	if ( format == WAVE_FORMAT_ADPCM )
	{
		// Determine actual duration
		sampleCount = ADPCMSampleCount( (ADPCMWAVEFORMAT *)formatBuffer, sampleCount );
	}

	return (float)sampleCount / sampleRate;
}

//-----------------------------------------------------------------------------
// Purpose: Fast method for determining duration of .wav/.mp3, exposed to server as well
// Input  : *pName - 
// Output : float
//-----------------------------------------------------------------------------
float AudioSource_GetSoundDuration( char const *pName )
{
#if defined( MP3_SUPPORT )
	if ( Audio_IsMP3( pName ) )
	{
		return Audio_GetMP3Duration( pName );
	}
#endif

	CSfxTable *pSound = S_PrecacheSound( pName );
	if ( pSound )
	{
		return AudioSource_GetSoundDuration( pSound );
	}

	return Audio_GetWaveDuration( pName );
}

float AudioSource_GetSoundDuration( CSfxTable *pSfx )
{
	if ( pSfx && pSfx->pSource )
	{
		return (float)pSfx->pSource->SampleCount() / (float)pSfx->pSource->SampleRate();
	}

	return 0;
}

CAudioSourceCachedInfo::CAudioSourceCachedInfo() :
	infolong( 0 ),
	flagsbyte( 0 ),
	m_dataStart( 0 ),
	m_dataSize( 0 ),
	m_loopStart( 0 ),
	m_sampleCount( 0 ),
	m_usCachedDataSize( 0 ),
	m_pCachedData( 0 ),
	m_usHeaderSize( 0 ),
	m_pHeader( 0 ),
	m_pSentence( 0 )
{
}

CAudioSourceCachedInfo& CAudioSourceCachedInfo::operator =( const CAudioSourceCachedInfo& src )
{
	if ( this == &src )
		return *this;

	infolong = src.infolong;
	flagsbyte = src.flagsbyte;
	SetDataStart( src.DataStart() );
	SetDataSize( src.DataSize() );
	SetLoopStart( src.LoopStart() );
	SetSampleCount( src.SampleCount() );

	CSentence *scopy = NULL;
	if ( src.Sentence() )
	{
		scopy = new CSentence();
		*scopy = *src.Sentence();
	}
	SetSentence( scopy );

	byte *data = NULL;

	Assert( src.CachedDataSize() == 0 || src.CachedData() );

	m_usCachedDataSize = 0;

	if ( src.CachedData() && src.CachedDataSize() > 0 )
	{
		SetCachedDataSize( src.CachedDataSize() );
		data = new byte[ src.CachedDataSize() ];
		Assert( data );
		Q_memcpy( data, src.CachedData(), src.CachedDataSize() );
	}

	SetCachedData( data );

	data = NULL;

	Assert( src.HeaderSize() == 0 || src.HeaderData() );

	m_usHeaderSize = 0;

	if ( src.HeaderData() && src.HeaderSize() > 0 )
	{
		SetHeaderSize( src.HeaderSize() );
		data = new byte[ src.HeaderSize() ];
		Assert( data );
		Q_memcpy( data, src.HeaderData(), src.HeaderSize() );
	}

	SetHeaderData( data );

	return *this;
}

CAudioSourceCachedInfo::CAudioSourceCachedInfo( const CAudioSourceCachedInfo& src )
{
	if ( this == &src )
	{
		Assert( 0 );
		return;
	}

	infolong = src.infolong;
	flagsbyte = src.flagsbyte;
	SetDataStart( src.DataStart() );
	SetDataSize( src.DataSize() );
	SetLoopStart( src.LoopStart() );
	SetSampleCount( src.SampleCount() );

	CSentence *scopy = NULL;
	if ( src.Sentence() )
	{
		scopy = new CSentence();
		*scopy = *src.Sentence();
	}
	SetSentence( scopy );

	byte *data = NULL;

	Assert( src.CachedDataSize() == 0 || src.CachedData() );

	m_usCachedDataSize = 0;

	if ( src.CachedData() && src.CachedDataSize() > 0 )
	{
		SetCachedDataSize( src.CachedDataSize() );
		data = new byte[ src.CachedDataSize() ];
		Assert( data );
		Q_memcpy( data, src.CachedData(), src.CachedDataSize() );
	}

	SetCachedData( data );

	data = NULL;

	Assert( src.HeaderSize() == 0 || src.HeaderData() );

	m_usHeaderSize = 0;

	if ( src.HeaderData() && src.HeaderSize() > 0 )
	{
		SetHeaderSize( src.HeaderSize() );
		data = new byte[ src.HeaderSize() ];
		Assert( data );
		Q_memcpy( data, src.HeaderData(), src.HeaderSize() );
	}

	SetHeaderData( data );
}

CAudioSourceCachedInfo::~CAudioSourceCachedInfo()
{
	Clear();
}

void CAudioSourceCachedInfo::Clear()
{
	infolong = 0;
	flagsbyte = 0;
	m_dataStart = 0;
	m_dataSize = 0;
	m_loopStart = 0;
	m_sampleCount = 0;

	delete m_pSentence;
	m_pSentence = NULL;

	delete[] m_pCachedData;
	m_pCachedData = NULL;
	m_usCachedDataSize = 0;

	delete[] m_pHeader;
	m_pHeader = NULL;
	m_usHeaderSize = 0;
}

void CAudioSourceCachedInfo::RemoveData()
{
	delete[] m_pCachedData;
	m_pCachedData = NULL;
	m_usCachedDataSize = 0;
	flags.m_bCachedData = false;
}

void CAudioSourceCachedInfo::Save( CUtlBuffer& buf )
{
	buf.PutInt( infolong );
	buf.PutChar( flagsbyte );
	buf.PutInt( m_dataStart );
	buf.PutInt( m_dataSize );
	buf.PutInt( m_loopStart );
	buf.PutInt( m_sampleCount );

	if ( flags.m_bSentence )
	{
		m_pSentence->CacheSaveToBuffer( buf, CACHED_SENTENCE_VERSION );
	}

	Assert( m_usCachedDataSize < 65535 );

	if ( flags.m_bCachedData && m_pCachedData )
	{
		buf.PutInt( m_usCachedDataSize );
		buf.Put( m_pCachedData, m_usCachedDataSize );
	}

	Assert( m_usHeaderSize <= 32767 );

	if ( flags.m_bHeader )
	{
		buf.PutShort( m_usHeaderSize );
		buf.Put( m_pHeader, m_usHeaderSize );
	}
}

void CAudioSourceCachedInfo::Restore( CUtlBuffer& buf )
{
	// Wipe any old data!!!
	Clear();

	infolong		= buf.GetInt();
	flagsbyte		= buf.GetChar();
	m_dataStart		= buf.GetInt();
	m_dataSize		= buf.GetInt();
	m_loopStart		= buf.GetInt();
	m_sampleCount	= buf.GetInt();
	if ( flags.m_bSentence )
	{
		m_pSentence = new CSentence();
		Assert( m_pSentence );
		m_pSentence->CacheRestoreFromBuffer( buf );
	}

	if ( flags.m_bCachedData )
	{
		m_usCachedDataSize = buf.GetInt();
		Assert( m_usCachedDataSize > 0 && m_usCachedDataSize < 65535 );
		if ( m_usCachedDataSize > 0 )
		{
			byte *data = new byte[ m_usCachedDataSize ];
			buf.Get( data, m_usCachedDataSize );
			SetCachedData( data );
		}
	}

	if ( flags.m_bHeader )
	{
		m_usHeaderSize = buf.GetShort();
		Assert( m_usHeaderSize > 0 && m_usHeaderSize <= 32767 );
		if ( m_usHeaderSize > 0 )
		{
			byte *data = new byte[ m_usHeaderSize ];
			buf.Get( data, m_usHeaderSize );
			SetHeaderData( data );
		}
	}
}

int CAudioSourceCachedInfo::s_CurrentType = CAudioSource::AUDIO_SOURCE_MAXTYPE;
CSfxTable *CAudioSourceCachedInfo::s_pSfx = NULL;
bool CAudioSourceCachedInfo::s_bIsPrecacheSound = false;

void CAudioSourceCachedInfo::Rebuild( char const *filename )
{
	// Wipe any old data
	Clear();

	Assert( s_pSfx );
	Assert( s_CurrentType != CAudioSource::AUDIO_SOURCE_MAXTYPE );

#if 0 
	// Never cachify something which is not in the client precache list
	if ( s_bIsPrecacheSound != s_pSfx->IsPrecachedSound() )
	{
		Msg( "Logic bug, precaching entry for '%s' which is not in precache list\n",
			filename );
	}
#endif

	SetType( s_CurrentType );

	CAudioSource *as = NULL;

	// Note though these instantiate a specific AudioSource subclass, it doesn't matter, we just need one for .wav and one for .mp3
	switch ( s_CurrentType )
	{
	default:
	case CAudioSource::AUDIO_SOURCE_VOICE:
		break;
	case CAudioSource::AUDIO_SOURCE_WAV:
		as = new CAudioSourceMemWave( s_pSfx );
		break;
	case CAudioSource::AUDIO_SOURCE_MP3:
#if defined( MP3_SUPPORT )
		as = new CAudioSourceMP3Cache( s_pSfx );
#endif
		break;
	}

	if ( as )
	{
		as->GetCacheData( this );
		delete as;
	}
}

#define AUDIOSOURCE_CACHE_VERSION	 3
class CAudioSourceCache : public IAudioSourceCache
{
public:
	typedef CUtlCachedFileData< CAudioSourceCachedInfo > CacheType_t;

	CAudioSourceCache()
	{
		Q_memset( m_szCurrentLanguage, 0, sizeof( m_szCurrentLanguage ) );

		Q_memset( m_szCurrentLevel, 0, sizeof( m_szCurrentLevel ) );
		Q_memset( m_szMapCache, 0, sizeof( m_szMapCache ) );
		Q_memset( m_szMapCacheBase, 0, sizeof( m_szMapCacheBase ) );
		Q_memset( m_szOtherSoundsCache, 0, sizeof( m_szOtherSoundsCache ) );
		Q_memset( m_szSharedPrecacheCache, 0, sizeof( m_szSharedPrecacheCache ) );
		m_pCache = NULL;
		m_pOtherSoundCache = NULL;
		m_pSharedPrecacheCache = NULL;
		m_pBuildingCache = NULL;
		m_bBuildingFullDataCache = false;
		m_bFirstTime = true;
		m_nServerCount = -1;
		m_pLastWorldModel = NULL;
		m_bSndCacheDebug = false;
	}

	bool Init( unsigned int memSize );
	void Shutdown();

	void CheckCacheBuild();

	static unsigned int AsyncLookaheadMetaChecksum( void );

	void LevelInit( char const *mapname );
	void LevelShutdown();

	virtual CAudioSourceCachedInfo *GetInfo( int audiosourcetype, bool soundisprecached, CSfxTable *sfx );
	virtual void RebuildCacheEntry( int audiosourcetype, bool soundisprecached, CSfxTable *sfx );
	virtual void ForceRecheckDiskInfo();

	bool FastBuildSharedPrecachedSoundsCache( bool fullbuild, bool showprogress, bool bForceBuild );
	void	WriteManifest();
private:
	// Purpose: 
	CacheType_t *LookUpCacheEntry( const char *fn, int audiosourcetype, bool soundisprecached, CSfxTable *sfx );

	struct AudioSourceUsage_t
	{
		AudioSourceUsage_t() :
			handle( 0 ),
			count( 0u )
		{
		}
		FileNameHandle_t	handle;
		unsigned int		count;
	};

	static bool AudioSourceUsageLessFunc( const AudioSourceUsage_t& lhs, const AudioSourceUsage_t& rhs )
	{
		return lhs.handle < rhs.handle;
	}

	CacheType_t *AllocAudioCache( char const *cachename );

	void AnalyzeReslists( CUtlRBTree< FileNameHandle_t, int >& other, CUtlRBTree< FileNameHandle_t, int >& sharedprecache );
	bool FastBuildAllMissingSoundCaches( CacheType_t *pOther, CacheType_t *pSharedPrecache, bool showprogress = false, bool forcerebuild = false, float flProgressStart = 0.0f, float flProgressEnd = 1.0f );
	bool FastBuildSoundCache( float progressfrac, char const *currentcache, CacheType_t *fullCache, CacheType_t *precacheCache, CacheType_t *cache, char const *manifest );
	CacheType_t *BuildCacheFromList( char const *cachename, CUtlRBTree< FileNameHandle_t, int >& list, bool fulldata, bool showprogress = false, float flProgressStart = 0.0f, float flProgressEnd = 1.0f );
	CacheType_t *BuildNoDataCacheFromFullDataCache( char const *cachename, CacheType_t *fulldata, bool showprogress = false, float flProgressStart = 0.0f, float flProgressEnd = 1.0f );
	bool IsValidCache( char const *cachename );
	void RemoveCache( char const *cachename );
	bool HasMissingCaches();

	void KillCache();

	char const				*GetAudioCacheLanguageSuffix( char *buf, size_t bufsize );

	enum
	{
		MAX_LANGUAGE_NAME = 64,
		MAX_LEVEL_NAME = 128,
		MAX_REPOSITORY_NAME = 256,
		MAX_LIST_SIZE = 1024
	};

	char				m_szCurrentLevel[ MAX_LEVEL_NAME ];
	char				m_szMapCache[ MAX_REPOSITORY_NAME ];
	char				m_szMapCacheBase[ MAX_REPOSITORY_NAME ];
	char				m_szOtherSoundsCache[ MAX_REPOSITORY_NAME ];
	char				m_szSharedPrecacheCache[ MAX_REPOSITORY_NAME ];
	char				m_szCurrentLanguage[ MAX_LANGUAGE_NAME ];

	typedef enum
	{
		CACHE_MAP = 0,
		CACHE_SHARED,
		CACHE_OTHER,
		CACHE_BUILDING
	} SoundCacheType_t;

	void					SetCachePointer( SoundCacheType_t ptrType, CacheType_t *ptr );

	// Current level specific sounds (only in precache list)
	CacheType_t				*m_pCache;
	// All sounds (no startup data) referenced anywhere in game
	CacheType_t				*m_pOtherSoundCache;
	// Current level specific sounds which are shared across a bunchof levels
	CacheType_t				*m_pSharedPrecacheCache;

	CacheType_t				*m_pBuildingCache;
	bool					m_bBuildingFullDataCache;

	bool					m_bFirstTime;
	int						m_nServerCount;
	model_t					*m_pLastWorldModel;
	bool					m_bSndCacheDebug;
};

static CAudioSourceCache	g_ASCache;
IAudioSourceCache *audiosourcecache = &g_ASCache;

unsigned int CAudioSourceCachedInfoHandle_t::s_nCurrentFlushCount = 1;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceCachedInfoHandle_t::InvalidateCache()
{
	++s_nCurrentFlushCount;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::Init( unsigned int memSize )
{
#if defined( _DEBUG )
	Msg( "CAudioSourceCache: Init\n" );
#endif

	m_bSndCacheDebug = CommandLine()->FindParm( "-sndcachedebug" ) ? true : false;

	GetAudioCacheLanguageSuffix( m_szCurrentLanguage, sizeof( m_szCurrentLanguage ) );

	if ( m_bSndCacheDebug )
	{
		DevMsg(	1, "Audio Caches using '%s' as suffix\n", m_szCurrentLanguage );
	}

	if ( !wavedatacache->Init( memSize ) )
	{
		Error( "Unable to init wavedatacache system\n" );
		return false;
	}

	if ( IsX360() )
	{
		// 360 doesn't use audio source caches
		return true;
	}

	g_pFullFileSystem->CreateDirHierarchy( AUDIOSOURCE_CACHE_ROOTDIR, "MOD" );

	Q_snprintf( m_szOtherSoundsCache, sizeof( m_szOtherSoundsCache ), "%s/_other%s.cache", AUDIOSOURCE_CACHE_ROOTDIR, m_szCurrentLanguage );
	Q_snprintf( m_szSharedPrecacheCache, sizeof( m_szSharedPrecacheCache ), "%s/_sharedprecache%s.cache", AUDIOSOURCE_CACHE_ROOTDIR, m_szCurrentLanguage );

	if ( m_bSndCacheDebug )
	{
		DevMsg(	1, "Other Cache :  '%s'\n", m_szOtherSoundsCache );
		DevMsg(	1, "Shared Cache:  '%s'\n", m_szSharedPrecacheCache );
	}

	CacheType_t *cache = AllocAudioCache( m_szOtherSoundsCache );

	SetCachePointer( CACHE_OTHER, cache );

	Assert( m_pOtherSoundCache );
	if ( !m_pOtherSoundCache->Init() )
	{
		Error( "Failed to init 'other' sound cache '%s'\n", m_szOtherSoundsCache );
		delete m_pOtherSoundCache;
		m_pOtherSoundCache = NULL;
	}

	cache = AllocAudioCache( m_szSharedPrecacheCache );

	SetCachePointer( CACHE_SHARED, cache );

	Assert( m_pSharedPrecacheCache );
	if ( !m_pSharedPrecacheCache->Init() )
	{
		Error( "Failed to init 'shared precache' sound cache '%s'\n", m_szSharedPrecacheCache );
		delete m_pSharedPrecacheCache;
		m_pSharedPrecacheCache = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceCache::Shutdown()
{
#if defined( _DEBUG )
	Msg( "CAudioSourceCache: Shutdown\n" );
#endif

	if ( !IsX360() || IsPC() )
	{
		KillCache();

		if ( m_pSharedPrecacheCache )
		{
			m_pSharedPrecacheCache->Shutdown();
			delete m_pSharedPrecacheCache;
		}

		SetCachePointer( CACHE_SHARED, NULL );

		if ( m_pOtherSoundCache )
		{
			m_pOtherSoundCache->Shutdown();
			delete m_pOtherSoundCache;
		}

		SetCachePointer( CACHE_OTHER, NULL );

		m_szCurrentLevel[0] = 0;
		m_szMapCache[0] = 0;
		m_szMapCacheBase[0] = 0;
	}

	wavedatacache->Shutdown();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *cachename - 
// Output : CacheType_t
//-----------------------------------------------------------------------------
CAudioSourceCache::CacheType_t *CAudioSourceCache::AllocAudioCache( char const *cachename )
{
	if ( IsX360() )
	{
		return NULL;
	}

	CacheType_t *cache = new CacheType_t( cachename, AUDIOSOURCE_CACHE_VERSION, AsyncLookaheadMetaChecksum, UTL_CACHED_FILE_USE_FILESIZE );
	return cache;
}

//-----------------------------------------------------------------------------
// Purpose: Called by Host_Init on engine startup to rebuild everything if needed
//-----------------------------------------------------------------------------
void CAudioSourceCache::CheckCacheBuild()
{
	if ( IsX360() )
	{
		return;
	}

	Assert( g_pFullFileSystem );

	if ( m_bFirstTime )
	{
		FastBuildSharedPrecachedSoundsCache( false, true, false );
		m_bFirstTime = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Static method
// Output : unsigned int
//-----------------------------------------------------------------------------
unsigned int CAudioSourceCache::AsyncLookaheadMetaChecksum( void )
{
	if ( IsX360() )
	{
		return 0;
	}

	CRC32_t crc;
	CRC32_Init( &crc );

	float f = SND_ASYNC_LOOKAHEAD_SECONDS;
	CRC32_ProcessBuffer( &crc, &f, sizeof( f ) );
	// Finish
	CRC32_Final( &crc );

	return (unsigned int)crc;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *mapname - 
//-----------------------------------------------------------------------------
void CAudioSourceCache::LevelInit( char const *mapname )
{
	if ( IsX360() )
	{
		// 360 not using
		return;
	}

	if ( !Q_stricmp( mapname, m_szCurrentLevel ) )
	{
		if ( !m_pLastWorldModel )
		{
			m_pLastWorldModel = host_state.worldmodel;
		}

		Assert( !host_state.worldmodel || ( m_pLastWorldModel == host_state.worldmodel ) );
		return;
	}

	if ( !g_pFullFileSystem )
	{
		return;
	}

	m_pLastWorldModel = host_state.worldmodel;

	KillCache();

	Assert( !m_pCache );

	g_pFullFileSystem->CreateDirHierarchy( AUDIOSOURCE_CACHE_ROOTDIR, "MOD" );

	Q_snprintf( m_szMapCache, sizeof( m_szMapCache ), "%s/%s%s.cache", AUDIOSOURCE_CACHE_ROOTDIR, mapname, m_szCurrentLanguage );
	Q_snprintf( m_szMapCacheBase, sizeof( m_szMapCacheBase ), "%s/%s", AUDIOSOURCE_CACHE_ROOTDIR, mapname );

	if ( m_bSndCacheDebug )
	{
		DevMsg(	1, "Map Cache     :  '%s'\n", m_szMapCache );
		DevMsg(	1, "Map Cache Base:  '%s'\n", m_szMapCacheBase );
	}

#if defined( _DEBUG )
	ConColorMsg( Color( 0,100,255,255), "CAudioSourceCache: LevelInit:  %s\n", m_szMapCache );
#endif

	CacheType_t *newCache = AllocAudioCache( m_szMapCache );
	Assert( newCache );
	if ( !newCache->Init() )
	{
		Warning( "Failed to init sound cache '%s'\n", m_szMapCache );
		delete newCache;
		newCache = NULL;
	}

	if ( !newCache )
	{
		return;
	}

	Q_strncpy( m_szCurrentLevel, mapname, sizeof( m_szCurrentLevel ) );

	SetCachePointer( CACHE_MAP, newCache );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceCache::LevelShutdown()
{
	if ( IsX360() )
	{
		// 360 not using
		return;
	}

	if ( !m_pCache )
	{
		return;
	}

	// Get precached sound count and store manifest if running with -makereslists
	if ( !CommandLine()->FindParm( "-makereslists" ) )
	{
		return;
	}

	int count = g_pSoundServices->GetPrecachedSoundCount();

	if ( !count )
	{
		return;
	}

	// So that we only save this out once per level
	if ( g_pSoundServices->GetServerCount() == m_nServerCount )
	{
		return;
	}

	m_nServerCount = g_pSoundServices->GetServerCount();

	WriteManifest();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CAudioSourceCache::WriteManifest()
{
	if ( IsX360() )
	{
		// 360 not using
		return;
	}

	if ( !m_pCache )
	{
		DevMsg( "Skipping WriteManifest, must be running map locally\n" );
		return;
	}

	int count = g_pSoundServices->GetPrecachedSoundCount();

	if ( !count )
	{
		DevMsg( "Skipping WriteManifest, no entries in sound precache string table\n" );
		return;
	}

	// Save manifest out to disk...
	CUtlBuffer buf( 0, 0, CUtlBuffer::TEXT_BUFFER );

	for ( int i = 0; i < count; ++i )
	{
		char const *fn = g_pSoundServices->GetPrecachedSound( i );
		if ( fn && fn[ 0 ] )
		{
			char full[ 512 ];
			Q_snprintf( full, sizeof( full ), "sound\\%s", PSkipSoundChars( fn ) );
			Q_strlower( full );
			Q_FixSlashes( full );

			// Write to file
			buf.Printf( "\"%s\"\r\n", full );
		}
	}

	g_pFullFileSystem->CreateDirHierarchy( AUDIOSOURCE_CACHE_ROOTDIR, "MOD" );

	char manifest_name[ 512 ];
	Q_snprintf( manifest_name, sizeof( manifest_name ), "%s.manifest", m_szMapCacheBase );

	if ( g_pFullFileSystem->FileExists( manifest_name, "MOD" ) && 
		!g_pFullFileSystem->IsFileWritable( manifest_name, "MOD" ) )
	{
		g_pFullFileSystem->SetFileWritable( manifest_name, true, "MOD" );
	}

	// Now write to file
	FileHandle_t fh;
	fh = g_pFullFileSystem->Open( manifest_name, "wb" );
	if ( FILESYSTEM_INVALID_HANDLE != fh )
	{
		g_pFullFileSystem->Write( buf.Base(), buf.TellPut(), fh );
		g_pFullFileSystem->Close( fh );

		DevMsg( "WriteManifest:  Persisting cache manifest '%s' (%d entries)\n", manifest_name, count );
	}
	else
	{
		Warning( "WriteManifest:  Unable to persist cache manifest '%s', check file permissions\n", manifest_name );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CAudioSourceCache::CacheType_t *CAudioSourceCache::LookUpCacheEntry( const char *fn, int audiosourcetype, bool soundisprecached, CSfxTable *sfx )
{
	if ( IsX360() )
	{
		return NULL;
	}

	// Hack to remember the type of audiosource to create if we need to recreate it
	CAudioSourceCachedInfo::s_CurrentType = audiosourcetype;
	CAudioSourceCachedInfo::s_pSfx = sfx;
	CAudioSourceCachedInfo::s_bIsPrecacheSound = soundisprecached || m_bBuildingFullDataCache;

	CacheType_t *pCache = NULL;

	// If building a cache, just shortcut thourgh to target cache
	if ( m_pBuildingCache )
	{
		pCache = m_pBuildingCache;
	}
	// Always check shared/common sounds first, since it has startup data included
	else 
	{
		if ( m_pSharedPrecacheCache && m_pSharedPrecacheCache->EntryExists( fn ) )
		{
			pCache = m_pSharedPrecacheCache;
		}
		else
		{
			if ( soundisprecached )
			{
				if ( !m_pLastWorldModel )
				{
					m_pLastWorldModel = host_state.worldmodel;
				}
				Assert( m_pLastWorldModel == host_state.worldmodel );

				// No level loaoded, return NULL
				if ( !m_pCache )
				{
					return NULL;
				}

				// Check the level specific precache list
				pCache = m_pCache;
			}
			else
			{
				// Grab from the full game list
				Assert( m_pOtherSoundCache );
				pCache = m_pOtherSoundCache;
			}
		}
	}

	return pCache;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CAudioSourceCachedInfo *CAudioSourceCache::GetInfo( int audiosourcetype, bool soundisprecached, CSfxTable *sfx )
{
	VPROF("CAudioSourceCache::GetInfo");

	if ( IsX360() )
	{
		// 360 not using
		return NULL;
	}

	Assert( sfx );

	char fn[ 512 ];
	Q_snprintf( fn, sizeof( fn ), "sound/%s", sfx->GetFileName() );

	CAudioSourceCachedInfo *info = NULL;
	CacheType_t *pCache = LookUpCacheEntry( fn, audiosourcetype, soundisprecached, sfx );
	if ( !pCache )
		return NULL;

	info = pCache->Get( fn );

	if ( info && info->Format() == 0 )
	{
		if ( g_pFullFileSystem->FileExists( fn, "BSP" ) )
		{
			DevMsg( 1, "Forced rebuild of bsp cache sound '%s'\n", fn );
			info = pCache->RebuildItem( fn );
			Assert( info->Format() != 0 );
		}
	}

	return info;
}


void CAudioSourceCache::RebuildCacheEntry( int audiosourcetype, bool soundisprecached, CSfxTable *sfx )
{
	VPROF("CAudioSourceCache::GetInfo");

	if ( IsX360() )
	{
		// 360 not using
		return;
	}

	Assert( sfx );

	char fn[ 512 ];
	Q_snprintf( fn, sizeof( fn ), "sound/%s", sfx->GetFileName() );
	CacheType_t *pCache = LookUpCacheEntry( fn, audiosourcetype, soundisprecached, sfx );
	if ( !pCache )
		return;

	pCache->RebuildItem( fn );
}


void CAudioSourceCache::ForceRecheckDiskInfo()
{
	if ( m_pCache )
		m_pCache->ForceRecheckDiskInfo();

	if ( m_pOtherSoundCache )
		m_pOtherSoundCache->ForceRecheckDiskInfo();
	
	if ( m_pSharedPrecacheCache )
		m_pSharedPrecacheCache->ForceRecheckDiskInfo();

	if ( m_pBuildingCache )
		m_pBuildingCache->ForceRecheckDiskInfo();
}


//-----------------------------------------------------------------------------
// Purpose: Analyzes all sounds referenced in reslists and determines which ones are "shared", i.e., used in a bunch of maps, and
// Purpose: 
// Input  : FileNameHandle_t - 
//			other - 
//			FileNameHandle_t - 
//			sharedprecache - 
//-----------------------------------------------------------------------------
void CAudioSourceCache::AnalyzeReslists( CUtlRBTree< FileNameHandle_t, int >& other, CUtlRBTree< FileNameHandle_t, int >& sharedprecache )
{
	int i, j;

	if ( IsX360() )
	{
		return;
	}

	other.RemoveAll();
	sharedprecache.RemoveAll();

	CUtlRBTree< FileNameHandle_t, int > manifests( 0, 0, DefLessFunc( FileNameHandle_t ) );

	g_pSoundServices->GetAllManifestFiles( manifests );

	int c = manifests.Count();
	if ( !c )
	{
		return;
	}

	CUtlRBTree< AudioSourceUsage_t, int >	usage( 0, 0, AudioSourceUsageLessFunc );

	// Now walk through each manifest and try to build sounds
	for ( i = manifests.FirstInorder(); i != manifests.InvalidIndex(); i = manifests.NextInorder( i ) )
	{
		char manifest_file[ 512 ];
		if ( g_pFullFileSystem->String( manifests[ i ], manifest_file, sizeof( manifest_file ) ) )
		{
			CUtlRBTree< FileNameHandle_t, int > filenames( 0, 0, DefLessFunc( FileNameHandle_t ) );
			g_pSoundServices->GetAllSoundFilesInManifest( filenames, manifest_file );
	
			for ( j = filenames.FirstInorder() ; j != filenames.InvalidIndex() ; j = filenames.NextInorder( j ) )
			{
				FileNameHandle_t& h = filenames[ j ];

				AudioSourceUsage_t u;
				u.handle = h;

				// Add it if it's the first one
				int idx = usage.Find( u );
				if ( idx == usage.InvalidIndex() )
				{
					idx = usage.Insert( u );
				}

				// Increment count
				++usage[ idx ].count;
			}
		}
	}

	// Now figure out which .wavs are referenced by multiple .bsps
	unsigned int threshold = (unsigned int)( SOUND_PRECACHESHARED_THRESHOLD * manifests.Count() );

	for ( i = usage.FirstInorder(); i != usage.InvalidIndex() ; i = usage.NextInorder( i ) )
	{
		char soundfile[ 512 ];
		if ( g_pFullFileSystem->String( usage[ i ].handle, soundfile, sizeof( soundfile ) ) )
		{
			//  Msg( "%02i : %s\n", usage[ i ].count, soundfile );
			unsigned int ucount = usage[ i ].count;

			if ( ucount >= threshold )
			{
				sharedprecache.Insert( usage[ i ].handle );
			}
			else
			{
				other.Insert( usage[ i ].handle );
			}
		}
	}
	
	// Now actually get the list of all sounds used by the game during the actual reslists run and make sure those are in "other", too
	CUtlRBTree< FileNameHandle_t, int > soundfiles( 0, 0, DefLessFunc( FileNameHandle_t ) );

	g_pSoundServices->GetAllSoundFilesReferencedInReslists( soundfiles );

	// Now walk through these and see if they are in either list
	for ( i = soundfiles.FirstInorder(); i != soundfiles.InvalidIndex() ; i = soundfiles.NextInorder( i ) )
	{
		FileNameHandle_t& handle = soundfiles[ i ];

		// It's in this list
		if ( sharedprecache.Find( handle ) != sharedprecache.InvalidIndex() )
		{
			continue;
		}

		if ( other.Find( handle ) != other.InvalidIndex() )
		{
			continue;
		}

		// Otherwise, it goes in the "other" list
		other.Insert( handle );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *cachename - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::IsValidCache( char const *cachename )
{
	if ( IsX360() )
	{
		return false;
	}

	CacheType_t *cache = AllocAudioCache( cachename );

	// This will delete any outdated .cache files
	bool valid = cache->IsUpToDate();

	delete cache;

	return valid;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *cachename - 
//-----------------------------------------------------------------------------
void CAudioSourceCache::RemoveCache( char const *cachename )
{
	if ( IsX360() )
	{
		return;
	}

	if ( g_pFullFileSystem->FileExists( cachename, "MOD" ) )
	{
		if ( !g_pFullFileSystem->IsFileWritable( cachename, "MOD" ) )
		{
			g_pFullFileSystem->SetFileWritable( cachename, true, "MOD" );
		}
		g_pFullFileSystem->RemoveFile( cachename, "MOD" );
	}
}

void CAudioSourceCache::SetCachePointer( SoundCacheType_t ptrType, CacheType_t *ptr )
{
	if ( IsX360() )
	{
		return;
	}

	bool dirty = false;

	switch ( ptrType )
	{
	default:
		Error( "SetCachePointer with bogus type %i\n", (int)ptrType );
		break;
	case CACHE_MAP:
		if ( m_pCache != ptr )
		{
			dirty = true;
			m_pCache = ptr;
		}
		break;
	case CACHE_SHARED:
		if ( m_pSharedPrecacheCache != ptr )
		{
			dirty = true;
			m_pSharedPrecacheCache = ptr;
		}
		break;
	case CACHE_OTHER:
		if ( m_pOtherSoundCache != ptr )
		{
			dirty = true;
			m_pOtherSoundCache = ptr;
		}
		break;
	case CACHE_BUILDING:
		if ( m_pBuildingCache != ptr )
		{
			dirty = true;
			m_pBuildingCache = ptr;
		}
		break;
	}

	if ( dirty )
	{
		CAudioSourceCachedInfoHandle_t::InvalidateCache();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::HasMissingCaches()
{
	if ( IsX360() )
	{
		return false;
	}

	CUtlRBTree< FileNameHandle_t, int > manifests( 0, 0, DefLessFunc( FileNameHandle_t ) );

	g_pSoundServices->GetAllManifestFiles( manifests );

	int c = manifests.Count();
	if ( !c )
	{
		return false;
	}

	// Now walk through each manifest and try to build sounds
	for ( int i = manifests.FirstInorder(); i != manifests.InvalidIndex(); i = manifests.NextInorder( i ) )
	{
		char manifest_file[ 512 ];
		if ( g_pFullFileSystem->String( manifests[ i ], manifest_file, sizeof( manifest_file ) ) )
		{
			char mapname[ 512 ];
			Q_StripExtension( manifest_file, mapname, sizeof( mapname ) );

			// See if there is a valid cache file for this manifest
			char cachename[ 512 ];
			Q_snprintf( cachename, sizeof( cachename ), "%s%s.cache", mapname, m_szCurrentLanguage ); 

			if ( !IsValidCache( cachename ) )
				return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : showprogress - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::FastBuildSharedPrecachedSoundsCache( bool rebuild, bool showprogress, bool bForceBuild )
{
	if ( IsX360() )
	{
		// 360 not using
		return true;
	}

	if ( !m_bFirstTime && !bForceBuild )
	{
		return true;
	}
	
	bool needsRebuildWork = false;

	char fn[ 512 ];
	Q_snprintf( fn, sizeof( fn ), "%s/%s%s.cache", AUDIOSOURCE_CACHE_ROOTDIR, OTHER_REBUILD_CACHE_NAME, m_szCurrentLanguage );

	if ( m_bSndCacheDebug )
	{
		DevMsg(	1, "Fast Build Temp Cache:  '%s'\n", fn );
	}

	if ( rebuild )
	{
		// Blow away the metacaches if rebuilding, which will force a full cache build
		RemoveCache( fn );
		RemoveCache( m_szSharedPrecacheCache );
		RemoveCache( m_szOtherSoundsCache );

		needsRebuildWork = true;

	}
	else
	{
		// This will do a fast check and delete the cache if it's invalid (old format)
		if ( !IsValidCache( fn ) )
		{
			needsRebuildWork = true;
		}
		if ( !IsValidCache( m_szSharedPrecacheCache ) )
		{
			needsRebuildWork = true;
		}
		if ( !IsValidCache( m_szOtherSoundsCache ) )
		{
			needsRebuildWork = true;
		}
	}

	bool needsLevelWork = HasMissingCaches();

	if ( !needsRebuildWork && !needsLevelWork )
	{
		return true;
	}

	if ( showprogress )
	{
		g_pSoundServices->CacheBuildingStart();	
	}

	CacheType_t *pOtherFullData		= NULL;
	CacheType_t *pSharedFullData	= NULL;
	CacheType_t *pOtherNoData		= NULL;

	if ( needsRebuildWork )
	{
		CUtlRBTree< FileNameHandle_t, int > other( 0, 0, DefLessFunc( FileNameHandle_t ) );
		CUtlRBTree< FileNameHandle_t, int > sharedprecache( 0, 0, DefLessFunc( FileNameHandle_t ) );
		
		AnalyzeReslists( other, sharedprecache );

		pOtherFullData		= BuildCacheFromList( fn, other, true, showprogress, 0.0f, 0.33f );
		pSharedFullData		= BuildCacheFromList( m_szSharedPrecacheCache, sharedprecache, true, showprogress, 0.33f, 0.75f );
		pOtherNoData		= BuildNoDataCacheFromFullDataCache( m_szOtherSoundsCache, pOtherFullData, showprogress, 0.75f, 0.90f );

		if ( pSharedFullData )
		{
			if ( m_pSharedPrecacheCache )
			{
				// Don't shutdown/save, since we have a new one already
				delete m_pSharedPrecacheCache;
			}

			// Take over ptr
			SetCachePointer( CACHE_SHARED, pSharedFullData );
		}

		if ( pOtherNoData )
		{
			if ( m_pOtherSoundCache )
			{
				// Don't shutdown/save, since we have a new one already
				delete m_pOtherSoundCache;
			}

			// Take over ptr
			SetCachePointer( CACHE_OTHER, pOtherNoData );
		}
	}
	else
	{
		// Load the full data cache from disk
		pOtherFullData = AllocAudioCache( fn );
		Assert( pOtherFullData );
		pOtherFullData->Init();

		pSharedFullData = m_pSharedPrecacheCache;
	}

	if ( pOtherFullData && pSharedFullData )
	{
		FastBuildAllMissingSoundCaches( pOtherFullData, pSharedFullData, showprogress, rebuild, 0.8f, 1.0f );
	}

	// Always discard this one
	if ( pOtherFullData )
	{
		pOtherFullData->Shutdown();
		delete pOtherFullData;
	}

	if ( showprogress )
	{
		g_pSoundServices->CacheBuildingFinish();	
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//			*pSharedPrecache - 
//			showprogress - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::FastBuildAllMissingSoundCaches( CacheType_t *pOther, CacheType_t *pSharedPrecache, bool showprogress /*=false*/, bool forcerebuild /*=false*/, float flProgressStart /*= 0.0f*/, float flProgressEnd /*= 1.0f*/ )
{
	if ( IsX360() )
	{
		// 360 not using
		return true;
	}

	if ( !m_bFirstTime )
	{
		return true;
	}

	CUtlRBTree< FileNameHandle_t, int > manifests( 0, 0, DefLessFunc( FileNameHandle_t ) );

	g_pSoundServices->GetAllManifestFiles( manifests );

	int c = manifests.Count();
	if ( !c )
	{
		return false;
	}

	int i;

	CUtlVector< FileNameHandle_t >	worklist;

	// Now walk through each manifest and try to build sounds
	for ( i = manifests.FirstInorder(); i != manifests.InvalidIndex(); i = manifests.NextInorder( i ) )
	{
		char manifest_file[ 512 ];
		if ( g_pFullFileSystem->String( manifests[ i ], manifest_file, sizeof( manifest_file ) ) )
		{
			char mapname[ 512 ];
			Q_StripExtension( manifest_file, mapname, sizeof( mapname ) );

			//  Cache is same filename, with .cache extension instead.
			char cachename[ 512 ];
			Q_snprintf( cachename, sizeof( cachename ), "%s%s.cache", mapname, m_szCurrentLanguage );

			CacheType_t *cache = AllocAudioCache( cachename );
			// This will delete any outdated .cache files
			if ( cache->IsUpToDate() && !forcerebuild )
			{
				delete cache;
				continue;
			}
			if ( forcerebuild )
			{
				RemoveCache( cachename ); // force it to rebuild the cache
			}

			worklist.AddToTail( manifests[ i ] );
		}
	}

	// Nothing to do
	if ( worklist.Count() == 0 )
	{
		return true;
	}

	double st = Plat_FloatTime();

	int num = 0;

	// Now walk through each manifest and try to build sounds
	for ( i = 0; i < worklist.Count(); ++i )
	{
		char manifest_file[ 512 ];
		if ( g_pFullFileSystem->String( worklist[ i ], manifest_file, sizeof( manifest_file ) ) )
		{
			char mapname[ 512 ];
			Q_StripExtension( manifest_file, mapname, sizeof( mapname ) );

			//  Cache is same filename, with .cache extension instead.
			char cachename[ 512 ];
			Q_snprintf( cachename, sizeof( cachename ), "%s%s.cache", mapname, m_szCurrentLanguage );

			++num;

			CacheType_t *cache = AllocAudioCache( cachename );
			if ( !cache )
			{
				Error( "Unable to create '%s'!", cachename );
			}

			if ( !cache->Init() )
			{
				Warning( "Failed to init sound cache '%s'\n", cachename );
				delete cache;
				cache = NULL;
				continue;
			}

			float frac = (float)(num - 1 ) / (float)worklist.Count();

			if ( showprogress )
			{
				char base[ 256 ];
				Q_FileBase( cachename, base, sizeof( base ) );
				Q_strlower( base );
				g_pSoundServices->CacheBuildingUpdateProgress( flProgressStart + frac * ( flProgressEnd - flProgressStart ), base );
			}

			FastBuildSoundCache( frac, cachename, pOther, pSharedPrecache, cache, manifest_file );

			cache->Shutdown();
			delete cache;
		}
	}

	double ed = Plat_FloatTime();
	DevMsg( "Rebuild took %.3f seconds\n", ( float )( ed - st ) );

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Given a manfest and fulldat versions of the other and shared caches, rebuild the level specific data cache
// Input  : progressfrac - 
//			*fullCache - 
//			*cache - 
//			fullCache - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CAudioSourceCache::FastBuildSoundCache( 
	float progressfrac, 
	char const *currentcache, 
	CacheType_t *fullCache, 
	CacheType_t *sharedPrecacheCache,
	CacheType_t *cache, 
	char const *manifest )
{
	if ( IsX360() )
	{
		// 360 not using
		return true;
	}

	Assert( currentcache );
	Assert( fullCache );
	Assert( cache );
	Assert( manifest );

	CUtlRBTree< FileNameHandle_t, int > list( 0, 0, DefLessFunc( FileNameHandle_t ) );
	
	g_pSoundServices->GetAllSoundFilesInManifest( list, manifest );
	
	DevMsg( "%.2f %% -> %s\n", 100.0f * progressfrac, currentcache );

	bool needsave = false;
	for ( int i = list.FirstInorder() ; i != list.InvalidIndex() ; i = list.NextInorder( i ) )
	{
		FileNameHandle_t& h = list[ i ];
		char fn[ 512 ];
		if ( g_pFullFileSystem->String( h, fn, sizeof( fn ) ) )
		{
			// If entry is in the sharedprecache cache, don't add to the per-level cache
			if ( sharedPrecacheCache->EntryExists( fn ) )
			{
				continue;
			}

			// Otherwise, it should be in the full cache
			if ( fullCache->EntryExists( fn ) )
			{
				needsave = true;

				CAudioSourceCachedInfo* entry = fullCache->Get( fn );

				long info = fullCache->GetFileInfo( fn );

				cache->SetElement( fn, info, entry );
			}
		}
	}
	
	if ( needsave )
	{
		cache->Save();
	}

	return true;
}

CAudioSourceCache::CacheType_t *CAudioSourceCache::BuildNoDataCacheFromFullDataCache( char const *cachename, CacheType_t *fullCache, bool showprogress /*= false*/, float flProgressStart /*= 0.0f*/, float flProgressEnd /*= 1.0f*/ )
{
	if ( IsX360() )
	{
		// 360 not using
		return NULL;
	}

	CacheType_t *newCache = NULL;
	
	newCache = AllocAudioCache( cachename );
	Assert( newCache );
	if ( newCache->Init() )
	{
		int visited = 0;
		int c = fullCache->Count();

		for ( int i =0; i < c; ++i )
		{
			char fn[ 512 ];
			
			fullCache->GetElementName( i, fn, sizeof( fn ) );

			CAudioSourceCachedInfo* entry = (*fullCache)[ i ];

			long fileinfo = fullCache->GetFileInfo( fn );

			CAudioSourceCachedInfo *entryCopy = new CAudioSourceCachedInfo( *entry );

			entryCopy->RemoveData();

			newCache->SetElement( fn, fileinfo, entryCopy );

			++visited;

			if ( !( visited % 100 ) )
			{
				Msg( "  progress %i/%i (%i %%)\n",
					visited, c, (int)( 100.0f * ( float) visited / (float) c ) );
			}

			if ( showprogress )
			{
				float frac = ( float )( visited - 1 )/( float )c;
				
				frac = flProgressStart + frac * ( flProgressEnd - flProgressStart );

				char base[ 256 ];
				Q_FileBase( fn, base, sizeof( base ) );
				Q_strlower( base );
				g_pSoundServices->CacheBuildingUpdateProgress( frac, base );
			}
		}

		Msg( "Touched %i cached files\n", c);

		// Persist data to HD if dirty
		newCache->Save();
	}
	else
	{
		delete newCache;
		newCache = NULL;
	}

	return newCache;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *cachename - 
//			FileNameHandle_t - 
//			soundlist - 
//			fulldata - 
//			showprogress - 
// Output : CAudioSourceCache::CacheType_t
//-----------------------------------------------------------------------------
CAudioSourceCache::CacheType_t *CAudioSourceCache::BuildCacheFromList( char const *cachename, CUtlRBTree< FileNameHandle_t, int >& soundlist, bool fulldata, bool showprogress /*= false*/, float flProgressStart /*= 0.0f*/, float flProgressEnd /*= 1.0f*/ )
{
	if ( IsX360() )
	{
		// 360 not using
		return NULL;
	}

	CacheType_t *newCache = NULL;

	newCache = AllocAudioCache( cachename );
	Assert( newCache );
	if ( newCache->Init() )
	{
		SetCachePointer( CACHE_BUILDING, newCache );

		m_bBuildingFullDataCache = fulldata;

		int visited = 0;

		for ( int i = soundlist.FirstInorder(); i != soundlist.InvalidIndex(); i = soundlist.NextInorder( i ) )
		{
			FileNameHandle_t& handle = soundlist[ i ];
			char soundname[ 512 ];
			soundname[ 0 ] = 0;
			if ( g_pFullFileSystem->String( handle, soundname, sizeof( soundname ) ) )
			{
				// Touch the cache
				// Force it to go into the "other" cache but to also appear as "full data" precache
				CSfxTable *pTable = S_PrecacheSound( &soundname[ SOUND_DIRECTORY_LENGTH ] );
				// This will "re-cache" this if it's not in this level's cache already
				if ( pTable && pTable->pSource )
				{
					GetInfo( pTable->pSource->GetType(), fulldata, pTable );
				}
			}
			else
			{
				Assert( !"Unable to find FileNameHandle_t in fileystem list." );
			}

			++visited;

			if ( !( visited % 100 ) )
			{
				Msg( "  progress %i/%i (%i %%)\n",
					visited, soundlist.Count(), (int)( 100.0f * ( float) visited / (float) soundlist.Count() ) );
			}

			if ( showprogress )
			{
				float frac = ( float )( visited - 1 )/( float )soundlist.Count();

				frac = flProgressStart + frac * ( flProgressEnd - flProgressStart );

				char base[ 256 ];
				Q_FileBase( soundname, base, sizeof( base ) );
				Q_strlower( base );
				g_pSoundServices->CacheBuildingUpdateProgress( frac, base );
			}
		}

		Msg( "Touched %i cached files\n", soundlist.Count() );

		m_bBuildingFullDataCache = false;
		SetCachePointer( CACHE_BUILDING, NULL );

		// Persist data to HD if dirty
		newCache->Save();
	}
	else
	{
		delete newCache;
		newCache = NULL;
	}

	return newCache;
}

//-----------------------------------------------------------------------------
// Purpose: Shutdown the level specific cache
//-----------------------------------------------------------------------------
void CAudioSourceCache::KillCache()
{
	if ( m_pCache )
	{
#if defined( _DEBUG )
		ConColorMsg( Color( 0,100,255,255), "Audio LevelShutdown:  %s\n", m_szMapCache );
#endif
		m_pCache->Shutdown();
		delete m_pCache;
	}
	SetCachePointer( CACHE_MAP, NULL );
}

char const *CAudioSourceCache::GetAudioCacheLanguageSuffix( char *buf, size_t bufsize )
{
	char const *pchLanguage = g_pSoundServices->GetUILanguage();

	if ( !pchLanguage || !*pchLanguage || !Q_stricmp( pchLanguage, "english" ) )
	{
		buf[ 0 ] = 0;
		return buf;
	}
	
	//Check language right here to see if we need the caches for it.
	char szLanguageList[ MAX_REPOSITORY_NAME ];
	Q_snprintf( szLanguageList, sizeof( szLanguageList ), "%s/localization_cache_list.txt", AUDIOSOURCE_CACHE_ROOTDIR );	
	FileHandle_t fh = g_pFullFileSystem->Open( szLanguageList, "r" );
	char szCacheLanguage[ MAX_LIST_SIZE ];
	if ( fh )
	{
		g_pFullFileSystem->Read( szCacheLanguage, MAX_LIST_SIZE, fh);
		g_pFullFileSystem->Close( fh );

		if ( Q_stristr( szCacheLanguage, pchLanguage ) )
		{
			Q_snprintf( buf, bufsize, "_%s", pchLanguage );
			return buf;
		}
		else
		{
			buf[ 0 ] = 0;
			return buf;
		}
	}	

	Q_snprintf( buf, bufsize, "_%s", pchLanguage );
	return buf;
}

void CheckCacheBuild()
{
	g_ASCache.CheckCacheBuild();
}

void FastBuildSharedPrecachedSoundsCache( bool bForceBuild )
{
	g_ASCache.FastBuildSharedPrecachedSoundsCache( true, true, bForceBuild );
}

CON_COMMAND( snd_rebuildaudiocache, "Rebuilds all audio caches (_other, _other_rebuild, _sharedprecache, level caches) from reslists\n" )
{
	FastBuildSharedPrecachedSoundsCache( true );
}

CON_COMMAND( snd_writemanifest, "If running a game, outputs the precache manifest for the current level\n" )
{
	g_ASCache.WriteManifest();
}
