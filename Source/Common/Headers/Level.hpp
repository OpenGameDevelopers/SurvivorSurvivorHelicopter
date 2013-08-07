#ifndef __SURVIVORSURVIVORHELICOPTER_LEVEL_HPP__
#define __SURVIVORSURVIVORHELICOPTER_LEVEL_HPP__

#include <System/DataTypes.hpp>

namespace SurvivorSurvivorHelicopter
{
#pragma pack( 1 )
	typedef struct __tagLEVELHEADER
	{
		ZED_CHAR8	ID[ 4 ];
		ZED_CHAR8	Path[ 256 ];
		ZED_UINT16	Dimensions[ 2 ];
	}LEVELHEADER,*LPLEVELHEADER;

	typedef struct __tagLEVELDATA
	{
		ZED_UINT16	*pData;
		ZED_UINT16	Dimensions[ 2 ];
	}LEVELDATA,*LPLEVELDATA;
#pragma pop

	class Level
	{
	public:
		Level( );
		Level( const ZED_CHAR8 *p_pLevelPath );
		~Level( );

		ZED_UINT32	Initialise( );
		void		Render( );

		ZED_UINT32	Load( const ZED_CHAR8 *p_pLevelPath );
		void		Unload( );

		// Retrieve the level data which can be used by entities within the
		// game
		ZED_UINT32	LevelData( LPLEVELDATA p_pData );

	private:
		ZED_UINT16	*m_pTileMap;
		ZED_CHAR8	*m_pTileSetPath;
	};
}

#endif

