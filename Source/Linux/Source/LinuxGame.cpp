#include <Game.hpp>
#include <Renderer/LinuxRendererOGL3.hpp>
#include <System/LinuxWindow.hpp>
#include <System/LinuxInputManager.hpp>

namespace SurvivorSurvivorHelicopter
{
	ZED_UINT32 Game::Initialise( )
	{
		m_pRenderer = new ZED::Renderer::LinuxRendererOGL3( );
		m_pWindow = new ZED::System::LinuxWindow( );

		ZED_UINT32 X = 0, Y = 0, Width = 0, Height = 0;
		ZED::System::ZED_SCREENSIZE NativeSize;
		ZED::System::ZED_SCREENSIZE *pScreenSizes = ZED_NULL;
		ZED_MEMSIZE ScreenSizeCount = 0;

		ZED::System::EnumerateScreenSizes( &pScreenSizes, &ScreenSizeCount, 0,
			ZED::System::GetCurrentScreenNumber( ) );

		if( ZED::System::GetNativeScreenSize( 0,
			ZED::System::GetCurrentScreenNumber( ), NativeSize ) != ZED_OK )
		{
			zedTrace( "[SSH!::Game::Initialise] <ERROR> "
				"Could not get native screen size\n" );

			return ZED_FAIL;
		}

		ZED_UINT32 WindowStyle = 0;

		if( m_FullScreen == ZED_TRUE )
		{
			X = 0;
			Y = 0;
			Width = NativeSize.Width;
			Height = NativeSize.Height;

			WindowStyle |= ZED_WINDOW_STYLE_FULLSCREEN;
		}
		else
		{
#ifdef ZED_BUILD_DEBUG
			Width = 1280;
			Height = 720;
			X = ( NativeSize.Width / 2 ) - ( Width / 2 );
			Y = ( NativeSize.Height / 2 ) - ( Height / 2 );
#else
#endif
			WindowStyle |= ZED_WINDOW_STYLE_MINIMISE | ZED_WINDOW_STYLE_CLOSE |
				ZED_WINDOW_STYLE_TITLEBAR | ZED_WINDOW_STYLE_MOVE;
		}

		m_pWindow->Create( X, Y, Width, Height, 0,
			ZED::System::GetCurrentScreenNumber( ), WindowStyle );

		m_Canvas.Width( Width );
		m_Canvas.Height( Height );
		m_Canvas.BackBufferCount( 1 );
		m_Canvas.ColourFormat( ZED_FORMAT_ARGB8 );
		m_Canvas.DepthStencilFormat( ZED_FORMAT_D24S8 );

		m_pRenderer->Create( m_Canvas, ( *m_pWindow ) );

		m_pRenderer->ClearColour( 0.2f, 0.0f, 0.0f );
		ZED::System::ZED_WINDOWDATA WinData = m_pWindow->WindowData( );

		m_pInputManager =
			new ZED::System::LinuxInputManager( WinData.pX11Display );
		m_pInputManager->AddDevice( &m_Keyboard );

		m_Running = ZED_TRUE;

		return ZED_OK;
	}
}

