/*-------Shape Drawer-------
*	WIP
*	--WIP
*
*
*	Moon Wiz Studios (c) - 09/10/2018
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.0.1
*/

#ifndef _H_MWSD
#define _H_MWSD

#include <gl\GL.h>

#include "..\Control\CustomTypes-SA.h"
#include "..\Maths\Vec3d.h"
#include "Mesh.h"
#include "ShapeTemplate.h"

enum ShapeDrawModes
{
	SHAPE_DRAW_LINE,		//Draw as a plain line (default)
	SHAPE_DRAW_POINTS,		//Draw as an array of points
	SHAPE_DRAW_FULL			//Draw as a full polygon
};

inline void glColorRGBPalette(Pixel &Pal)
{
	glColor3ub(Pal.getRed(), Pal.getGreen(), Pal.getBlue());
}

inline void glColorRGBAPalette(Pixel &Pal)
{
	glColor4ub(Pal.getRed(), Pal.getGreen(), Pal.getBlue(), Pal.getAlpha());
}

template <typename T>
inline void glVertexVec(const Vec2<T> &v)
{
	glVertex2i(v._X, v._Y);
}
template <>
inline void glVertexVec<float>(const Vec2<float> &v)
{
	glVertex2f(v._X, v._Y);
}
template <>
inline void glVertexVec<double>(const Vec2<double> &v)
{
	glVertex2d(v._X, v._Y);
}
template <typename T>
inline void glVertexVec(const Vec3<T> &v)
{
	glVertex3i(v._X, v._Y, v._Z);
}
template <>
inline void glVertexVec<float>(const Vec3<float> &v)
{
	glVertex3f(v._X, v._Y, v._Z);
}
template <>
inline void glVertexVec<double>(const Vec3<double> &v)
{
	glVertex3d(v._X, v._Y, v._Z);
}

namespace DrawShape
{
	void Circle(BaseShape &c, ShapeDrawModes d)
	{
		switch (d)
		{
		case SHAPE_DRAW_LINE:
			glBegin(GL_LINE_STRIP);
			break;
		case SHAPE_DRAW_POINTS:
			glBegin(GL_POINTS);
			break;
		case SHAPE_DRAW_FULL:
			glBegin(GL_POLYGON);
			break;
		}
				for (uint32 i = 0; i < c.getSections(); i++)
				{
					glVertexVec(c[i]);
				}
			glEnd();
	}

	void Rectangle(BaseShape &c, ShapeDrawModes d)
	{
		switch (d)
		{
		case SHAPE_DRAW_LINE:
			glBegin(GL_LINE_STRIP);
			break;
		case SHAPE_DRAW_POINTS:
			glBegin(GL_POINTS);
			break;
		case SHAPE_DRAW_FULL:
			glBegin(GL_POLYGON);
			break;
		}
				glVertexVec(c[0]);
				glVertexVec(c[1]);
				glVertexVec(c[3]);
				glVertexVec(c[2]);
				glVertexVec(c[0]);
			glEnd();
	}
}

template <class Vec>
void drawVecSetOf(uint32 Mode, Vec *input, uint32 size)
{
	glBegin(Mode);
		for(int i = 0; i < size; i++)
		{
			glVertexVec(input[i]);
		}
	glEnd();
}

template <class Vec>
void drawVecSetOf(uint32 Mode, Pixel *Palletes, Vec *input, uint32 size)
{
	glBegin(Mode);
	for(int i = 0; i < size; i++)
	{
		glColorRGBAPalette(Palletes[i]);
		glVertexVec(input[i]);
	}
	glEnd();
}

void drawVecSetOf(uint32 Mode, Mesh *input)
{
	glBegin(Mode);
		for(uint32 i = 0; i < input->getWidth(); i++)
		{
			for(uint32 j = 0; j < input->getHeight(); j++)
			{
				glVertexVec(input->getVertex(i, j));
			}
		}
	glEnd();
}

void drawMesh(Mesh *input)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	for(uint32 i = 0; i < input->getWidth() - 1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
			for(uint32 j = 0; j < input->getHeight(); j++)
			{
				glVertexVec(input->getVertex(i, j));
				glVertexVec(input->getVertex(i + 1, j));
			}
		glEnd();
	}
	glPolygonMode(GL_FRONT, GL_FILL);
}

void drawMesh(Mesh *input, Pixel *Palletes)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	for(uint32 i = 0; i < input->getWidth() - 1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		glColorRGBPalette(Palletes[i]);
		for(uint32 j = 0; j < input->getHeight(); j++)
		{
			glVertexVec(input->getVertex(i, j));
			glVertexVec(input->getVertex(i + 1, j));
		}
		glEnd();
	}
	glPolygonMode(GL_FRONT, GL_FILL);
}

#endif	//Moon Wiz Shape Drawer