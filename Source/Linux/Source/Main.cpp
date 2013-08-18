#include <iostream>
#include <GitVersion.hpp>
#include <Game.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "SurvivorSurvivor Helicopter!" << std::endl;
	std::cout << "\tVersion: " << GIT_COMMITHASH << std::endl;

	SurvivorSurvivorHelicopter::Game TheGame;

	if( TheGame.Initialise( ) != ZED_OK )
	{
		std::cout << "[SSH!::main] <ERROR> "
			"Failed to initialise the game" << std::endl;

		return 0;
	}

	return TheGame.Execute( );
}

