/*-------Moon Physics Engine Core (WIP)-------
*	Core engine for physics transformations
*
*
*	Moon Wiz Studios (c) - 02/06/2015
*	by: Ramon Darwich de Menezes
*
*	This work is a copyright of Moon Wiz Studios
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.5
*/

#ifndef _MOON_PHYS_CORE_H_
#define _MOON_PHYS_CORE_H_

#ifndef _H_SSTREAM_
#define _H_SSTREAM_
#include <sstream>
#endif;

#ifndef _H_ALGORITHM_
#define _H_ALGORITHM_
#include <algorithm>
#endif;

#define NORM_LEFT 0										//Draw left normal
#define NORM_RIGTH 1									//Draw right normal

#define SUM_VEC_DRW 0									//Private: Used to indicate which vector to draw
#define NORM_VEC_DRW 1									//Private: Used to indicate which vector to draw
#define COL_NORM_VEC_DRW 2								//Private: Used to indicate which vector to draw

#define CONVERT_TO_DEG(x) ((std::acos(x)*180)/PI)		//Use it to convert to degree, the angle between 2 vectors (PhysObject<>::VecAngle(...))

#define PI 3.141592653

#define mwPhsObji PhysObject<int>
#define mwPhsObjf PhysObject<float>
#define mwPhsObjd PhysObject<double>

//#include <cstdarg>		//Header para funções com multiplos argumentos. Garante portabilidade. !! SOMENTE PARA C++. Para C use <stdarg.h> !!
#include "Vec3d.h"	//Already include it's Vec2 counterpart
#pragma message ("Compiling Physics Engine Core...")

//Definition zone
typedef unsigned char uint8;	//Using char as a number. Unsigned char = 256 max value (ub)
 
//Structures
struct AABB	//Axis Alined Bounding Box (Caixa de colisão com eixo alinhado)
{
	mwVec2f min;
	mwVec2f max;
};

struct Circle	//"Caixa" de colisão para circulos
{
	float Radius;
	mwVec2f Pos;
};

//Classes and functions zone
//WARNING !! TEMPLATE ONLY WORKS INSIDE A HEADER !!
template<class type>
class PhysObject
{
public:
	static type Dist2v(const Vec2<type> Vector1, const Vec2<type> Vector2);
	static type Dist2v(const Vec3<type> Vector1, const Vec3<type> Vector2);
	static Vec2<type> Normal(Vec2<type> Vector,uint8 NormalSide);				//<Vec3> Computes vector normals 
	static Vec2<type> Normal(Vec3<type> Vector, uint8 NormalSide);				//<Vec3> Computes vector normals
	static type Magnitude(const Vec2<type> Vector);								//<Vec2> Computes the magnitude (length) of a vector
	static type Magnitude(const Vec3<type> Vector);								//<Vec3> Computes the magnitude (length) of a vector
	static type Min(const type Val1, const type Val2);							//Return the minimum value between Val1 & Val2
	static type Max(const type Val1, const type Val2);							//Return the maximum value between Val1 & Val2
	static type VecAngle(const Vec2<type> Vector1, const Vec2<type> Vector2);	//<Vec2> Computes the angle between 2 Vectors
	static type VecAngle(const Vec3<type> Vector1, const Vec3<type> Vector2);	//<Vec3> Computes the angle between 2 Vectors
	static void DrawNormals(const Vec2<type> Vector);							//<Vec2> Draw the normals from the sum of all forces (aesthetic)
	static void DrawNormals(const Vec3<type> Vector);							//<Vec3> Draw the normals from the sum of all forces (aesthetic)
	static void DrawColisionNormal(const Vec2<type> Vector);					//Draw colision normal
	static void DrawSumVector(const Vec2<type> Vector);							//Draw a vector with the sum of all forces applied
private:
	PhysObject() {};
	~PhysObject() {};
	static void DrawVectorArrows(const Vec2<type> Vector, uint8 TypeDrawn);
};

//Functions Definitions
#include "MainDefinitions.h"

//Undef Core especific definitions
#undef NORM_VEC_DRW
#undef SUM_VEC_DRW
#undef COL_NORM_VEC_DRW
#endif;