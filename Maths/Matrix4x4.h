/*-------4x4 Matrix representation (WIP)-------
*	--4x4 Template Matrix representation and operators
*		--Require "Vec4d.h" library for extra functionality (Row and Column fill)
*		--Require "Matrix2x2.h" library for core transformations
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
*	Version 0.5.1
*/

#ifndef _MOON_MATH_MAT4_H_
#define _MOON_MATH_MAT4_H_

#include "Matrix2x2.h"	//Mat2 Definitions
#include "Vec4d.h"		//Contains Vec2, Vec3 and Vec4 definitions

#ifndef HIDE_ALL_MAT					//Hides all the matrices macros
	#ifndef HIDE_MAT4					//Hide this matrix's macros, to prevent polluting the IDE

		#define mwMat4i Mat4<int>		//4x4 INT Matrix
		#define mwMat4f Mat4<float>		//4x4 FLOAT Matrix
		#define mwMat4d Mat4<double>	//4x4 DOUBLE Matrix

	#endif
#endif

//Index Reference
// 0 > m11	| 4 > m21	| 8 > m31	| 12> m41
// 1 > m12	| 5 > m22	| 9 > m32	| 13> m42
// 2 > m13	| 6 > m23	| 10> m33	| 14> m43
// 3 > m14	| 7 > m24	| 11> m34	| 15> m44

//Pattern: column-major (Values filled from the first column to the others)
template <class type>
class Mat4
{
private:
	type Matrix[16];

