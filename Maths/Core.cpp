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

#include <GL\glut.h>
#include <Objects.h>		//May be removed in further updates
#include "Core.h"

float ePisilon = 0;

void CollisionResolve(Object a, Object b)
{

}

bool AABBxAABB(const AABB Box1, const AABB Box2)
{
	//NÃO HOUVE colisão entre as caixas
	if ((Box1.max._X < Box2.min._X || Box1.min._X > Box2.max._X) || (Box1.max._Y < Box2.min._Y || Box1.min._Y > Box2.max._Y))
	{
		return false;
	}
	return true;
}

bool CirclexCircle(const Circle Cr1, const Circle Cr2)
{
	float Rad = Cr1.Radius + Cr2.Radius;	//Total de raio entre os circulos
	Rad = Rad * Rad;						//Para evitar ter de chamar Dist2V e não ter que usar std::std::sqrt (poupar CPU)
	return Rad < ((Cr1.Pos._X - Cr2.Pos._X) * (Cr1.Pos._X - Cr2.Pos._X)) + ((Cr1.Pos._Y - Cr2.Pos._Y) * (Cr1.Pos._Y - Cr2.Pos._Y));
	//Retorna true caso a distancia seja menor que a soma dos raios (COLISÃO) e false caso contrario
}