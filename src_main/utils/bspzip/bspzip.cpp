//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include <stdio.h>
#include "bsplib.h"
#include "cmdlib.h"
#include "tier0/icommandline.h"
#include "utlbuffer.h"

int CopyVariableLump( int lump, void **dest, int size );

void StripPath( const char* pPath, char* pBuf, int nBufLen )
{
	const char	*pSrc;

	// want filename only
	pSrc = pPath + Q_strlen( pPath ) - 1;
	while ((pSrc != pPath) && (*(pSrc-1) != '\\') && (*(pSrc-1) != '/') && (*(pSrc-1) != ':'))
		pSrc--;

	Q_strncpy( pBuf, pSrc, nBufLen );
}

void Usage( void )
{
	fprintf( stderr, "usage: \n" );
	fprintf( stderr, "bspzip -extract <bspfile> <blah.zip>\n");
	fprintf( stderr, "bspzip -extractfiles <bspfile>\n");
	fprintf( stderr, "bspzip -dir <bspfile>\n");
	fprintf( stderr, "bspzip -addfile <bspfile> <relativepathname> <fullpathname> <newbspfile>\n");
	fprintf( stderr, "bspzip -addlist <bspfile> <listfile> <newbspfile>\n");
	fprintf( stderr, "bspzip -addorupdatelist <bspfile> <listfile> <newbspfile>\n");
	fprintf( stderr, "bspzip -extractcubemaps <bspfile> <targetPath>\n");
	fprintf( stderr, "  Extracts the cubemaps to <targetPath>.\n");
	fprintf( stderr, "bspzip -deletecubemaps <bspfile>\n");
	fprintf( stderr, "  Deletes the cubemaps from <bspFile>.\n");
	fprintf( stderr, "bspzip -addfiles <bspfile> <relativePathPrefix> <listfile> <newbspfile>\n");
	fprintf( stderr, "  Adds files to <newbspfile>.\n");
	
	exit( -1 );
}

char* xzpFilename = NULL;

