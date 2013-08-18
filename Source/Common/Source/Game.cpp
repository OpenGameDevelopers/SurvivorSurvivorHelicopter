#include <Game.hpp>
#include <System/Memory.hpp>
#include <System/File.hpp>
#include <System/Time.hpp>
#include <GitVersion.hpp>
#include <cstring>

namespace SurvivorSurvivorHelicopter
{
	Game::Game( )
	{
		m_pRenderer = ZED_NULL;
		m_pWindow = ZED_NULL;
		m_pInputManager = ZED_NULL;
		memset( &m_Canvas, 0, sizeof( m_Canvas ) );
		m_Running = ZED_FALSE;
		m_FullScreen = ZED_FALSE;
		m_pTestLevel = ZED_NULL;
	}

	Game::~Game( )
	{
		zedSafeDelete( m_pTestLevel );
		zedSafeDelete( m_pRenderer );
		zedSafeDelete( m_pInputManager );
		zedSafeDelete( m_pWindow );
	}

	void Game::Update( const ZED_UINT64 p_ElapsedGameTime )
	{
	}

	void Game::Render( )
	{
		m_pRenderer->BeginScene( ZED_TRUE, ZED_TRUE, ZED_TRUE );
		m_pTestLevel->Render( );
		m_pRenderer->EndScene( );
	}

	ZED_UINT32 Game::Execute( )
	{
		XEvent Event;
		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
		char *pBinDir = new ZED_CHAR8[ 256 ];
		char *pLevelPath = new ZED_CHAR8[ 256 ];
		ZED_UINT64 ElapsedTime = 0ULL;
		ZED_UINT64 TimeStep = 16667ULL;
		ZED_UINT64 OldTime = ZED::System::GetTimeMiS( );
		ZED_UINT64 FrameTime = ZED::System::GetTimeMiS( );
		ZED_MEMSIZE FrameRate = 0;
		ZED_UINT64 Accumulator = 0ULL;
		char WindowTitle[ 1024 ] = "SurvivorSurvivor Helicopter! Alpha Build";
#ifdef ZED_BUILD_DEBUG
		strcat( WindowTitle, " | [DEBUG] | [Ver. " );
		strcat( WindowTitle, GIT_COMMITHASH );
		strcat( WindowTitle, "]" );
#elif defined ZED_BUILD_RELEASE
		strcat( WindowTitle, " | [Ver. " );
		strcat( WindowTitle, GIT_COMMITHASH );
		strcat( WindowTitle, "]" );
#elif defined ZED_BUILD_PROFILE
		strcat( WindowTitle, " | [PROFILE] | [Ver. " );
		strcat( WindowTitle, GIT_COMMITHASH );
		strcat( WindowTitle, "]" );
#else
#error Unknown build target
#endif

		memset( pBinDir, '\0', sizeof( ZED_CHAR8 )*256 );
		memset( pLevelPath, '\0', sizeof( ZED_CHAR8 )*256 );

		m_pTestLevel = new Level( );

		ZED::System::GetExecutablePath( &pBinDir, 256 );

		if( m_pTestLevel->Initialise( m_pRenderer ) != ZED_OK )
		{
			zedSafeDeleteArray( pLevelPath );
			zedSafeDeleteArray( pBinDir );
			return ZED_FAIL;
		}

		strcat( pLevelPath, pBinDir );
		strcat( pLevelPath, "GameMedia/Levels/TestLevel.ssh" );

		if( m_pTestLevel->Load( pLevelPath ) != ZED_OK )
		{
			zedSafeDeleteArray( pLevelPath );
			zedSafeDeleteArray( pBinDir );
			return ZED_FAIL;
		}

		m_pWindow->Title( WindowTitle );

		while( m_Running == ZED_TRUE )
		{
			m_pWindow->Update( );
			m_pInputManager->Update( );

			while( XPending( WinData.pX11Display ) > 0 )
			{
				XNextEvent( WinData.pX11Display, &Event );

				switch( Event.type )
				{
					default:
					{
						break;
					}
				}
			}

			if( m_pWindow->Closed( ) )
			{
				m_Running = ZED_FALSE;
			}

			if( m_Keyboard.IsKeyDown( K_ESCAPE ) )
			{
				m_Running = ZED_FALSE;
			}

			const ZED_UINT64 NewTime = ZED::System::GetTimeMiS( );
			ZED_UINT64 DeltaTime = NewTime - OldTime;

			if( DeltaTime > 250000ULL )
			{
				DeltaTime = 250000ULL;
			}

			OldTime = NewTime;
			Accumulator += DeltaTime;

			while( Accumulator >= TimeStep )
			{
				this->Update( 16667 );

				ElapsedTime += TimeStep;
				Accumulator -= DeltaTime;
			}

			this->Render( );
			++FrameRate;

			if( ( NewTime - FrameTime ) > 1000000ULL )
			{
				zedTrace( "FPS: %d\n", FrameRate );
				FrameTime = ZED::System::GetTimeMiS( );
				FrameRate = 0;
			}
		}

		zedSafeDeleteArray( pLevelPath );
		zedSafeDeleteArray( pBinDir );

		return ZED_OK;
	}
}

