#include <Level.hpp>
#include <System/Memory.hpp>
#include <System/Debugger.hpp>
#include <System/File.hpp>
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
		m_pRenderer = ZED_NULL;
	}

	Level::Level( const ZED::Renderer::Renderer *p_pRenderer,
		const ZED_CHAR8 *p_pLevelPath )
	{
		m_pTileSetPath = ZED_NULL;
		m_pTileMap = ZED_NULL;
		m_pRenderer = ZED_NULL;

		this->Initialise( p_pRenderer );
		this->Load( p_pLevelPath );
	}

	Level::~Level( )
	{
		zedSafeDelete( m_pShader );
		zedSafeDelete( m_pModel );
		zedSafeDeleteArray( m_pTileSetPath );
		zedSafeDeleteArray( m_pTileMap );
	}

	ZED_UINT32 Level::Initialise( const ZED::Renderer::Renderer *p_pRenderer )
	{
		ZED_CHAR8 *pBinDir = new ZED_CHAR8[ 256 ];
		ZED_CHAR8 *pVertexShader = new ZED_CHAR8[ 256 ];
		ZED_CHAR8 *pFragmentShader = new ZED_CHAR8[ 256 ];
		ZED_CHAR8 *pModelFile = new ZED_CHAR8[ 256 ];

		ZED::System::GetExecutablePath( &pBinDir, 256 );
		m_pRenderer = const_cast< ZED::Renderer::Renderer * >( p_pRenderer );
		m_pModel = new ZED::Renderer::GLModel( m_pRenderer );
		m_pShader = new ZED::Renderer::GLShader( );

		memset( pVertexShader, '\0', sizeof( ZED_CHAR8 )*256 );
		memset( pFragmentShader, '\0', sizeof( ZED_CHAR8 )*256 );
		memset( pModelFile, '\0', sizeof( ZED_CHAR8)*256 );
		strcat( pVertexShader, pBinDir );
		strcat( pFragmentShader, pBinDir );
		strcat( pModelFile, pBinDir );
		strcat( pVertexShader, "GameMedia/Shaders/Level.vsh" );
		strcat( pFragmentShader, "GameMedia/Shaders/Level.fsh" );
		strcat( pModelFile, "GameMedia/Models/BlankTile.zed" );

		if( m_pShader->Compile(
			const_cast< const ZED_CHAR8 ** >( &pVertexShader ),
			ZED_VERTEX_SHADER, ZED_TRUE ) != ZED_OK )
		{
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pVertexShader );
			zedSafeDeleteArray( pFragmentShader );
			zedSafeDeleteArray( pModelFile );

			return ZED_FAIL;
		}

		zedSafeDelete( pVertexShader );

		if( m_pShader->Compile(
			const_cast< const ZED_CHAR8 ** >( &pFragmentShader ),
			ZED_FRAGMENT_SHADER, ZED_TRUE ) != ZED_OK )
		{
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pFragmentShader );
			zedSafeDeleteArray( pModelFile );

			return ZED_FAIL;
		}

		zedSafeDelete( pFragmentShader );

		if( m_pModel->Load( pModelFile ) != ZED_OK )
		{
			zedSafeDeleteArray( pBinDir );
			zedSafeDeleteArray( pModelFile );

			return ZED_FAIL;
		}

		zedSafeDeleteArray( pBinDir );
		zedSafeDeleteArray( pModelFile );

		ZED_SHADER_VERTEXATTRIBUTE_GL Attributes[ 2 ];
		ZED_SHADER_CONSTANT_MAP Constants[ 10 ];

		Attributes[ 0 ].Index = 0;
		Attributes[ 0 ].pName = "vPosition";
		Attributes[ 1 ].Index = 1;
		Attributes[ 1 ].pName = "vNormal";

		m_pShader->SetVertexAttributeTypes( Attributes, 2 );

		zedSetConstant( Constants, 0, ZED_MAT4X4, "uWVP" );
		zedSetConstant( Constants, 1, ZED_FLOAT3, "uGlobalAmbient" );
		zedSetConstant( Constants, 2, ZED_FLOAT3, "uLightColour" );
		zedSetConstant( Constants, 3, ZED_FLOAT3, "uLightPos" );
		zedSetConstant( Constants, 4, ZED_FLOAT3, "uEyePos" );
		zedSetConstant( Constants, 5, ZED_FLOAT3, "uEmissive" );
		zedSetConstant( Constants, 6, ZED_FLOAT3, "uAmbient" );
		zedSetConstant( Constants, 7, ZED_FLOAT3, "uDiffuse" );
		zedSetConstant( Constants, 8, ZED_FLOAT3, "uSpecular" );
		zedSetConstant( Constants, 9, ZED_FLOAT1, "uShininess" );

		m_pShader->SetConstantTypes( Constants, 10 );

		ZED::Arithmetic::Vector3 GlobalAmbient( 0.0001f, 0.0001f, 0.0001f );
		ZED::Arithmetic::Vector3 LightColour( 0.5f, 0.5f, 0.5f );
		ZED::Arithmetic::Vector3 LightPosition( 0.0f, 200.0f, 0.0f );

		m_pShader->Activate( );
		m_pShader->SetConstantData( 1, ( void * )( &GlobalAmbient ) );
		m_pShader->SetConstantData( 2, ( void * )( &LightColour ) );
		m_pShader->SetConstantData( 3, ( void * )( &LightPosition ) );
		
		return ZED_OK;
	}

	void Level::Render( )
	{
		std::map< ZED_BYTE,
			std::list< ZED::Arithmetic::Vector3 > >::const_iterator Itr =
			m_TileToPosition.begin( );
	
		while( Itr != m_TileToPosition.end( ) )
		{
			ZED::Arithmetic::Matrix4x4	WVP;
			ZED::Arithmetic::Matrix4x4	PerspProj;
			ZED::Arithmetic::Matrix4x4	Translation;
			ZED::Arithmetic::Matrix4x4	World;
			ZED::Arithmetic::Vector3	CameraPosition( 0.0f, 900.0f, -100.0f );
			ZED::Arithmetic::Vector3	Up( 0.0f, 1.0f, 0.0f );

			m_pShader->Activate( );

			m_pShader->SetConstantData( 4, ( void * )( &CameraPosition ) );

			if( Itr->first == 0 )
			{
				ZED::Arithmetic::Vector3 Emissive( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Ambient( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Diffuse( 1.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Specular( 0.0f, 0.0f, 0.0f );
				ZED_FLOAT32	Shininess = 0.0f;

				m_pShader->Activate( );

				m_pShader->SetConstantData( 5, ( void * )( &Emissive ) );
				m_pShader->SetConstantData( 6, ( void * )( &Ambient ) );
				m_pShader->SetConstantData( 7, ( void * )( &Diffuse ) );
				m_pShader->SetConstantData( 8, ( void * )( &Specular ) );
				m_pShader->SetConstantData( 9, ( void * )( &Shininess ) );
			}
			else if( Itr->first == 1 )
			{
				ZED::Arithmetic::Vector3 Emissive( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Ambient( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Diffuse( 0.0f, 1.0f, 0.0f );
				ZED::Arithmetic::Vector3 Specular( 0.0f, 0.0f, 0.0f );
				ZED_FLOAT32	Shininess = 0.0f;

				m_pShader->Activate( );

				m_pShader->SetConstantData( 5, ( void * )( &Emissive ) );
				m_pShader->SetConstantData( 6, ( void * )( &Ambient ) );
				m_pShader->SetConstantData( 7, ( void * )( &Diffuse ) );
				m_pShader->SetConstantData( 8, ( void * )( &Specular ) );
				m_pShader->SetConstantData( 9, ( void * )( &Shininess ) );
			}
			else if( Itr->first == 2 )
			{
				ZED::Arithmetic::Vector3 Emissive( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Ambient( 0.0f, 0.0f, 0.0f );
				ZED::Arithmetic::Vector3 Diffuse( 0.5f, 0.5f, 0.5f );
				ZED::Arithmetic::Vector3 Specular( 0.0f, 0.0f, 0.0f );
				ZED_FLOAT32	Shininess = 0.0f;

				m_pShader->Activate( );

				m_pShader->SetConstantData( 5, ( void * )( &Emissive ) );
				m_pShader->SetConstantData( 6, ( void * )( &Ambient ) );
				m_pShader->SetConstantData( 7, ( void * )( &Diffuse ) );
				m_pShader->SetConstantData( 8, ( void * )( &Specular ) );
				m_pShader->SetConstantData( 9, ( void * )( &Shininess ) );
			}
			std::list< ZED::Arithmetic::Vector3 >::const_iterator Itr2 =
				Itr->second.begin( );
			while( Itr2 != Itr->second.end( ) )
			{
				ZED::Arithmetic::Vector3 Position( Itr2->X( ), Itr2->Y( ),
					Itr2->Z( ) );
				ZED::Arithmetic::Vector3 Up( 0.0f, 1.0f, 0.0f );
				ZED::Arithmetic::Vector3 LookPos( 0.0f, 0.0f, -200.0f );
				ZED_FLOAT32 Matrix[ 16 ];

				m_pRenderer->SetViewLookAt( CameraPosition, LookPos, Up );
				m_pRenderer->PerspectiveProjectionMatrix( &PerspProj );
				m_pRenderer->GetWVP( &World );

				Translation.Translate( Position );

				WVP = PerspProj*World*Translation;

				WVP.AsFloat( Matrix );

				m_pShader->Activate( );

				m_pShader->SetConstantData( 0, Matrix );

				m_pModel->Render( );

				++Itr2;
			}

			++Itr;
		}
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

		m_pData =
			new LEVELTILE[ Header.Dimensions[ 0 ]*Header.Dimensions[ 1 ] ];
		fread( m_pData, sizeof( LEVELTILE ),
			Header.Dimensions[ 0 ]*Header.Dimensions[ 1 ], pFile );

		for( ZED_UINT16 Row = 0; Row < Header.Dimensions[ 0 ]; ++Row )
		{
			for( ZED_UINT16 Col = 0; Col < Header.Dimensions[ 1 ]; ++Col )
			{
				ZED::Arithmetic::Vector3 Position( ( 200.0f * Col ) - 100.0f,
					0.0f, ( -200.0f * Row ) - 100.0f );
				switch( m_pData[ ( Row*Header.Dimensions[ 1 ] ) + Col ].Type )
				{
					// Helicopter
					case 0x0001:
					{
						zedTrace( "Pushing a helicopter\n" );
						m_TileToPosition[ 0 ].push_back( Position );
						break;
					}
					// Spawn point
					case 0x0002:
					{
						zedTrace( "Pushing a spawn point\n" );
						m_TileToPosition[ 1 ].push_back( Position );
						break;
					}
					// Regular
					case 0x0004:
					{
						zedTrace( "Pushing a regular tile\n" );
						m_TileToPosition[ 2 ].push_back( Position );
						break;
					}
					default:
					{
						zedTrace( "Unknown tile\n" );
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

