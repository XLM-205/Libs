/*-------3x3 Matrix representation-------
*	--3x3 Template Matrix representation and operators
*		--Require "Vec3d.h" library for extra functionality (Row and Column fill)
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

#ifndef _MOON_MATH_MAT3_H_
#define _MOON_MATH_MAT3_H_

#include "Vec3d.h"

#ifndef HIDE_ALL_MAT					//Hides all the matrices macros
	#ifndef HIDE_MAT3					//Hide this matrix's macros, to prevent polluting the IDE

		#define mwMat3i Mat3<int>		//3x3 INT Matrix
		#define mwMat3f Mat3<float>		//3x3 FLOAT Matrix
		#define mwMat3d Mat3<double>	//3x3 DOUBLE Matrix

	#endif
#endif

//Index Reference:
// 0 > m11	| 3 > m21	| 6 > m31
// 1 > m12	| 4 > m22	| 7 > m32
// 2 > m13	| 5 > m23	| 8 > m33

//Pattern: column-major (Values filled from the first column to the others)
template <class type>
class Mat3
{
private:
	type Matrix[9];
public:
	Mat3() { Identity(); };		//If no parameter given, then make the matrix a identity
	Mat3(type Number)			//Fill the matrix with only one number
	{
		setMatrix(Number, Number, Number, Number, Number, Number, Number, Number, Number);
	}
	Mat3(type Values[9])
	{
		setMatrix(Values);
	}
	Mat3(type m11, type m12, type m13, type m21, type m22, type m23, type m31, type m32, type m33)
	{
		setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
	}

	Mat3 &Identity()
	{
		Matrix[0] = Matrix[4] = Matrix[8] =  1;
		Matrix[1] = Matrix[2] = Matrix[3] = Matrix[5] = Matrix[6] = Matrix[7] = 0;
		return *this;
	}	//Test
	Mat3 &Transpose()
	{
		Utils::swap(Matrix[1], Matrix[3]);
		Utils::swap(Matrix[2], Matrix[6]);
		Utils::swap(Matrix[5], Matrix[7]);
		return *this;
	}
	Mat3 &Invert()
	{
		//Reference 3x3 matrix. The signal are only for the equation (i - j + k)
		//In this method, we will pick the determinant of a 2x2 matrix and multiply it for the reference. The reference will draw a cross, excluding the others 5 values thus forming the 2x2 matrix
		//		[+m11  -m21	+m31]
		//		[-m12  +m22	-m32]
		//		[+m13  -m23 +m33]
		Mat3<type> Temp;
		Temp[0] = (Matrix[4] * Matrix[8]) - (Matrix[5] * Matrix[7]);
		Temp[3] = (Matrix[1] * Matrix[8]) - (Matrix[2] * Matrix[7]);
		Temp[6] = (Matrix[1] * Matrix[5]) - (Matrix[4] * Matrix[2]);

		Temp[1] = (Matrix[3] * Matrix[8]) - (Matrix[5] * Matrix[6]);
		Temp[4] = (Matrix[0] * Matrix[8]) - (Matrix[6] * Matrix[2]);
		Temp[7] = (Matrix[0] * Matrix[5]) - (Matrix[3] * Matrix[2]);

		Temp[2] = (Matrix[3] * Matrix[7]) - (Matrix[4] * Matrix[6]);
		Temp[5] = (Matrix[0] * Matrix[7]) - (Matrix[1] * Matrix[6]);
		Temp[8] = (Matrix[0] * Matrix[4]) - (Matrix[3] * Matrix[1]);
		//We can also get the determinant easily, by multiplying the first column to the "Temp", since "Temp" is made by 2x2 matrix determinants
		//Then our Determinant will be: m[0] * Temp[0] - m[1] * Temp[1] + m[2] * Temp[2]
		type InvDet = Matrix[0] * Temp[0] - Matrix[1] * Temp[1] + Matrix[2] * Temp[2];
		if (InvDet == 0)	//Failsafe against x / 0
		{
			return Identity();
		}
		InvDet = 1 / InvDet;
		Matrix[0] = Temp[0] * InvDet;
		Matrix[1] = -Temp[3] * InvDet;
		Matrix[2] = Temp[6] * InvDet;
		Matrix[3] = -Temp[1] * InvDet;
		Matrix[4] = Temp[4] * InvDet;
		Matrix[5] = -Temp[7] * InvDet;
		Matrix[6] = Temp[2] * InvDet;
		Matrix[7] = -Temp[5] * InvDet;
		Matrix[8] = Temp[8] * InvDet;
		return *this;
	}
	//Operators
	Mat3 operator-() const
	{
		return Mat3(-Matrix[0], -Matrix[1], -Matrix[2], 
					-Matrix[3], -Matrix[4], -Matrix[5], 
					-Matrix[6], -Matrix[7], -Matrix[8]);
	}
	Mat3 operator=(const Mat3 &m)
	{
		if (this != &m)
		{
			Matrix[0] = m[0];
			Matrix[1] = m[1];
			Matrix[2] = m[2];
			Matrix[3] = m[3];
			Matrix[4] = m[4];
			Matrix[5] = m[5];
			Matrix[6] = m[6];
			Matrix[7] = m[7];
			Matrix[8] = m[8];
		}
		return *this;
	}
	Mat3 operator+(const Mat3 &m) const
	{
		return Mat3(Matrix[0] + m[0], Matrix[1] + m[1], Matrix[2] + m[2],
					Matrix[3] + m[3], Matrix[4] + m[4], Matrix[5] + m[5],
					Matrix[6] + m[6], Matrix[7] + m[7], Matrix[8] + m[8]);
	}
	Mat3 & operator+=(const Mat3 &m)
	{
		Matrix[0] += m[0];
		Matrix[1] += m[1];
		Matrix[2] += m[2];
		Matrix[3] += m[3];
		Matrix[4] += m[4];
		Matrix[5] += m[5];
		Matrix[6] += m[6];
		Matrix[7] += m[7];
		Matrix[8] += m[8];
		return *this;
	}
	Mat3 operator-(const Mat3 &m) const
	{
		return Mat3(Matrix[0] - m[0], Matrix[1] - m[1], Matrix[2] - m[2], 
					Matrix[3] - m[3], Matrix[4] - m[4], Matrix[5] - m[5], 
					Matrix[6] - m[6], Matrix[7] - m[7], Matrix[8] - m[8]);
	}
	Mat3 & operator-=(const Mat3 &m)
	{
		Matrix[0] -= m[0];
		Matrix[1] -= m[1];
		Matrix[2] -= m[2];
		Matrix[3] -= m[3];
		Matrix[4] -= m[4];
		Matrix[5] -= m[5];
		Matrix[6] -= m[6];
		Matrix[7] -= m[7];
		Matrix[8] -= m[8];
		return *this;
	}
	Mat3 operator*(const Mat3 &m) const
	{
		return Mat3((Matrix[0] * m[0]) + (Matrix[1] * m[3]) + (Matrix[2] * m[6]), (Matrix[0] * m[1]) + (Matrix[1] * m[4]) + (Matrix[2] * m[7]), (Matrix[0] * m[2]) + (Matrix[1] * m[5]) + (Matrix[2] * m[8]),
					(Matrix[3] * m[0]) + (Matrix[4] * m[3]) + (Matrix[5] * m[6]), (Matrix[3] * m[1]) + (Matrix[4] * m[4]) + (Matrix[5] * m[7]), (Matrix[3] * m[2]) + (Matrix[4] * m[5]) + (Matrix[5] * m[8]),
					(Matrix[6] * m[0]) + (Matrix[7] * m[3]) + (Matrix[8] * m[6]), (Matrix[6] * m[1]) + (Matrix[7] * m[4]) + (Matrix[8] * m[7]), (Matrix[6] * m[2]) + (Matrix[7] * m[5]) + (Matrix[8] * m[8]));
	}
	Mat3 & operator*=(const Mat3 &m)
	{
		//The pointer to this (the Matrix m) = the returned pointer from m * m
		*this = *this * m;
		return *this;
	}
	Vec3<type> operator*(const Vec3<type> &v) const		//As a property of matrices: You can do Mat3 * Vec3, but not Vec3 * Mat3
	{
		return Vec3<type>((Matrix[0] * v._X) + (Matrix[1] * v._Y) + (Matrix[2] * v._Z), 
						(Matrix[3] * v._X) + (Matrix[4] * v._Y) + (Matrix[5] * v._Z), 
						(Matrix[6] * v._X) + (Matrix[7] * v._Y) + (Matrix[8] * v._Z));
	}
	bool operator==(const Mat3 &m) const
	{
		return ((Matrix[0] == m[0]) && (Matrix[1] == m[1]) && (Matrix[2] == m[2]) && 
				(Matrix[3] == m[3]) && (Matrix[4] == m[4]) && (Matrix[5] == m[5]) && 
				(Matrix[6] == m[6]) && (Matrix[7] == m[7]) && (Matrix[8] == m[8]));
	}
	bool operator!=(const Mat3 &m) const
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
	void setMatrix(type Values[9])
	{
		Matrix[0] = Values[0];
		Matrix[1] = Values[1];
		Matrix[2] = Values[2];
		Matrix[3] = Values[3];
		Matrix[4] = Values[4];
		Matrix[5] = Values[5];
		Matrix[6] = Values[6];
		Matrix[7] = Values[7];
		Matrix[8] = Values[8];
	}
	void setMatrix(type m11, type m12, type m13, type m21, type m22, type m23, type m31, type m32, type m33)
	{
		Matrix[0] = m11;
		Matrix[1] = m12;
		Matrix[2] = m13;
		Matrix[3] = m21;
		Matrix[4] = m22;
		Matrix[5] = m23;
		Matrix[6] = m31;
		Matrix[7] = m32;
		Matrix[8] = m33;
	}
	void setRow(int Index, type RowData[3])
	{
		Matrix[Index] = RowData[0];
		Matrix[Index + 3] = RowData[1];
		Matrix[Index + 6] = RowData[2];
	}
	void setRow(int Index, Vec3<type> &VectorData)
	{
		Matrix[Index] = VectorData._X;
		Matrix[Index + 3] = VectorData._Y;
		Matrix[Index + 6] = VectorData._Z;
	}
	void setColumn(int Index, type ColumnData[3])
	{
		Matrix[Index * 3] = ColumnData[0];
		Matrix[(Index * 3) + 1] = ColumnData[1];
		Matrix[(Index * 3) + 2] = ColumnData[2];
	}
	void setColumn(int Index, Vec3<type> &VectorData)
	{
		Matrix[Index * 3] = VectorData._X;
		Matrix[(Index * 3) + 1] = VectorData._Y;
		Matrix[(Index * 3) + 2] = VectorData._Z;
	}
	type *getMatrix()
	{
		return Matrix;
	}
	type getDeterminant()
	{
		//Reference 3x3 matrix. The signal are only for the equation (i - j + k)
		//In this method, we will pick the determinant of a 2x2 matrix and multiply it for the reference. The reference will draw a cross, exculding the others 5 values thus forming the 2x2 matrix
		//		[+m11  -m21	+m31]
		//		[-m12  +m22	-m32]
		//		[+m13  -m23 +m33]
		//The Determinant will be i - j + k. In our case, we will pick m[0], m[1] and m[2] (faster), so:
		//i = m11 * (m22*m33 - m32*m23)
		//j = m12 * (m21*m33 - m31*m23)
		//k = m13 * (m21*m32 - m31*m22)
		//Which implies that Det = (m11 * (m22*m33 - m32*m23)) - (m12 * (m21*m33 - m31*m23)) + (m13 * (m21*m32 - m31*m22))
		return (Matrix[0] * (Matrix[4] * Matrix[8] - Matrix[7] * Matrix[5])) -
			   (Matrix[1] * (Matrix[3] * Matrix[8] - Matrix[6] * Matrix[5])) +
			   (Matrix[2] * (Matrix[3] * Matrix[7] - Matrix[6] * Matrix[4]));
	}

#ifdef _INC_UtilsIO
	void print(void)
	{
		Utils::PrintArray(Matrix, 9, 3, "[%d]");
	}
#endif
};

#endif