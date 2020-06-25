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

#ifndef _MOON_PHYSICS_OBJDEF_H_
#define _MOON_PHYSICS_OBJDEF_H_

#include "Vec3d.h"
#include "Clamper.h"

class BaseObj
{
protected:
	mwVec3lf Pos;
	mwVec3lf Rot;

public:
	BaseObj() {};
	template <class type, class type2>
	BaseObj(Vec3l<type> &Position, Vec3l<type2> &Rotation) { Pos = Position; Rot = Rotation; };
	template <class type, class type2>
	BaseObj(Vec3<type> &Position, Vec3<type2> &Rotation) { Pos = Position; Rot = Rotation; };
	template <class type, class type2>
	BaseObj(Vec3<type> &Position, Vec3l<type2> &Rotation) { Pos = Position; Rot = Rotation; };
	template <class type, class type2>
	BaseObj(Vec3l<type> &Position, Vec3<type2> &Rotation) { Pos = Position; Rot = Rotation; };

	mwVec3lf &getPosition(void)
	{
		return Pos;
	}
	mwVec3lf &getRotation(void)
	{
		return Rot;
	}
	template <class type>
	void setPosition(Vec3l<type> &pos)
	{
		Pos = pos;
	}
	template <class type>
	void setRotation(Vec3l<type> &rot)
	{
		Rot = rot;
	}
	template <class type>
	void setPosition(Vec3<type> &pos)
	{
		Pos = pos;
	}
	template <class type>
	void setRotation(Vec3<type> &rot)
	{
		Rot = rot;
	}
};

class BasePhysObj : public BaseObj
{
protected:
	mwVec3f PosAccel;
	mwVec3f RotAccel;
	bool is_Sleeping = false;	//If true, the engine will NOT test colisions between it and other objects. Becomes false when idle for too much time

	//Physical properties
	float Energy = 0;
	float Velocity = 0;
	float Mass = 0;
	float Density = 0;	//Should only be set by a calulation between object's area and mass
	
	//Defines how much momentum the object will lost when moving. Defaults Max to [1]
	mwClampuf FrictionCoef;	//Fluctuates between 0 ~ 1. Higher values means MORE FRICTION
	//Defines how much momentum the object will lost when colliding. Defaults Max to [1] and Min to [0]
	mwClampf ElasticCoef;	//Fluctuautes between -1 ~ 1. If lower than 0, each collision will add extra momentum (doesn't affect post collision trajectory)

	void setEnergy(float En)
	{
		Energy = En;
	}
	void setVelocity(float Vl)
	{
		Velocity = Vl;
	}
	void setMass(float Ms)
	{
		Mass = Ms;
	}
	void setFrictionCoef(float FrCoef)
	{
		FrictionCoef.setValue(FrCoef);
	}
	void setElasticCoef(float ElCoef)
	{
		ElasticCoef.setValue(ElCoef);
	}

public:
	BasePhysObj() 
	{ 
		FrictionCoef.setClampMax(1); 
		ElasticCoef.setClampMin(0); 
		ElasticCoef.setClampMax(1);
	}
	template <class type, class type2>
	BasePhysObj(Vec3<type> &Position, Vec3<type2> &Rotation) : BaseObj(Position, Rotation) 
	{ 
		FrictionCoef.setClampMax(1); 
		ElasticCoef.setClampMin(0);
		ElasticCoef.setClampMax(1);
	}
	template <class type, class type2, class type3, class type4>
	BasePhysObj(Vec3<type> &PosAcceleration, Vec3<type2> &RotAcceleration, Vec3<type3> &Position, Vec3<type4> &Rotation) : BaseObj(Position, Rotation), PosAccel(PosAcceleration), RotAccel(RotAcceleration)
	{ 
		FrictionCoef.setClampMax(1); 
		ElasticCoef.setClampMin(0);
		ElasticCoef.setClampMax(1);
	}

	template <class type>
	void setPositionAccel(Vec3l<type> &posA)
	{
		PosAccel = posA;
		Pos = 0;
	}
	template <class type>
	void setRotationAccel(Vec3l<type> &rotA)
	{
		RotAccel = rotA;
	}
	template <class type>
	void setPositionAccel(Vec3<type> &posA)
	{
		PosAccel = posA;
	}
	template <class type>
	void setRotationAccel(Vec3<type> &rotA)
	{
		RotAccel = rotA;
	}
	mwVec3f &getPosAccel(void)
	{
		return PosAccel;
	}
	mwVec3f &getRotAccel(void)
	{
		return RotAccel;
	}
	float getMass(void)
	{
		return Mass;
	}
	float getDensity(void)
	{
		return Density;
	}
	float getVelocity(void)
	{
		return Velocity;
	}
	float getEnergy(void)
	{
		return Energy;
	}
	float getFrictionCoef(void)
	{
		return FrictionCoef.getValue();
	}
	float getElasticCoef(void)
	{
		return ElasticCoef.getValue();
	}
	float getFrictionCoefMax(void)
	{
		return FrictionCoef.getMax();
	}
	float getElasticCoefMax(void)
	{
		return ElasticCoef.getMax();
	}
	float getElasticCoefMin(void)
	{
		return ElasticCoef.getMin();
	}
	bool isSleeping(void)
	{
		return is_Sleeping;
	}
	void setSleep(void)
	{
		is_Sleeping = true;
	}
	void setAwake(void)
	{
		is_Sleeping = false;
	}
	void setSleepState(bool sts)
	{
		is_Sleeping = sts;
	}
	void setFrictionCoefMax(float Mx)
	{
		uClamper<float> Temp(Mx, 1);
		FrictionCoef.setClampMax(Temp.getValue());
	}
	void setElasticCoefMax(float Mx)
	{
		uClamper<float> Temp(Mx, 1);
		ElasticCoef.setClampMax(Mx);
	}
	void setElasticCoefMin(float Mn)
	{
		Clamper<float>(Mn, 1, -1);
		ElasticCoef.setClampMin(Mn);
	}
	void toggleSleepState(void)
	{
		is_Sleeping = !is_Sleeping;
	}
};

class PhysSquare : public BasePhysObj
{
protected:
	mwVec3f AxisA;	//Top-left
	mwVec3f AxisB;	//Bottom-Right

public:
	template <class type, class type2>
	PhysSquare(Vec3<type> &TopLft, Vec3<type2> &BttmRght) { AxisA = TopLft; AxisB = BttmRght; };
	template <class type, class type2, class type3, class type4>
	PhysSquare(Vec3<type> &TopLft, Vec3<type2> &BttmRght, Vec3<type3> &Position, Vec3<type4> &Rotation) : BasePhysObj(Position, Rotation) { AxisA = TopLft; AxisB = BttmRght; };

	template <class type>
	void setAxisA(Vec3<type> &Axis)
	{
		AxisA = Axis;
	}
	template <class type>
	void setAxisB(Vec3<type> &Axis)
	{
		AxisB = Axis;
	}
	mwVec3f &getAxisA(void)
	{
		return AxisA;
	}
	mwVec3f &getAxisB(void)
	{
		return AxisB;
	}
};

class PhysCircle : public BasePhysObj
{
protected:
	float Rad;

public:
	PhysCircle(void) : Rad(1) { };
	PhysCircle(float Radius) : Rad(Radius) { };
	template <class type, class type2>
	PhysCircle(float Radius, Vec3<type> &Position, Vec3<type2> &Rotation) : Rad(Radius), BasePhysObj(Position, Rotation) {};

	void setRadius(float Radius)
	{
		Rad = Radius;
	}
	float getRadius(void)
	{
		return Rad;
	}
};



#endif;	//ObjDef.h