	//A cofactor is a 3x3 inside the main 4x4 matrix. When computing a determinant, for example, we will have 4 cofactors
	//It's formula is the same used to get the determinant of a 3x3 matrix (which, by the way, also use a cofactor)
	type getCofactor(type m11, type m12, type m13, type m21, type m22, type m23, type m31, type m32, type m33)
	{
		return (m11 * (m22 * m33 - m32 * m23)) -
			   (m12 * (m21 * m33 - m31 * m23)) +
			   (m13 * (m21 * m32 - m31 * m22));
	}
public:
	Mat4() { Identity(); };			//If no parameter given, then make the matrix an identity
	Mat4(type Number)				//Fill the matrix with only one number
	{
		setMatrix(Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number, Number);
	}
	Mat4(type Values[16])
	{
		setMatrix(Values);
	}
	Mat4(type m11, type m12, type m13, type m14, type m21, type m22, type m23, type m24, type m31, type m32, type m33, type m34, type m41, type m42, type m43, type m44)
	{
		setMatrix(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
	}

	Mat4 &Identity()
	{
		Matrix[0] = Matrix[5] = Matrix[10] = Matrix[15] = 1;
		Matrix[1] = Matrix[2] = Matrix[3] = Matrix[4] = Matrix[6] = Matrix[7] = Matrix[8] = Matrix[9] = Matrix[11] = Matrix[12] = Matrix[13] = Matrix[14] = 0;
		return *this;
	}	//Test
	Mat4 &Transpose()
	{
		Utils::Swap(Matrix[1], Matrix[4]);
		Utils::Swap(Matrix[2], Matrix[8]);
		Utils::Swap(Matrix[12], Matrix[3]);
		Utils::Swap(Matrix[6], Matrix[9]);
		Utils::Swap(Matrix[7], Matrix[13]);
		Utils::Swap(Matrix[11], Matrix[14]);
		return *this;
	}
	Mat4 &Invert()	//TODO: Fix this to match a Mat4 matrix (currently it's a Mat3)
	{
		//Reference 4x4 matrix. The signal are only for the equation (i - j + k)
		//		[+m11  -m21	 +m31  -m41]
		//		[-m12  +m22	 -m32  +m42]
		//		[+m13  -m23  +m33  -m43]
		//		[-m14  +m24  -m34  +m44]
		Mat4<type> Temp;
		Temp[0]		= (Matrix[4] * Matrix[8]) - (Matrix[5] * Matrix[7]);
		Temp[4]		= (Matrix[0] * Matrix[8]) - (Matrix[6] * Matrix[2]);
		Temp[8]		= (Matrix[0] * Matrix[4]) - (Matrix[3] * Matrix[1]);
		//Temp[12]	= ()

		Temp[1] = (Matrix[3] * Matrix[8]) - (Matrix[5] * Matrix[6]);
		Temp[5] = (Matrix[0] * Matrix[7]) - (Matrix[1] * Matrix[6]);
		//Temp[9] = ()
		//Temp[13]

		Temp[2] = (Matrix[3] * Matrix[7]) - (Matrix[4] * Matrix[6]);
		Temp[6] = (Matrix[1] * Matrix[5]) - (Matrix[4] * Matrix[2]);
		//Temp[10]
		//Temp[14]

		Temp[3] = (Matrix[1] * Matrix[8]) - (Matrix[2] * Matrix[7]);
		Temp[7] = (Matrix[0] * Matrix[5]) - (Matrix[3] * Matrix[2]);
		//Temp[11]
		//Temp[15]

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
	Mat4 operator-() const
	{
		return Mat4(-Matrix[0], -Matrix[1], -Matrix[2], -Matrix[3],
					-Matrix[4], -Matrix[5], -Matrix[6], -Matrix[7],
					-Matrix[8], -Matrix[9], -Matrix[10], -Matrix[11],
					-Matrix[12], -Matrix[13], -Matrix[14], -Matrix[15]);
	}
	Mat4 operator=(const Mat4 &m)
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
			Matrix[9] = m[9];
			Matrix[10] = m[10];
			Matrix[11] = m[11];
			Matrix[12] = m[12];
			Matrix[13] = m[13];
			Matrix[14] = m[14];
			Matrix[15] = m[15];
		}
		return *this;
	}
	Mat4 operator+(const Mat4 &m) const
	{
		return Mat4(Matrix[0] + m[0], Matrix[1] + m[1], Matrix[2] + m[2], Matrix[3] + m[3],
					Matrix[4] + m[4], Matrix[5] + m[5], Matrix[6] + m[6], Matrix[7] + m[7],
					Matrix[8] + m[8], Matrix[9] + m[9], Matrix[10] + m[10], Matrix[11] + m[11],
					Matrix[12] + m[12], Matrix[13] + m[13], Matrix[14] + m[14], Matrix[15] + m[15]);
	}
	Mat4 & operator+=(const Mat4 &m)
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
		Matrix[9] += m[9];
		Matrix[10] += m[10];
		Matrix[11] += m[11];
		Matrix[12] += m[12];
		Matrix[13] += m[13];
		Matrix[14] += m[14];
		Matrix[15] += m[15];
		return *this;
	}
	Mat4 operator-(const Mat4 &m) const
	{
		return Mat4(Matrix[0] - m[0], Matrix[1] - m[1], Matrix[2] - m[2], Matrix[3] - m[3],
					Matrix[4] - m[4], Matrix[5] - m[5],	Matrix[6] - m[6], Matrix[7] - m[7],
					Matrix[8] - m[8], Matrix[9] - m[9], Matrix[10] - m[10], Matrix[11] - m[11],
					Matrix[12] - m[12], Matrix[13] - m[13], Matrix[14] - m[14], Matrix[15] - m[15]);
	}
	Mat4 & operator-=(const Mat4 &m)
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
		Matrix[9] -= m[9];
		Matrix[10] -= m[10];
		Matrix[11] -= m[11];
		Matrix[12] -= m[12];
		Matrix[13] -= m[13];
		Matrix[14] -= m[14];
		Matrix[15] -= m[15];
		return *this;
	}
	Mat4 operator*(const Mat4 &m) const
	{
		return Mat4(Matrix[0] * m[0] + Matrix[1] * m[4] + Matrix[2] * m[8] + Matrix[3] * m[12], Matrix[0] * m[1] + Matrix[1] * m[5] + Matrix[2] * m[9] + Matrix[3] * m[13], Matrix[0] * m[2] + Matrix[1] * m[6] + Matrix[2] * m[10] + Matrix[3] * m[14], Matrix[0] * m[3] + Matrix[1] * m[7] + Matrix[2] * m[11] + Matrix[3] * m[15],
					Matrix[4] * m[0] + Matrix[5] * m[4] + Matrix[6] * m[8] + Matrix[7] * m[12], Matrix[4] * m[1] + Matrix[5] * m[5] + Matrix[6] * m[9] + Matrix[7] * m[13], Matrix[4] * m[2] + Matrix[5] * m[6] + Matrix[6] * m[10] + Matrix[7] * m[14], Matrix[4] * m[3] + Matrix[5] * m[7] + Matrix[6] * m[11] + Matrix[7] * m[15],
					Matrix[8] * m[0] + Matrix[9] * m[4] + Matrix[10] * m[8] + Matrix[11] * m[12], Matrix[8] * m[1] + Matrix[9] * m[5] + Matrix[10] * m[9] + Matrix[11] * m[13], Matrix[8] * m[2] + Matrix[9] * m[6] + Matrix[10] * m[10] + Matrix[11] * m[14], Matrix[8] * m[3] + Matrix[9] * m[7] + Matrix[10] * m[11] + Matrix[11] * m[15],
					Matrix[12] * m[0] + Matrix[13] * m[4] + Matrix[14] * m[8] + Matrix[15] * m[12], Matrix[12] * m[1] + Matrix[13] * m[5] + Matrix[14] * m[9] + Matrix[15] * m[13], Matrix[12] * m[2] + Matrix[13] * m[6] + Matrix[14] * m[10] + Matrix[15] * m[14], Matrix[12] * m[3] + Matrix[13] * m[7] + Matrix[14] * m[11] + Matrix[15] * m[15]);
	}
	Mat4 & operator*=(const Mat4 &m)
	{
		//The pointer to this (the Matrix m) = the returned pointer from m * m
		*this = *this * m;
		return *this;
	}
	Vec4<type> operator*(const Vec4<type> &v) const		//As a property of matrices: You can do Mat4 * Vec4, but not Vec4 * Mat4
	{
		//Test if the operation is correct
		return Vec4<type>((Matrix[0] * v._X) + (Matrix[1] * v._Y) + (Matrix[2] * v._Z) + (Matrix[3] * v._W),
						  (Matrix[4] * v._X) + (Matrix[5] * v._Y) + (Matrix[6] * v._Z) + (Matrix[7] * v._W),
						  (Matrix[8] * v._X) + (Matrix[9] * v._Y) + (Matrix[10] * v._Z) + (Matrix[11] * v._W),
						  (Matrix[12] * v._X) + (Matrix[13] * v._Y) + (Matrix[14] * v._Z) + (Matrix[15] * v._W));
	}
	bool operator==(const Mat4 &m) const
	{
		return ((Matrix[0] == m[0]) && (Matrix[1] == m[1]) && (Matrix[2] == m[2]) && (Matrix[3] == m[3]) && 
				(Matrix[4] == m[4]) && (Matrix[5] == m[5]) && (Matrix[6] == m[6]) && (Matrix[7] == m[7]) &&
				(Matrix[8] == m[8]) && (Matrix[9] == m[9]) && (Matrix[10] == m[10]) && (Matrix[11] == m[11]) &&
				(Matrix[12] == m[12]) && (Matrix[13] == m[13]) && (Matrix[14] == m[14]) && (Matrix[15] == m[15]));
	}
	bool operator!=(const Mat4 &m) const
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
	void setMatrix(type Values[16])
	{
		Matrix[0]  = Values[0];
		Matrix[1]  = Values[1];
		Matrix[2]  = Values[2];
		Matrix[3]  = Values[3];
		Matrix[4]  = Values[4];
		Matrix[5]  = Values[5];
		Matrix[6]  = Values[6];
		Matrix[7]  = Values[7];
		Matrix[8]  = Values[8];
		Matrix[9]  = Values[9];
		Matrix[10] = Values[10];
		Matrix[11] = Values[11];
		Matrix[12] = Values[12];
		Matrix[13] = Values[13];
		Matrix[14] = Values[14];
		Matrix[15] = Values[15];
	}
	void setMatrix(type m11, type m12, type m13, type m14, type m21, type m22, type m23, type m24, type m31, type m32, type m33, type m34, type m41, type m42, type m43, type m44)
	{
		Matrix[0]  = m11;
		Matrix[1]  = m12;
		Matrix[2]  = m13;
		Matrix[3]  = m14;
		Matrix[4]  = m21;
		Matrix[5]  = m22;
		Matrix[6]  = m23;
		Matrix[7]  = m24;
		Matrix[8]  = m31;
		Matrix[9]  = m32;
		Matrix[10] = m33;
		Matrix[11] = m34;
		Matrix[12] = m41;
		Matrix[13] = m42;
		Matrix[14] = m43;
		Matrix[15] = m44;
	}
	void setRow(int Index, type RowData[4])
	{
		Matrix[Index] = RowData[0];
		Matrix[Index + 4] = RowData[1];
		Matrix[Index + 8] = RowData[2];
		Matrix[Index + 12] = RowData[3];
	}
	void setRow(int Index, Vec3<type> &VectorData)
	{
		Matrix[Index] = VectorData._X;
		Matrix[Index + 4] = VectorData._Y;
		Matrix[Index + 8] = VectorData._Z;
	}
	void setRow(int Index, Vec4<type> &VectorData)
	{
		Matrix[Index] = VectorData._X;
		Matrix[Index + 4] = VectorData._Y;
		Matrix[Index + 8] = VectorData._Z;
		Matrix[Index + 12] = VectorData._W;
	}
	void setColumn(int Index, type ColumnData[4])
	{
		Matrix[Index * 4] = ColumnData[0];
		Matrix[(Index * 4) + 1] = ColumnData[1];
		Matrix[(Index * 4) + 2] = ColumnData[2];
		Matrix[(Index * 4) + 3] = ColumnData[3];
	}
	void setColumn(int Index, Vec3<type> &VectorData)
	{
		Matrix[Index * 4] = VectorData._X;
		Matrix[(Index * 4) + 1] = VectorData._Y;
		Matrix[(Index * 4) + 2] = VectorData._Z;
	}
	void setColumn(int Index, Vec4<type> &VectorData)
	{
		Matrix[Index * 4] = VectorData._X;
		Matrix[(Index * 4) + 1] = VectorData._Y;
		Matrix[(Index * 4) + 2] = VectorData._Z;
		Matrix[(Index * 4) + 3] = VectorData._W;
	}
	type getAt(unsigned int Row, unsigned int Column)
	{
		if (Row < 4)
		{
			if (Column < 4)
			{
				return Matrix[(Row * 4) + Column];
			}
			return Matrix[Row];
		}
		return 0;
	}
	type *getMatrix()
	{
		return Matrix;
	}
	type getDeterminant()
	{
		return (Matrix[0] * getCofactor(Matrix[5], Matrix[6], Matrix[7], Matrix[9], Matrix[10], Matrix[11], Matrix[13], Matrix[14], Matrix[15])) -
			   (Matrix[1] * getCofactor(Matrix[4], Matrix[6], Matrix[7], Matrix[8], Matrix[10], Matrix[11], Matrix[12], Matrix[14], Matrix[15])) +
			   (Matrix[2] * getCofactor(Matrix[4], Matrix[5], Matrix[7], Matrix[8], Matrix[9],  Matrix[11], Matrix[12], Matrix[13], Matrix[15])) -
			   (Matrix[3] * getCofactor(Matrix[4], Matrix[5], Matrix[6], Matrix[8], Matrix[9],  Matrix[10], Matrix[12], Matrix[13], Matrix[14]));
	}
	char* whoIs(int index)
	{
		switch (index)
		{
		case 0:
			return "m11";
			break;
		case 1:
			return "m12";
			break;
		case 2:
			return "m13";
			break;
		case 3:
			return "m14";
			break;
		case 4:
			return "m21";
			break;
		case 5:
			return "m22";
			break;
		case 6:
			return "m23";
			break;
		case 7:
			return "m24";
			break;
		case 8:
			return "m31";
			break;
		case 9:
			return "m32";
			break;
		case 10:
			return "m33";
			break;
		case 11:
			return "m34";
			break;
		case 12:
			return "m41";
			break;
		case 13:
			return "m42";
			break;
		case 14:
			return "m43";
			break;
		case 15:
			return "m44";
			break;
		default:
			return "???";
			break;
		}
	}
	char* whoIs(int Row, int Column)
	{
		return whoIs((Row * 4) + Column);
	}

#ifdef _INC_STDIO
	void print(void)
	{
		Utils::PrintArray(Matrix, 16, 4, "[%d]");
	}
#endif
};

#endif;