#ifndef __SURVIVORSURVIVORHELICOPTER_LEVEL_HPP__
#define __SURVIVORSURVIVORHELICOPTER_LEVEL_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>
#include <list>
#include <map>
#include <Renderer/OGL/GLShader.hpp>
#include <Renderer/OGL/GLModel.hpp>

namespace SurvivorSurvivorHelicopter
{
#pragma pack( 1 )
	typedef struct __tagLEVELHEADER
	{
		ZED_CHAR8	ID[ 4 ];
		ZED_CHAR8	Path[ 256 ];
		ZED_UINT16	Dimensions[ 2 ];
	}LEVELHEADER,*LPLEVELHEADER;

	typedef struct __tagLEVELTILE
	{
		ZED_UINT16	ID;
		ZED_UINT16	Type;
	}LEVELTILE,*LPLEVELTILE;

	typedef struct __tagLEVELDATA
	{
		LEVELTILE	*pData;
		ZED_UINT16	Dimensions[ 2 ];
	}LEVELDATA,*LPLEVELDATA;

	// TEMPORARY!
	typedef struct __tagLEVELVERT
	{
		ZED_FLOAT32	Position[ 3 ];
		ZED_FLOAT32	Normal[ 3 ];
		ZED_FLOAT32	Colour[ 3 ];
	}LEVELVERT,*LPLEVELVERT;
	// !TEMPORARY
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
		LPLEVELDATA	m_pData;
		std::map< ZED_BYTE, std::list< ZED::Arithmetic::Vector3 > >
			m_TileToPosition;

		ZED_UINT32 TilePresent( const ZED_CHAR8 *p_pPath,
			const ZED_BYTE p_ID ) const;

		// TEMPORARY!
		ZED::Renderer::GLModel	*m_pModel;
		ZED::Renderer::GLShader *m_pShader;
		// !TEMPORARY
	};
}

#endif

