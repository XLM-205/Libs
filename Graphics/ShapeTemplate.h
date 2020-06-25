/*-------Shape Stencil Template-------
*	Build stencil for custom shapes to use in Graphics Applications
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
*	Version 1.0
*/

#ifndef _H_MWSHAPE_
#define _H_MWSHAPE_
//Moon Wiz Shape a.k.a MWS

#include <math.h>
#include "..\Maths\Vec2d.h"
#include "..\Control\CustomTypes-SA.h"

//#define MWS_DEBUG_TIMES

#ifdef MWS_DEBUG_TIMES
#include <iostream>
#include <time.h>
#endif

class BaseShape
{
protected:
	uint32 m_sections;
	float m_baseScale;
	bool m_isBuiltSucess;
	mwVec2d m_shapeModifier;
	mwVec2d *m_Stencil;

public:
	BaseShape()
	{
		m_sections = 0;
		m_baseScale = 1;
		m_shapeModifier = 1;
		m_Stencil = nullptr;
		m_isBuiltSucess = false;
	}
	~BaseShape()
	{
		if (m_Stencil)
		{
			delete[] m_Stencil;
#ifdef MWS_DEBUG_TIMES
			printf("Stencil Destroyed");
			getchar();
#endif
		}
	}

	virtual uint32 getSections(void)
	{
		return m_sections;
	}
	virtual float getScale(void)
	{
		return m_baseScale;
	}
	virtual mwVec2d getShapeModifier(void)
	{
		return m_shapeModifier;
	}
	mwVec2d getStencilAt(uint32 index)
	{
		return m_Stencil[index];
	}

	virtual bool getStatus(void) = 0;
	virtual void setStatus(bool built) = 0;
	virtual void setShapeModifier(float ModfX, float ModfY) = 0;
	virtual void setShapeModifier(mwVec2d Modf) = 0;
	virtual void setScale(float Scale) = 0;
	virtual void setSections(uint32 Sections) = 0;

	mwVec2d operator[](const int index)
	{
		mwVec2d out;
		out = m_Stencil[index] * m_baseScale;
		out._X *= m_shapeModifier._X;
		out._Y *= m_shapeModifier._Y;
		return out;
	}
};

//Designed to created circle stencils. Since it uses the amount of sections to build a circle, lower values of sections build more "square" circles
//As a result, the shape that will be outputted when drawing (when in low section counts) will have Sections - 1 vertexes. e.g.: A circle with 4 sections will be a triangle (3 vertexes)
class ShapeCircle : public BaseShape
{
private:
	double m_Radius;

	void buildStencil(void)
	{
		setStatus(true);
		double Angle = 0;
		for (uint32 i = 0; i < m_sections; i++)
		{
			Angle = (CST_2PI * i) / (m_sections - 1);
			m_Stencil[i]._X = cos(Angle);
			m_Stencil[i]._Y = sin(Angle);
		}
		m_Radius = m_Stencil[0]._X;
	}
	virtual void setSections(uint32 Sections)
	{
		Sections <= 1 ? m_sections = 2 : m_sections = Sections;
	}
	virtual void setStatus(bool built)
	{
		this->m_isBuiltSucess = built;
	}

public:
	ShapeCircle(uint32 SectionsAmount)
	{
		setSections(SectionsAmount);
		m_Stencil = new mwVec2d[m_sections];
		if (m_Stencil != NULL)
		{
			buildStencil();
		}
	}
	ShapeCircle(uint32 SectionsAmount, float StencilScale)
	{
		setSections(SectionsAmount);
		setScale(StencilScale);
		m_Stencil = new mwVec2d[m_sections];
		if (m_Stencil != NULL)
		{
			buildStencil();
		}
	}
	ShapeCircle(uint32 SectionsAmount, float StencilScale, mwVec2d Modifier)
	{
		setSections(SectionsAmount);
		setScale(StencilScale);
		setShapeModifier(Modifier);
		m_Stencil = new mwVec2d[m_sections];
		if (m_Stencil != NULL)
		{
			buildStencil();
		}
	}
	ShapeCircle(uint32 SectionsAmount, float StencilScale, float ModifierX, float ModifierY)
	{
		setSections(SectionsAmount);
		setScale(StencilScale);
		setShapeModifier(ModifierX, ModifierY);
		m_Stencil = new mwVec2d[m_sections];
		if (m_Stencil != NULL)
		{
			buildStencil();
		}
	}

