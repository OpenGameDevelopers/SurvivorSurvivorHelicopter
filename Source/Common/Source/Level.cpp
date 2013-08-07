#include <Level.hpp>
#include <System/Memory.hpp>

namespace SurvivorSurvivorHelicopter
{
	Level::Level( )
	{
		m_pTileSetPath = ZED_NULL;
		m_pTileMap = ZED_NULL;
	}

	Level::Level( const ZED_CHAR8 *p_pLevelPath )
	{
	}

	Level::~Level( )
	{
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
		return ZED_OK;
	}

	void Level::Unload( )
	{
	}

	ZED_UINT32 Level::LevelData( LPLEVELDATA p_pData )
	{
		return ZED_FAIL;
	}
}

