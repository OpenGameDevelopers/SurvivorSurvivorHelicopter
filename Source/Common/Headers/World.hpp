#ifndef __SURVIVORSURVIVORHELICOPTER_WORLD_HPP__
#define __SURVIVORSURVIVORHELICOPTER_WORLD_HPP__

#include <list>

namespace SurvivorSurvivorHelicopter
{
	class Level;
	class Survivor;
	class SpawnLocation;

	class World
	{
	public:
		World( );
		~World( );

	private:
		Level	*m_pLevel;

		std::list< Survivor > m_Survivors;
		std::list< SpawnLocation > m_SpawnLocations;
	};
}

#endif

