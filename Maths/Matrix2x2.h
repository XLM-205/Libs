/*-------2x2 Matrix representation-------
*	--2x2 Template Matrix representation and operators
*		--Require "Vec2d.h" library for extra functionality (Row and Column fill)
*
*
*	Moon Wiz Studios (c) - 02/06/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.0.1
*/

#ifndef _MOON_MATH_MAT2_H_
#define _MOON_MATH_MAT2_H_

#include "Vec2d.h"

#ifndef HIDE_ALL_MAT					//Hides all the matrices macros
	#ifndef HIDE_MAT2					//Hide this matrix's macros, to prevent polluting the IDE

		#define mwMat2i Mat2<int>		//2x2 INT Matrix
		#define mwMat2f Mat2<float>		//2x2 FLOAT Matrix
		#define mwMat2d Mat2<double>	//2x2 DOUBLE Matrix

	#endif
#endif

//Index Reference:
// 0 > m11	| 2 > m21
// 1 > m12	| 3 > m22


//Pattern: column-major (Values filled from the first column to the others)
template <class type>
class Mat2
{
private:
	type Matrix[4];
public:
	Mat2() { Identity(); };		//If no parameter given, then make the matrix a identity
	Mat2(type Number)			//Fill the matrix with only one number
	{
		setMatrix(Number, Number, Number, Number);
	}
	Mat2(type Values[4])
	{
		setMatrix(Values);
	}
	Mat2(type m11, type m12, type m21, type m22)
	{
		setMatrix(m11, m12, m21, m22);
	}

	Mat2 &Identity()
	{
		Matrix[0] = Matrix[3] = 1;
		Matrix[1] = Matrix[2] = 0;
		return *this;
	}	//Test
	Mat2 &Transpose()
	{
		Utils::swap(Matrix[1], Matrix[2]);
		return *this;
	}
	Mat2 &Invert()
	{
		type InvDet = getDeterminant();
		if (InvDet == 0)	//Failsafe against x / 0
		{
			return Identity();
		}
		InvDet = 1 / InvDet;
		type TempM11 = Matrix[0];		//Temporary Holder for the M11 value
		Matrix[0] = Matrix[3] * InvDet;
		Matrix[1] = -Matrix[1] * InvDet;
		Matrix[2] = -Matrix[2] * InvDet;
		Matrix[3] = TempM11 * InvDet;
		return *this;
	}
	//Operators
	Mat2 operator-() const
	{
		return Mat2(-Matrix[0], -Matrix[1], -Matrix[2], -Matrix[3]);
	}
	Mat2 operator=(const Mat2 &m)
	{
		if (this != &m)
		{
			Matrix[0] = m[0];
			Matrix[1] = m[1];
			Matrix[2] = m[2];
			Matrix[3] = m[3];
		}
		return *this;
	}
	Mat2 operator+(const Mat2 &m) const
	{
		return Mat2(Matrix[0] + m[0], Matrix[1] + m[1], Matrix[2] + m[2], Matrix[3] + m[3]);
	}
	Mat2 & operator+=(const Mat2 &m)
	{
		Matrix[0] += m[0];
		Matrix[1] += m[1];
		Matrix[2] += m[2];
		Matrix[3] += m[3];
		return *this;
	}
	Mat2 operator-(const Mat2 &m) const
	{
		return Mat2(Matrix[0] - m[0], Matrix[1] - m[1], Matrix[2] - m[2], Matrix[3] - m[3]);
	}
	Mat2 & operator-=(const Mat2 &m)
	{
		Matrix[0] -= m[0];
		Matrix[1] -= m[1];
		Matrix[2] -= m[2];
		Matrix[3] -= m[3];
		return *this;
	}
	Mat2 operator*(const Mat2 &m) const
	{
		return Mat2((Matrix[0] * m[0]) + (Matrix[1] * m[2]), (Matrix[0] * m[1]) + (Matrix[1] * m[3]),
					(Matrix[2] * m[0]) + (Matrix[3] * m[2]), (Matrix[2] * m[1]) + (Matrix[3] * m[3]));
	}
	Mat2 & operator*=(const Mat2 &m)
	{
		//The pointer to this (the Matrix m) = the returned pointer from m * m
		*this = *this * m;
		return *this;
	}
	Vec2<type> operator*(const Vec2<type> &v) const		//As a property of matrices: You can do Mat2 * Vec2, but not Vec2 * Mat2
	{
		return Vec2<type>((Matrix[0] * v._X) + (Matrix[1] * v._Y), (Matrix[2] * v._X) + (Matrix[3] * v._Y));
	}
	bool operator==(const Mat2 &m) const
	{
		return ((Matrix[0] == m[0]) && (Matrix[1] == m[1]) && (Matrix[2] == m[2]) && (Matrix[3] == m[3]));
	}
	bool operator!=(const Mat2 &m) const
	{
		return !(*this == m);
	}
	type operator[](int Index) const
	{
		return Matrix[Index];
	}
	type & operator[](int Index)
	{
		return Matrix[Index];
	}
	//Sets and Gets
	void setMatrix(type Values[4])
	{
		Matrix[0] = Values[0];
		Matrix[1] = Values[1];
		Matrix[2] = Values[2];
		Matrix[3] = Values[3];
	}
	void setMatrix(type m11, type m12, type m21, type m22)
	{
		Matrix[0] = m11;
		Matrix[1] = m12;
		Matrix[2] = m21;
		Matrix[3] = m22;
	}
	void setRow(int Index, type RowData[2])
	{
		Matrix[Index] = RowData[0];
		Matrix[Index + 2] = RowData[1];
	}
	void setRow(int Index, Vec2<type> &VectorData)
	{
		Matrix[Index] = VectorData._X;
		Matrix[Index + 2] = VectorData._Y;
	}
	void setColumn(int Index, type ColumnData[2])
	{
		Matrix[Index * 2] = ColumnData[0];
		Matrix[(Index * 2) + 1] = ColumnData[1];
	}
	void setColumn(int Index, Vec2<type> &VectorData)
	{
		Matrix[Index * 2] = VectorData._X;
		Matrix[(Index * 2) + 1] = VectorData._Y;
	}
	type *getMatrix()
	{
		return Matrix;
	}
	type getDeterminant()
	{
		return (Matrix[0] * Matrix[3]) - (Matrix[1] * Matrix[2]);
	}

#ifdef _INC_STDIO
	void print(void)
	{
		Utils::PrintArray(Matrix, 4, 2, "[%d]");
	}
#endif
};

#endif;