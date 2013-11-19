//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Purpose: particle system code
//
//===========================================================================//

#include "tier0/platform.h"
#include "tier0/vprof.h"
#include "particles/particles.h"
#include "psheet.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#define SORT_BUCKET_SIZE 512						   // how many buckets for each radix sort pass
#define BUCKET_BLOCK_SIZE 64								// chunk size for bucket allocation
#define BUCKET_BLOCK_SHIFT 6

static SheetSequenceSample_t s_DefaultSheetSequence = 
{
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	1.0f,
};


struct SortBucket_t
{
	ParticleRenderData_t *m_pBucketData[( MAX_PARTICLES_IN_A_SYSTEM + BUCKET_BLOCK_SIZE - 1 ) / BUCKET_BLOCK_SIZE];
};

static SortBucket_t s_SortBuckets[SORT_BUCKET_SIZE];

// the pool for the block allocator
#define NUM_BUCKETS_NEEDED_IN_POOL ( SORT_BUCKET_SIZE+( (MAX_PARTICLES_IN_A_SYSTEM+BUCKET_BLOCK_SIZE))/BUCKET_BLOCK_SIZE)

static ParticleRenderData_t s_BlockPool[BUCKET_BLOCK_SIZE*NUM_BUCKETS_NEEDED_IN_POOL];
static ParticleRenderData_t s_SortedIndexList[MAX_PARTICLES_IN_A_SYSTEM];
static int s_nNumParticlesToGoInBucketInPassTwo[SORT_BUCKET_SIZE];
static int s_nNumParticlesInBucket[SORT_BUCKET_SIZE];
static ParticleRenderData_t *s_AvailableBlockPtr;



void CParticleCollection::GenerateFirstSortPassData( Vector CameraPos )
{
	size_t nXYZ_stride;
	const FourVectors *pXYZ = Get4VAttributePtr( PARTICLE_ATTRIBUTE_XYZ, &nXYZ_stride );

	size_t ct_stride;
	const fltx4 *pCreationTimeStamp = GetM128AttributePtr( PARTICLE_ATTRIBUTE_CREATION_TIME, &ct_stride );

// 	size_t seq_stride;
// 	const fltx4 *pSequenceNumber = GetM128AttributePtr( PARTICLE_ATTRIBUTE_SEQUENCE_NUMBER, &seq_stride );


// 	size_t ld_stride;
// 	const fltx4 *pLifeDuration = GetM128AttributePtr( PARTICLE_ATTRIBUTE_LIFE_DURATION, &ld_stride );

	// we'll do as much in see as we can
	int nParticles = m_nActiveParticles;

	FourVectors EyePos;
	EyePos.DuplicateVector( CameraPos );

	fltx4 CurTime = ReplicateX4( m_flCurTime );

	float flAgeScale = 1.0 * SEQUENCE_SAMPLE_COUNT;

	// fixme
	fltx4 AgeScale = ReplicateX4( flAgeScale );

	fltx4 MagicF2I = ReplicateX4( 1l<<23 );

	int nOutIdx = 0;										// fixme for children
	while( nParticles > 0 )
	{
		fltx4 Xdiff = SubSIMD( EyePos.x, pXYZ->x );
		fltx4 Ydiff = SubSIMD( EyePos.y, pXYZ->y );
		fltx4 Zdiff = SubSIMD( EyePos.z, pXYZ->z );
		// the 2.0 is added to the squared distance to prevent negative exponents so I can ignore exponent high bits
		// in my ieee hackery.
		fltx4 Distance_squared = AddSIMD( Four_Twos, 
											  AddSIMD( MulSIMD( Xdiff, Xdiff ),
														  AddSIMD( MulSIMD( Ydiff, Ydiff ),
																	  MulSIMD( Zdiff, Zdiff ) ) ) );
		// now, do animation calculation
		fltx4 ParticleAge = SubSIMD( CurTime, *pCreationTimeStamp );
		fltx4 AnimFrame = MulSIMD( ParticleAge, AgeScale );
		// clamp here if need be

		// magically convert to an int
		AnimFrame = AddSIMD( AnimFrame, MagicF2I );

		for(int nSub = 0; nSub < min( 4, nParticles ); nSub++)
		{
			uint32 nDistanceKey = SubInt( Distance_squared, nSub );
			// now, do some bit-twiddling to get it the way we like the addition of 2.0 to the
			// distance^2 allows us to remove the exponent bias because now we will have no
			// negative exponents
			nDistanceKey -= (127<<23);
// 			int exponent=nDistanceKey>>23;
// 			int mantissa=nDistanceKey & ((1<<23)-1);
			nDistanceKey >>= 12;							// move most exponent bits down
			nDistanceKey = min(nDistanceKey, 0x3ffff);

			int nBucket = nDistanceKey & ( SORT_BUCKET_SIZE-1 );
			int nBucketBlockIndex = ( s_nNumParticlesInBucket[ nBucket ] >> BUCKET_BLOCK_SHIFT );
			if ( ( s_nNumParticlesInBucket[ nBucket ] & (BUCKET_BLOCK_SIZE-1) ) == 0 )
			{
				// need to give this bucket a new block
				s_SortBuckets[ nBucket ].m_pBucketData[ nBucketBlockIndex] = s_AvailableBlockPtr;
				s_AvailableBlockPtr += BUCKET_BLOCK_SIZE;
			}
			ParticleRenderData_t *new_record =
				s_SortBuckets[nBucket].m_pBucketData[nBucketBlockIndex] +
				( s_nNumParticlesInBucket[nBucket] & ( BUCKET_BLOCK_SIZE-1 ) );
			new_record->m_nIndex = nOutIdx;
			nOutIdx++;
			new_record->m_nPass2SortKey = nDistanceKey >> 9;
			s_nNumParticlesToGoInBucketInPassTwo[ new_record->m_nPass2SortKey ]++; // update histogram for pass 2
#if DEBUG_PARTICLE_SORT
			new_record->m_nSortKey = nDistanceKey;
			new_record->m_flDistance = Distance_squared.m128_f32[nSub];
#endif
			new_record->m_pSample = &s_DefaultSheetSequence;	// fixme
			s_nNumParticlesInBucket[ nBucket ]++;
		}
		nParticles-=4;
		pXYZ += nXYZ_stride;
	}
	// do children here
}

