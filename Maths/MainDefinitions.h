/*-------Main Physics core Definitions-------
*	Holds all the definitions required by the physics engine core
*	--(WIP)
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

#include "Core.h"

template<class type>
type PhysObject<type>::Dist2v(const Vec2<type> Vector1, const Vec2<type> Vector2)
{
	return std::sqrt(((Vector1._X - Vector2._X) * (Vector1._X - Vector2._X)) + ((Vector1._Y - Vector2._Y) * (Vector1._Y - Vector2._Y)));
}

template<class type>
type PhysObject<type>::Dist2v(const Vec3<type> Vector1, const Vec3<type> Vector2)
{
	return std::sqrt(((Vector1._X - Vector2._X) * (Vector1._X - Vector2._X)) + ((Vector1._Y - Vector2._Y) * (Vector1._Y - Vector2._Y)) + ((Vector1._Z - Vector2._Z) * (Vector1._Z - Vector2._Z)));
}

template<class type>
Vec2<type> PhysObject<type>::Normal(Vec2<type> Vector, uint8 NormalSide)
{
	Vec2<type> TempVec = Vector;
	if (NormalSide == NORM_LEFT)
	{
		Vector._X = -TempVec._Y;
		Vector._Y = TempVec._X;
	}
	else
	{
		Vector._X = TempVec._Y;
		Vector._Y = -TempVec._X;
	}
	return Vector;
}

template<class type>
type PhysObject<type>::Magnitude(Vec2<type> Vector)
{
	return std::sqrt((Vector._X * Vector._X) + (Vector._Y * Vector._Y));
}

template<class type>
type PhysObject<type>::Magnitude(Vec3<type> Vector)
{
	return std::sqrt((Vector._X * Vector._X) + (Vector._Y * Vector._Y) + (Vector._Z * Vector._Z));
}

template<class type>
type PhysObject<type>::VecAngle(const Vec2<type> Vector1, const Vec2<type> Vector2)
{
	Vec2<type> U = Vector1, V = Vector2;
	float Mag1 = Magnitude(Vector1), Mag2 = Magnitude(Vector2), UxV = U * V;
	if (Mag1 > 0 && Mag2 > 0)
	{
		return UxV / (Mag1 * Mag2);
	}
	else
	{
		return 0;
	}
}

template<class type>
type PhysObject<type>::VecAngle(const Vec3<type> Vector1, const Vec3<type> Vector2)
{
	Vec3<type> U = Vector1, V = Vector2;
	type Mag1 = Magnitude(Vector1), Mag2 = Magnitude(Vector2), UxV = U * V;
	if (Mag1 > 0 && Mag2 > 0)
	{
		return UxV / (Mag1 * Mag2);
	}
	else
	{
		return 0;
	}
}

template<class type>
type PhysObject<type>::Min(const type Val1, const type Val2)
{
	return std::min(Val1, Val2);
}

template<class type>
type PhysObject<type>::Max(const type Val1, const type Val2)
{
	return std::max(Val1, Val2);
}

//Astetic Functions
template<class type>
void PhysObject<type>::DrawSumVector(const Vec2<type> Vector)
{
	glColor3ub(255, 128, 0);
	glBegin(GL_LINES);
		glVertex2i(0, 0);
		glVertex2f(Vector._X, Vector._Y);
	glEnd();
	PhysObject<type>::DrawVectorArrows(Vector, SUM_VEC_DRW);
}

template<class type>
void PhysObject<type>::DrawNormals(Vec2<type> Vector)
{
	Vec2<type> TempVec = Vector;
	glColor3ub(0, 255, 255);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		TempVec = PhysObject<type>::Normal(Vector, NORM_LEFT);
		glVertex2f(TempVec._X, TempVec._Y);
	glEnd();
	TempVec = Vector;
	glColor3ub(0, 255, 255);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		TempVec = PhysObject<type>::Normal(Vector, NORM_RIGTH);
		glVertex2f(TempVec._X, TempVec._Y);
	glEnd();
	PhysObject<type>::DrawVectorArrows(Vector, NORM_VEC_DRW);
}

template<class type>
void PhysObject<type>::DrawColisionNormal(const Vec2<type> Vector)
{

}

template<class type>
void PhysObject<type>::DrawVectorArrows(const Vec2<type> Vector, uint8 TypeDrawn)
{
	//Vector Sum
	if (TypeDrawn == SUM_VEC_DRW)
	{
		glPushMatrix();
		glTranslatef(Vector._X, Vector._Y, 0);
		glRotatef((std::atan2(0 - Vector._X, Vector._Y - 0) * 180) / PI, 0, 0, 1);
		glColor3ub(255, 0, 0);
		glBegin(GL_TRIANGLES);
			glVertex2i(30, 0);
			glVertex2i(0, 30);
			glVertex2i(-30, 0);
		glEnd();
		glPopMatrix();
	}
	else if (TypeDrawn == NORM_VEC_DRW)
	{
		//Vector Normal
		for (int i = 0; i < 2; i++)
		{
			glPushMatrix();
			if (i == 0)
			{
				glTranslatef(-Vector._Y / 1000, Vector._X / 1000, 0);
				glRotatef((std::atan2(Vector._Y, Vector._X) * 180) / PI, 0, 0, 1);
			}
			else
			{
				glTranslatef(Vector._Y / 1000, -Vector._X / 1000, 0);
				glRotatef((std::atan2(-Vector._Y, -Vector._X) * 180) / PI, 0, 0, 1);
			}
			glColor3ub(255, 0, 255);
			glBegin(GL_TRIANGLES);
				glVertex2i(30, 0);
				glVertex2i(0, 30);
				glVertex2i(-10, 0);
			glEnd();
			glPopMatrix();
		}
	}
}
//END of Aesthetic Functions
