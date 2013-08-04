#include <Game.hpp>
#include <System/Memory.hpp>
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
	}

	Game::~Game( )
	{
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
		m_pRenderer->EndScene( );
	}

	ZED_UINT32 Game::Execute( )
	{
		XEvent Event;
		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );
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

			this->Update( 16667 );

			this->Render( );
		}

		return ZED_OK;
	}
}