void CParticleCollection::GenerateSortedIndexList( Vector vecCamera )
{
	VPROF_BUDGET( "CParticleCollection::GenerateSortedIndexList", VPROF_BUDGETGROUP_PARTICLE_RENDERING );
	memset( s_nNumParticlesInBucket, 0, sizeof( s_nNumParticlesInBucket ) );
	memset( s_nNumParticlesToGoInBucketInPassTwo, 0, sizeof( s_nNumParticlesToGoInBucketInPassTwo ) );
	
	// perform distance caluclation, first pass radix sort, and histogram calculation for 2nd pass.

	s_AvailableBlockPtr = s_BlockPool;
	GenerateFirstSortPassData( vecCamera );

	// now, we will perform the last radix sort pass, serializing the buckets in groups based upon
	// high bits
	
	// first, init the output ptrs.
	ParticleRenderData_t *pOutputPtrs[SORT_BUCKET_SIZE];
	ParticleRenderData_t *pData = s_SortedIndexList;
	int nTotalParticles=0;
	for(int i=0 ; i < SORT_BUCKET_SIZE; i++)
	{
		pOutputPtrs[i] = pData;
		pData += s_nNumParticlesToGoInBucketInPassTwo[i];
		nTotalParticles += s_nNumParticlesToGoInBucketInPassTwo[i];
	}
	// now, serialize the buckets
	for(int i=0 ; i < SORT_BUCKET_SIZE; i++)
	{
		int nCount = s_nNumParticlesInBucket[i];
		ParticleRenderData_t **pInputBlockTbl=s_SortBuckets[i].m_pBucketData;
		while( nCount )
		{
			ParticleRenderData_t *pInputBlock = *(pInputBlockTbl++);
			int nNumInBlock = min( nCount, BUCKET_BLOCK_SIZE );
			nCount -= nNumInBlock;
			do
			{
				Assert( pInputBlock->m_nPass2SortKey < SORT_BUCKET_SIZE );
				*(pOutputPtrs[pInputBlock->m_nPass2SortKey]++) = *pInputBlock;
				pInputBlock++;
			} while( --nNumInBlock );
		}
	}
	// magically, we should now be done. lets check in case the code doesn't work the first time
#if DEBUG_PARTICLE_SORT
	for(int i=0 ; i < nTotalParticles-1; i++)
	{
		Assert( s_SortedIndexList[i].m_nSortKey <= s_SortedIndexList[i+1].m_nSortKey );
// 		if ( fabs( s_SortedIndexList[i].m_flDistance - s_SortedIndexList[i+1].m_flDistance )>10)
// 		{
// 			Assert( s_SortedIndexList[i].m_flDistance <= s_SortedIndexList[i+1].m_flDistance );
// 		}
	}
#endif

}


const ParticleRenderData_t *CParticleCollection::GetRenderList( IMatRenderContext *pRenderContext, bool bSorted, int *pNparticles)
{
	Vector vecCamera;
	pRenderContext->GetWorldSpaceCameraPosition( &vecCamera );
	GenerateSortedIndexList( vecCamera );
	*pNparticles = m_nActiveParticles;
	return s_SortedIndexList+m_nActiveParticles;
}




