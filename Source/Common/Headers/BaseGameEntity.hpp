#ifndef __SURVIVORSURVIVORHELICOPTER_BASEGAMEENTITY_HPP__
#define __SURVIVORSURVIVORHELICOPTER_BASEGAMEENITTY_HPP__

#include <System/DataTypes.hpp>
#include <Arithmetic/Vector3.hpp>

namespace SurvivorSurvivorHelicopter
{
	class BaseGameEntity
	{
	public:
		virtual ~BaseGameEntity( ){ }

		ZED_UINT32 ID( const ZED_CHAR8 *p_pName );
		ZED_UINT32 ID( ) const;

		void Position( const ZED::Arithmetic::Vector3 &p_Position )
			{ m_Position = p_Position; }
		void Position( ZED::Arithmetic::Vector3 *p_pPosition ) const
			{ ( *p_pPosition ) = m_Position; }

		void Orientation( const ZED::Arithmetic::Vector3 &p_Orientation )
			{ m_Orientation = p_Orientation; }
		void Orientation( ZED::Arithmetic::Vector3 *p_pOrientation ) const
			{ ( *p_pOrientation ) = m_Orientation; }

		void Visible( const ZED_BOOL p_Visible ) { m_Visible = p_Visible; }
		ZED_BOOL Visible( ) const { return m_Visible; }

	protected:
		ZED_UINT32					m_ID;
		ZED::Arithmetic::Vector3	m_Position;
		ZED::Arithmetic::Vector3	m_Orientation;
		ZED_BOOL					m_Visible;
	};
}

#endif