int main( int argc, char **argv )
{
	::SetHDRMode( false );

	Msg( "\nValve Software - bspzip.exe (%s)\n", __DATE__ );

	if (argc < 2 )
	{
		Usage();
	}
	
	CommandLine()->CreateCmdLine( argc, argv );
	CmdLib_InitFileSystem( argv[2] );

	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );
	if( ( stricmp( argv[1], "-extract" ) == 0 ) && argc >= 4 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension (bspName, ".bsp", sizeof( bspName ) );

		char zipName[1024];
		strcpy( zipName, argv[3] );
		Q_DefaultExtension (zipName, ".zip", sizeof( zipName ) );

		ExtractZipFileFromBSP( bspName, zipName );
	}
	else if( ( stricmp( argv[1], "-extractfiles" ) == 0 ) && argc >= 4 )
	{
		// necessary for xbox process
		// only the .vtf are extracted as necessary for streaming and not the .vmt
		// the .vmt are non-streamed and therefore remain, referenced normally as part of the bsp search path
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension( bspName, ".bsp", sizeof( bspName ) );

		char targetPathName[1024];
		strcpy( targetPathName, argv[3] );
		Q_AppendSlash( targetPathName, sizeof( targetPathName ) );

		printf( "\n" );
		printf( "Opening bsp file: %s\n", bspName );
		LoadBSPFile( bspName );		

		CUtlBuffer	buf;
		char		relativeName[1024];
		char		targetName[1024];
		int			fileSize;
		int			id = -1;
		int			numFilesExtracted = 0;
		while (1)
		{
			id = GetNextFilename( GetPakFile(), id, relativeName, sizeof(relativeName), fileSize );
			if ( id == -1)
				break;

			{
				buf.EnsureCapacity( fileSize );
				buf.SeekPut( CUtlBuffer::SEEK_HEAD, 0 );
				ReadFileFromPak( GetPakFile(), relativeName, false, buf );

				strcpy( targetName, targetPathName );
				strcat( targetName, relativeName );
				Q_FixSlashes( targetName, '\\' );

				SafeCreatePath( targetName );

				printf( "Writing file: %s\n", targetName );
				FILE *fp = fopen( targetName, "wb" );
				if ( !fp )
				{
					printf( "Error: Could not write %s\n", targetName);
					exit( -1 );
				}

				fwrite( buf.Base(), fileSize, 1, fp );
				fclose( fp );
				
				numFilesExtracted++;
			}
		}
		printf( "%d files extracted.\n", numFilesExtracted );
	}
	else if( ( stricmp( argv[1], "-extractcubemaps" ) == 0 ) && argc >= 4 )
	{
		// necessary for xbox process
		// only the .vtf are extracted as necessary for streaming and not the .vmt
		// the .vmt are non-streamed and therefore remain, referenced normally as part of the bsp search path
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension( bspName, ".bsp", sizeof( bspName ) );

		char targetPathName[1024];
		strcpy( targetPathName, argv[3] );
		Q_AppendSlash( targetPathName, sizeof( targetPathName ) );

		printf( "\n" );
		printf( "Opening bsp file: %s\n", bspName );
		LoadBSPFile( bspName );		

		CUtlBuffer	buf;
		char		relativeName[1024];
		char		targetName[1024];
		int			fileSize;
		int			id = -1;
		int			numFilesExtracted = 0;
		while (1)
		{
			id = GetNextFilename( GetPakFile(), id, relativeName, sizeof(relativeName), fileSize );
			if ( id == -1)
				break;

			if ( Q_stristr( relativeName, ".vtf" ) )
			{
				buf.EnsureCapacity( fileSize );
				buf.SeekPut( CUtlBuffer::SEEK_HEAD, 0 );
				ReadFileFromPak( GetPakFile(), relativeName, false, buf );

				strcpy( targetName, targetPathName );
				strcat( targetName, relativeName );
				Q_FixSlashes( targetName, '\\' );

				SafeCreatePath( targetName );

				printf( "Writing vtf file: %s\n", targetName );
				FILE *fp = fopen( targetName, "wb" );
				if ( !fp )
				{
					printf( "Error: Could not write %s\n", targetName);
					exit( -1 );
				}

				fwrite( buf.Base(), fileSize, 1, fp );
				fclose( fp );
				
				numFilesExtracted++;
			}
		}
		printf( "%d cubemaps extracted.\n", numFilesExtracted );
	}
	else if( ( stricmp( argv[1], "-deletecubemaps" ) == 0 ) && argc >= 3 )
	{
		// necessary for xbox process
		// the cubemaps are deleted as they cannot yet be streamed out of the bsp
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension( bspName, ".bsp", sizeof( bspName ) );

		printf( "\n" );
		printf( "Opening bsp file: %s\n", bspName );
		LoadBSPFile( bspName );		

		CUtlBuffer	buf;
		char		relativeName[1024];
		int			fileSize;
		int			id = -1;
		int			numFilesDeleted = 0;
		while (1)
		{
			id = GetNextFilename( GetPakFile(), id, relativeName, sizeof(relativeName), fileSize );
			if ( id == -1)
				break;

			if ( Q_stristr( relativeName, ".vtf" ) )
			{	
				RemoveFileFromPak( GetPakFile(), relativeName );
				
				numFilesDeleted++;

				// start over
				id = -1;
			}
		}

		printf( "%d cubemaps deleted.\n", numFilesDeleted );
		if ( numFilesDeleted )
		{
			// save out bsp file
			printf( "Updating bsp file: %s\n", bspName );
			WriteBSPFile( bspName );
		}
	}
	else if( ( stricmp( argv[1], "-addfiles" ) == 0 ) && argc >= 6 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension(bspName, ".bsp", sizeof( bspName ) );

		char relativePrefixName[1024];
		strcpy( relativePrefixName, argv[3] );

		char filelistName[1024];
		strcpy( filelistName, argv[4] );

		char newbspName[1024];
		Q_MakeAbsolutePath( newbspName, sizeof( newbspName ), argv[5] );
		Q_DefaultExtension( newbspName, ".bsp", sizeof( newbspName) );

		char fullpathName[1024];
		FILE *fp = fopen(filelistName, "r");
		if ( fp )
		{
			printf("Opening bsp file: %s\n", bspName);
			LoadBSPFile(bspName);		

			while ( !feof(fp) )
			{
				if ( ( fgets( fullpathName, 1024, fp ) != NULL ) )
				{
					fullpathName[strlen(fullpathName) - 1] = '\0';

					// strip the path and add relative prefix
					char fileName[1024];
					char relativeName[1024];
					StripPath( fullpathName, fileName, sizeof( fileName ) );
					Q_strcpy( relativeName, relativePrefixName );
					Q_strcat( relativeName, fileName, sizeof( relativeName ) );

					printf( "Adding file: %s as %s\n", fullpathName, relativeName );

					AddFileToPak( GetPakFile(), relativeName, fullpathName );
				}
				else if ( !feof( fp ) )
				{
					printf( "Error: Missing full path names\n");
					fclose( fp );
					return( -1 );
				}
			}
			printf("Writing new bsp file: %s\n", newbspName);
			WriteBSPFile(newbspName);
			fclose( fp );
		}
	}
	else if( ( stricmp( argv[1], "-dir" ) == 0 ) && argc >= 3 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension (bspName, ".bsp", sizeof( bspName ) );
		LoadBSPFile (bspName);		
		PrintBSPPackDirectory();
	}
	else if( ( stricmp( argv[1], "-addfile" ) == 0 ) && argc >= 6 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension (bspName, ".bsp", sizeof( bspName ) );

		char relativeName[1024];
		strcpy( relativeName, argv[3] );

		char fullpathName[1024];
		strcpy( fullpathName, argv[4] );

		char newbspName[1024];
		Q_MakeAbsolutePath( newbspName, sizeof( newbspName ), argv[5] );
		Q_DefaultExtension (newbspName, ".bsp", sizeof( newbspName ) );

		// read it in, add pack file, write it back out
		LoadBSPFile (bspName);		
		AddFileToPak( GetPakFile(), relativeName, fullpathName );
		WriteBSPFile(newbspName);
	}
	else if( ( stricmp( argv[1], "-addlist" ) == 0 ) && argc >= 5 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension (bspName, ".bsp", sizeof( bspName ) );

		char filelistName[1024];
		strcpy( filelistName, argv[3] );

		char newbspName[1024];
		Q_MakeAbsolutePath( newbspName, sizeof( newbspName ), argv[4] );
		Q_DefaultExtension (newbspName, ".bsp", sizeof( newbspName) );

		// read it in, add pack file, write it back out

		char relativeName[1024];
		char fullpathName[1024];
		FILE *fp = fopen(filelistName, "r");
		if ( fp )
		{
			printf("Opening bsp file: %s\n", bspName);
			LoadBSPFile (bspName);		

			while ( !feof(fp) )
			{
				relativeName[ 0 ] = 0;
				fullpathName[ 0 ] = 0;
				if ( ( fgets( relativeName, 1024, fp ) != NULL ) &&	
					 ( fgets( fullpathName, 1024, fp ) != NULL ) )
				{
					int l1 = strlen(relativeName);
					int l2 = strlen(fullpathName);
					if ( l1 > 0 )
					{
						if ( relativeName[ l1 - 1 ] == '\n' )
						{
							relativeName[ l1 - 1 ] = 0;
						}
					}
					if ( l2 > 0 )
					{
						if ( fullpathName[ l2 - 1 ] == '\n' )
						{
							fullpathName[ l2 - 1 ] = 0;
						}
					}
					printf("Adding file: %s\n", fullpathName);
					AddFileToPak( GetPakFile(), relativeName, fullpathName );
				}
				else if ( !feof( fp ) || ( relativeName[0] && !fullpathName[0] ) )
				{
					printf( "Error: Missing paired relative/full path names\n");
					fclose( fp );
					return( -1 );
				}
			}
			printf("Writing new bsp file: %s\n", newbspName);
			WriteBSPFile(newbspName);
			fclose( fp );
		}
	}
	else if( ( stricmp( argv[1], "-addorupdatelist" ) == 0 ) && argc >= 5 )
	{
		char bspName[1024];
		Q_MakeAbsolutePath( bspName, sizeof( bspName ), argv[2] );
		Q_DefaultExtension (bspName, ".bsp", sizeof( bspName ) );

		char filelistName[1024];
		strcpy( filelistName, argv[3] );

		char newbspName[1024];
		Q_MakeAbsolutePath( newbspName, sizeof( newbspName ), argv[4] );
		Q_DefaultExtension (newbspName, ".bsp", sizeof( newbspName) );

		// read it in, add pack file, write it back out

		char relativeName[1024];
		char fullpathName[1024];
		FILE *fp = fopen(filelistName, "r");
		if ( fp )
		{
			printf("Opening bsp file: %s\n", bspName);
			LoadBSPFile (bspName);		

			while ( !feof(fp) )
			{
				relativeName[ 0 ] = 0;
				fullpathName[ 0 ] = 0;
				if ( ( fgets( relativeName, 1024, fp ) != NULL ) &&	
					( fgets( fullpathName, 1024, fp ) != NULL ) )
				{
					int l1 = strlen(relativeName);
					int l2 = strlen(fullpathName);
					if ( l1 > 0 )
					{
						if ( relativeName[ l1 - 1 ] == '\n' )
						{
							relativeName[ l1 - 1 ] = 0;
						}
					}
					if ( l2 > 0 )
					{
						if ( fullpathName[ l2 - 1 ] == '\n' )
						{
							fullpathName[ l2 - 1 ] = 0;
						}
					}
					
					bool bUpdating = FileExistsInPak( GetPakFile(), relativeName );
					printf("%s file: %s\n", bUpdating ? "Updating" : "Adding", fullpathName);
					if ( bUpdating )
						RemoveFileFromPak( GetPakFile(), relativeName );
					AddFileToPak( GetPakFile(), relativeName, fullpathName );
				}
				else if ( !feof( fp ) || ( relativeName[0] && !fullpathName[0] ) )
				{
					printf( "Error: Missing paired relative/full path names\n");
					fclose( fp );
					return( -1 );
				}
			}
			printf("Writing new bsp file: %s\n", newbspName);
			WriteBSPFile(newbspName);
			fclose( fp );
		}
	}
	else
	{
		Usage();
	}
	return 0;
}
