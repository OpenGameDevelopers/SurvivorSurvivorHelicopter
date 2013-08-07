#ifndef __SURVIVORSURVIVORHELICOPTER_WORLD_HPP__
#define __SURVIVORSURVIVORHELICOPTER_WORLD_HPP__

namespace SurvivorSurvivorHelicopter
{
	class Level;

	class World
	{
	public:
		World( )
		~World( );

	private:
		Level	*m_pLevel;
	};
}

#endif

