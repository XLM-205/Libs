/*------- WIP -------
*	--
*
*	Moon Wiz Studios (c) - --/--/--
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version --.--
*/

#ifndef _MOON_PHYSICS_COLCORE_H_
#define _MOON_PHYSICS_COLCORE_H_

#include "ObjDef.h"
#include "..\Control\HashTable.h"

enum PhysicsOptions
{
	MWP_SETUP_DIMENSION_2D,
	MWP_SETUP_DIMENSION_3D,

};

static class PhysicsCore
{
	/*
			   A(x,y,z) +---------------------------+
					   /|						   /|
					  /	|						  /	|
					 /	|						 /	|
				   	/   |						/	|
		  C(x,y,z) +---------------------------+	|
				   |	|					   |	|
				   |	|					   |	|
				   |	+----------------------|----+ B(x,y,z)
				   |   /					   |   / 
				   |  /						   |  /
				   | /						   | /
				   |/						   |/
				   +---------------------------+ D(x,y,z)
	*/
private:
	mwVec3d m_BoundaryA, m_BoundaryB;	//Represents the primary square boundary of the world. If the world is 2D, only this Boundary should have values
	mwVec3d m_BoundaryC, m_BoundaryD;	//Represents the secondary square boundary of the world. Same as the first Boundary but with the 'Depth' accounted for
	mwVec3d m_WorldCenter;
	double m_Depth = 0;
	uint8 m_WorldDimension = MWP_SETUP_DIMENSION_2D;	//Defaults to 2D

	bool m_is_WorldBuilt = false;

	PhysicsCore(mwVec3d A, mwVec3d B, mwVec3d C, mwVec3d D) : m_BoundaryA(A), m_BoundaryB(B), m_BoundaryC(C), m_BoundaryD(D)
	{
		m_Depth = abs(A._Z - D._Z);
		if (m_Depth)
		{
			m_WorldDimension = MWP_SETUP_DIMENSION_3D;
		}
		else
		{
			m_BoundaryC.zero();
			m_BoundaryD.zero();
		}
	}
	PhysicsCore(mwVec3d A, mwVec3d B, double Depth) : m_BoundaryA(A), m_BoundaryB(B), m_BoundaryC(A), m_BoundaryD(B), m_Depth(Depth)
	{
		m_BoundaryC._Z = m_Depth;
		m_BoundaryD._Z = m_Depth;
		if (m_Depth)
		{
			m_WorldDimension = MWP_SETUP_DIMENSION_3D;
		}
		else
		{
			m_BoundaryC.zero();
			m_BoundaryD.zero();
		}
	}

public:
	mwVec3d& getBoundaryA(void)
	{
		return m_BoundaryA;
	}
	mwVec3d& getBoundaryB(void)
	{
		return m_BoundaryB;
	}
	mwVec3d& getBoundaryC(void)
	{
		return m_BoundaryC;
	}
	mwVec3d& getBoundaryD(void)
	{
		return m_BoundaryD;
	}
	
};

namespace PhysicsOperations
{
	namespace Primitives
	{
		template <typename T1, typename T2>
		double CircleToCircleDist2D(Vec2<T1> A, Vec2<T2> B)
		{

			return sqrt(Pow2(A._X + B._X) + Pow2(A._Y + B._Y));
		}

		template <typename T1, typename T2>
		bool CircleCollision2D(Vec2<T1> A, double RadiusA, Vec2<T2> B, double RadiusB)
		{
			mwVec2d Diffs(A._X - B._X, A._Y - B._Y);
			double Radii = RadiusA + RadiusB;
			return (Pow2(Diffs._X) + Pow2(Diffs._Y) <= Pow2(Radii));
		}
		
	}

	template <typename T>
	T Pow2(T input)
	{
		return input * input;
	}
	template <typename T>
	T Pow3(T input)
	{
		return input * input * input;
	}
//	//Using a method of SQRT, as seen where (method 14):
//	//https://www.codeproject.com/kb/cpp/sqrt_prec_vs_speed.aspx
//	//WARNING ! ASSEMBLY CODE !! FURTHER STUDY REQUIRED
//	template <typename T>
//	double inline __declspec (naked) __fastcall Sqrt(double n)
//	{
//		_asm fld qword ptr[esp + 4]
//		_asm fsqrt
//		_asm ret 8
//	}
}

#endif;	//CollisionCore.h