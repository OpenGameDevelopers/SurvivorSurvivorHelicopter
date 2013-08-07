#ifndef __SURVIVORSURVIVORHELICOPTER_HPP__
#define __SURVIVORSURVIVORHELICOPTER_HPP__

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
		Level( const ZED_CHAR8 *p_pFileName );
		~Level( );

		ZED_UINT32	Initialise( );
		void		Render( );

		ZED_UINT32	Load( const ZED_CHAR8 *p_pLevel );

		// Retrieve the level data which can be used by entities within the
		// game
		ZED_UINT32	LevelData( LEVELDATA *p_pLevelData );

	private:
		ZED_UINT16	*m_pTileMap;
		ZED_CHAR8	*m_pTileSetPath;
	};
}

#endif