	virtual bool getStatus(void)
	{
		return m_isBuiltSucess;
	}
	mwVec2d* getStencil(void)
	{
		return m_Stencil;
	}
	double getRadius(void)
	{
		return m_Radius;
	}
	void setModifierDefault(void)
	{
		setShapeModifier(1, 1);
	}
	virtual void setShapeModifier(float ModfX, float ModfY)
	{
		m_shapeModifier.setValues(ModfX, ModfY);
	}
	virtual void setShapeModifier(mwVec2d Modf)
	{
		m_shapeModifier = Modf;
	}
	virtual void setScale(float Scale)
	{
		Scale < 0 ? m_baseScale = 0 : m_baseScale = Scale;
	}

	void rebuildStencil(uint32 newSections)
	{
		delete[] m_Stencil;
		setSections(newSections);
		m_Stencil = new mwVec2d[m_sections];
		if (m_Stencil != NULL)
		{
			buildStencil();
		}
	}
	void reScale(float Scale)
	{
		setScale(Scale);
		for (uint32 i = 0; i < m_sections; i++)
		{
			m_Stencil[i] *= m_baseScale;
		}
	}

	//Operators
	mwVec2d operator[](int Index) const
	{
		return m_Stencil[Index];
	}
	mwVec2d & operator[](int Index)
	{
		return m_Stencil[Index];
	}
};

//Defines a rectangle with a pair of points PA and PB, where PA is the TOP-RIGHT most point, and PB is the BOTTOM-LEFT most point
class ShapeRectangle : public BaseShape
{
protected:
	// PA[0](+)----+ [1]
	//	     |     |
	//   [2] +----(+)[3]PB
	mwVec2d m_PA, m_PB;
	double m_Area;

	void computeArea(void)
	{
		m_Area = (m_PA._X - m_PB._X) * (m_PA._Y - m_PB._Y);
	}
	virtual void setStatus(bool built)
	{
		m_isBuiltSucess = built;
	}
	virtual void setSections(uint32 Sections)
	{
		m_sections = Sections;
	}
	void buildStencil(void)
	{
		setStatus(true);
		m_Stencil = new mwVec2d[m_sections];
		updateStencil();
		computeArea();
	}
	void updateStencil(void)
	{
		m_Stencil[0] = m_PA;
		m_Stencil[1].setValues(m_PB._X, m_PA._Y);
		m_Stencil[2].setValues(m_PA._X, m_PB._X);
		m_Stencil[3] = m_PB;
	}

public:
	ShapeRectangle(mwVec2d PA, mwVec2d PB)
	{ 
		m_PA = PA;
		m_PB = PB;
		setSections(4);
		buildStencil();
	}

	ShapeRectangle(double PAX, double PAY, double PBX, double PBY)
	{
		m_PA.setValues(PAX, PAY);
		m_PB.setValues(PBX, PBY);
		setSections(4);
		buildStencil();
	}

	void setPA(mwVec2d PA)
	{
		m_PA = PA;
		updateStencil();
	}
	void setPA(double X, double Y)
	{
		m_PA.setValues(X, Y);
		updateStencil();
	}
	void setPB(mwVec2d PB)
	{
		m_PB = PB;
		updateStencil();
	}
	void setPB(double X, double Y)
	{
		m_PB.setValues(X, Y);
		updateStencil();
	}
	mwVec2d getPA(void)
	{
		return m_PA;
	}
	mwVec2d getPB(void)
	{
		return m_PB;
	}
	double DeltaHeight(void)
	{
		return m_PA._Y - m_PB._Y;
	}
	double DeltaWidth(void)
	{
		return m_PA._X - m_PB._X;
	}
	
	virtual bool getStatus(void)
	{
		return m_isBuiltSucess;
	}
	virtual void setShapeModifier(float ModfX, float ModfY)
	{
		m_shapeModifier.setValues(ModfX, ModfY);
	}
	virtual void setShapeModifier(mwVec2d Modf)
	{
		m_shapeModifier = Modf;
	}
	virtual void setScale(float Scale)
	{
		m_baseScale = Scale;
	}

};

class ShapeWave : public BaseShape
{

};

//( !! WIP !!)
class ShapeStar : protected BaseShape
{
private:


	virtual void setSections(uint32 Sections)
	{
		this->m_sections = Sections;
	}
	virtual void setScale(float Scale)
	{
		m_baseScale = Scale;
	}

public:


	virtual uint32 getSections(void)
	{
		return m_sections;
	}
	virtual float getScale(void)
	{
		return m_baseScale;
	}
	virtual bool getStatus(void)
	{
		return m_isBuiltSucess;
	}
};

#endif;	//Library