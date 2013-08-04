#ifndef __SURVIVORSURVIVORHELICOPTER_GAME_HPP__
#define __SURVIVORSURVIVORHELICOPTER_GAME_HPP__

#include <System/DataTypes.hpp>
#include <System/Window.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/CanvasDescription.hpp>
#include <System/InputManager.hpp>
#include <System/Keyboard.hpp>



namespace SurvivorSurvivorHelicopter
{
	class Game
	{
	public:
		Game( );
		~Game( );

		ZED_UINT32 Initialise( );
		void Update( const ZED_UINT64 p_ElapsedGameTime );
		void Render( );

		ZED_UINT32 Execute( );

	private:
		ZED::Renderer::Renderer				*m_pRenderer;
		ZED::Renderer::CanvasDescription	m_Canvas;
		ZED::System::Window					*m_pWindow;
		ZED::System::InputManager			*m_pInputManager;
		ZED::System::Keyboard				m_Keyboard;

		ZED_BOOL	m_Running;
		ZED_BOOL	m_FullScreen;
	};
}

#endif

