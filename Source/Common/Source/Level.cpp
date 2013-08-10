#include <Level.hpp>
#include <System/Memory.hpp>
#include <System/Debugger.hpp>
#include <cstdio>
#include <cstring>
#include <dirent.h>

namespace SurvivorSurvivorHelicopter
{
	Level::Level( )
	{
		m_pTileSetPath = ZED_NULL;
		m_pTileMap = ZED_NULL;
		m_pModel = ZED_NULL;
		m_pShader = ZED_NULL;
	}

	Level::Level( const ZED_CHAR8 *p_pLevelPath )
	{
		m_pTileSetPath = ZED_NULL;
		m_pTileMap = ZED_NULL;

		this->Initialise( );
		this->Load( p_pLevelPath );
	}

	Level::~Level( )
	{
		zedSafeDeleteArray( m_pShader );
		zedSafeDeleteArray( m_pModel );
		zedSafeDeleteArray( m_pTileSetPath );
		zedSafeDeleteArray( m_pTileMap );
	}

	ZED_UINT32 Level::Initialise( )
	{		
		return ZED_OK;
	}

	void Level::Render( )
	{
	}

	ZED_UINT32 Level::Load( const ZED_CHAR8 *p_pLevelPath )
	{
		if( p_pLevelPath == ZED_NULL )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"No level file passed in\n" );
			return ZED_FAIL;
		}

		FILE *pFile = fopen( p_pLevelPath, "rb" );

		if( pFile == ZED_NULL )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"Failed to open level: \"%s\"\n", p_pLevelPath );
			return ZED_FAIL;
		}

		// Make sure the file is at least the size of a header
		fseek( pFile, 0, SEEK_END );
		long FileSize = ftell( pFile );

		if( FileSize < sizeof( LEVELHEADER ) )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"Size of file is too small\n" );

			return ZED_FAIL;
		}

		rewind( pFile );

		LEVELHEADER Header;

		memset( &Header, 0, sizeof( Header ) );

		fread( &Header, sizeof( Header ), 1, pFile );

		if( strncmp( Header.ID, "SSHL", 4 ) != 0 )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"Header not valid, found: \"%s\", expected: \"SSHL\"\n",
				Header.ID );

			return ZED_FAIL;
		}
		// Below code commented out for now because loading the meshes isn't
		// important or relevant at this stage
		/*
		if( Header.Path == ZED_NULL )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"Header path invalid\n" );
				
			return ZED_FAIL;
		}

		// Before reading the level data, confirm that the tiles for the
		// survivor spawn point and the helicopter are present
		DIR *pTileDirectory;

		if( ( pTileDirectory = opendir( Header.Path ) ) == ZED_NULL )
		{
			return ZED_FAIL;
		}
		
		struct dirent *pEntry;
		ZED_UINT32	FilesFound = 0;

		while( ( pEntry = readdir( pTileDirectory ) ) != ZED_NULL )
		{
			if( strncmp( pEntry->d_name, "00000.zed", 10 ) == 0 )
			{
				FilesFound |= 1;
			}
			if( strncmp( pEntry->d_name, "00001.zed", 10 ) == 0 )
			{
				FilesFound |= 2;
			}			
		}

		if( !( FilesFound & 1 ) || !( FilesFound & 2 ) )
		{
			zedTrace( "[SurvivorSurvivorHelicopter::Level::Load] <ERROR> "
				"Failed to locate tiles 000 and 001\n" );

			return ZED_FAIL;
		}

		fread( m_pData->pData, sizeof( ZED_BYTE ),
			Header.Dimensions[ 0 ]*Header.Dimensions[ 1 ], pFile );

		// The only thing left to do now is to check that the other tiles are
		// in the directory
		for( ZED_MEMSIZE TC = 0;
			TC < ( Header.Dimensions[ 0 ]*Header.Dimensions[ 1 ] ); ++TC )
		{
			
		}

		if( pTileDirectory )
		{
			closedir( pTileDirectory );
			pTileDirectory = ZED_NULL;
		}*/

		// Yes, I know it's naive, but bear with me...

		ZED_FLOAT32 ZPosition = 0.0f;
		ZED_MEMSIZE Counter = 0;

		for( ZED_UINT16 Row = 0; Row < Header.Dimensions[ 0 ]; ++Row )
		{
			for( ZED_UINT16 Col = 0; Col < Header.Dimensions[ 1 ]; ++Col )
			{
				switch( m_pData->pData[ Row*Col ].Type )
				{
					// Helicopter
					case 0x0001:
					{
						m_TileToPosition[ 0 ].push_back(
							ZED::Arithmetic::Vector3( ( 100.0f * Col ) + 50.0f,
								0.0f, ( -100.0f * Row ) - 50.0f ) );
						break;
					}
					// Spawn point
					case 0x0002:
					{
						m_TileToPosition[ 1 ].push_back(
							ZED::Arithmetic::Vector3( ( 100.0f * Col ) + 50.0f,
								0.0f, ( -100.0f * Row ) - 50.0f ) );
						break;
					}
					// Regular
					case 0x0004:
					{
						m_TileToPosition[ 2 ].push_back(
							ZED::Arithmetic::Vector3( ( 100.0f * Col ) + 50.0f,
								0.0f, ( -100.0f * Row ) - 50.0f ) );
						break;
					}
					default:
					{
						break;
					}
				}
			}

			ZPosition -= 100.0f;
		}
		
		if( pFile != ZED_NULL )
		{
			fclose( pFile );
			pFile = ZED_NULL;
		}

		return ZED_OK;
	}

	void Level::Unload( )
	{
	}

	ZED_UINT32 Level::LevelData( LPLEVELDATA p_pData )
	{
		return ZED_FAIL;
	}

	ZED_UINT32 Level::TilePresent( const ZED_CHAR8 *p_pPath,
		const ZED_BYTE p_ID ) const
	{
		DIR *pDirectory;
		struct dirent *pEntry;
		char FileName[ 10 ];
		memset( FileName, '\0', 10 );
		snprintf( FileName, 10, "%03d.zed\0", p_ID );

		while( ( pEntry = readdir( pDirectory ) ) != ZED_NULL )
		{
			if( strncmp( pEntry->d_name, FileName, 8 ) )
			{
				closedir( pDirectory );
				pDirectory = ZED_NULL;

				return ZED_OK;
			}
		}

		return ZED_FAIL;
	}
}

