/*-------3D Vector representation-------
*	--3D template Vector representation and operators
*		--Requires "Vec2d.h" library for extra functionality (Extra constructor)
*
*	Moon Wiz Studios (c) - 03/06/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 1.6.0
*/

#ifndef _MOON_PHYSICS_VEC3_H_
#define _MOON_PHYSICS_VEC3_H_

#include "Vec2d.h"

#ifndef HIDE_ALL_VEC					//Hides all the vectors macros
	#ifndef HIDE_VEC3					//Hide this vector's macros, to prevent polluting the IDE

		#define mwVec3i Vec3<int>		//3D INT Vector, without length (magnitude)
		#define mwVec3f Vec3<float>		//3D FLOAT Vector, without length (magnitude)
		#define mwVec3d Vec3<double>	//3D DOUBLE Vector, without length (magnitude)

		#define mwVec3li Vec3l<int>		//3D INT Vector, with length (magnitude)
		#define mwVec3lf Vec3l<float>	//3D FLOAT Vector, with length (magnitude)
		#define mwVec3ld Vec3l<double>	//3D DOUBLE Vector, with length (magnitude)

	#endif
#endif

template <class type>
class Vec3
{
public:
	type _X, _Y, _Z;
	Vec3() : _X(0), _Y(0), _Z(0) { };
	Vec3(Vec2<type> &v) : _X(v._X), _Y(v._Y), _Z(0) { };
	Vec3(type X, type Y, type Z) : _X(X), _Y(Y), _Z(Z) { };

	//Basic Operators ( + | - | * | / )
	Vec3 operator=(const type k)	//Absolute equal: Makes the entire vector have the same 'k' value
	{
		_X = k;
		_Y = k;
		_Z = k;
		return *this;
	}
	Vec3 operator=(const Vec3 &v)
	{
		if (this != &v)
		{
			_X = v._X;
			_Y = v._Y;
			_Z = v._Z;
		}
		return *this;
	}
	Vec3 operator--(const int k)
	{
		_X--;
		_Y--;
		_Z--;
		return *this;
	}
	Vec3 operator++(const int k)
	{
		_X++;
		_Y++;
		_Z++;
		return *this;
	}
	Vec3 operator-=(const type k)
	{
		_X-= k;
		_Y-= k;
		_Z-= k;
		return *this;
	}
	Vec3 operator+=(const type k)
	{
		_X+= k;
		_Y+= k;
		_Z+= k;
		return *this;
	}
	bool operator==(const type k) const
	{
		return (_X == k, _Y == k, _Z == k);
	}
	bool operator!=(const type k) const
	{
		return !(*this == k);
	}
	bool operator<(const type k) const
	{
		return (_X < k, _Y < k, _Z < k);
	}
	bool operator<=(const type k) const
	{
		return (_X <= k, _Y <= k, _Z <= k);
	}
	bool operator>(const type k) const
	{
		return (_X > k, _Y > k, _Z > k);
	}
	bool operator>=(const type k) const
	{
		return (_X >= k, _Y >= k, _Z >= k);
	}
	bool operator==(const Vec3 &v) const
	{
		return (_X == v._X, _Y == v._Y, _Z == v._Z);
	}
	bool operator!=(const Vec3 &v) const
	{
		return !(*this == v);
	}
	bool operator<(const Vec3 &v) const
	{
		return (_X < v._X && _Y < v._Y && _Z < v._Z);
	}
	bool operator<=(const Vec3 &v) const
	{
		return (_X <= v._X && _Y <= v._Y && _Z <= v._Z);
	}
	bool operator>(const Vec3 &v) const
	{
		return (_X > v._X && _Y > v._Y && _Z > v._Z);
	}
	bool operator>=(const Vec3 &v) const
	{
		return (_X >= v._X && _Y >= v._Y && _Z >= v._Z);
	}
	Vec3 operator+(const Vec3 &v) const
	{
		return Vec3(_X + v._X, _Y + v._Y, _Z + v._Z);
	}
	Vec3 & operator+=(const Vec3 &v)
	{
		_X += v._X;
		_Y += v._Y;
		_Z += v._Z;
		return *this;
	}
	Vec3 operator-(const Vec3 &v) const
	{
		return Vec3(_X - v._X, _Y - v._Y, _Z - v._Z);
	}
	Vec3 & operator-=(const Vec3 &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		_Z -= v._Z;
		return *this;
	}
	Vec3 operator-() const		//Negates value (not logicaly)
	{
		return Vec3(-_X, -_Y, -_Z);
	}
	//Scalar Multiplication
	Vec3 operator*(const type k) const
	{
		return Vec3(_X * k, _Y * k, _Z * k);
	}
	Vec3 & operator*=(const type k)
	{
		_X *= k;
		_Y *= k;
		_Z *= k;
		return *this;
	}
	//Scalar Division
	Vec3 operator/(const type k) const
	{
		return Vec3(_X / k, _Y / k, _Z / k);
	}
	Vec3 & operator/=(const type k)
	{
		_X /= k;
		_Y /= k;
		_Z /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec3 &v)
	{
		return ((_X * v._X) + (_Y * v._Y) + (_Z * v._Z));
	}
	//Produto Vetorial (Cross Product)
	Vec3 CrossProduct(const Vec3<type> &u, const Vec3<type> &v)
	{
		//It's just a 3x3 Matrix calculation:
		//		[  i	 j		k  |  i		j ]
		//		[u._X  u._Y	  u._Z |u._X  u._Y]
		//		[v._X  v._Y   v._Z |v._X  v._Y]
		//Where i, j and k will be the new _X, _Y and _Z, respectively.
		//Then, i = (u._Y * v._Z) - (u._Z * v._Y) | j = (u._Z * v._X) - (u._X * v._Z) | k = (u._X * v._Y) - (u._Y * v._X)
		_X = (u._Y * v._Z) - (u._Z * v._Y);
		_Y = (u._Z * v._X) - (u._X * v._Z);
		_Z = (u._X * v._Y) - (u._Y * v._X);
		return *this;
	}
	//Length Calculation
	virtual double getLength(void)
	{
		return sqrt((_X * _X) + (_Y * _Y) + (_Z * _Z));
	}
	//Return the Vec3 as a Vec2 of X and Y (constructs vec2)
	Vec2<type> getXY(void)
	{
		return Vec2<type>(_X, _Y);
	}
	//Return the Vec3 as a Vec2 of X and Z (constructs vec2)
	Vec2<type> getXZ(void)
	{
		return Vec2<type>(_X, _Z);
	}
	//Return the Vec3 as a Vec2 of Y and Z (constructs vec2)
	Vec2<type> getYZ(void)
	{
		return Vec2<type>(_Y, _Z);
	}
	//Return vector as an array
	type *returnAsArray(void)
	{
		type TempArray[3] = { _X, _Y, _Z};
		return TempArray;
	}
	//Return the vector as a array with X and Y
	type *returnAsArrayXY(void)
	{
		type TempArray[2] = { _X, _Y};
		return TempArray;
	}
	//Return the vector as a array with X and Z
	type *returnAsArrayXZ(void)
	{
		type TempArray[2] = { _X, _Z};
		return TempArray;
	}
	//Return the vector as a array with Y and Z
	type *returnAsArrayYZ(void)
	{
		type TempArray[2] = {_Y, _Z};
		return TempArray;
	}
	template <typename T, typename T2, typename T3>
	void setValues(T X, T2 Y, T3 Z)
	{
		_X = X;
		_Y = Y;
		_Z = Z;
	}

	double distanceFrom(double X, double Y, double Z)
	{
		double x = _X - X;
		double y = _Y - Y;
		double z = _Z - Z;
		return sqrt((x * x) + (y * y) + (z * z));
	}
	template <typename type2>
	double distanceFrom(const Vec3<type2> &v)
	{
		double x = _X - v._X;
		double y = _Y - v._Y;
		double z = _Z - v._Z;
		return sqrt((x * x) + (y * y) + (z * z));
	}

	//Multi input type conversion

	template <typename type2>
	Vec3 operator=(const Vec3<type2> &v)
	{
		_X = v._X;
		_Y = v._Y;
		_Z = v._Z;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec3<type2> &v) const
	{
		return (_X == v._X, _Y == v._Y, _Z == v._Z);
	}
	template <typename type2>
	bool operator!=(const Vec3<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec3 operator+(const Vec3<type2> &v) const
	{
		return Vec3(_X + v._X, _Y + v._Y, _Z + v._Z);
	}
	template <typename type2>
	Vec3 & operator+=(const Vec3<type2> &v)
	{
		_X += v._X;
		_Y += v._Y;
		_Z += v._Z;
		return *this;
	}
	template <typename type2>
	Vec3 operator-(const Vec3<type2> &v) const
	{
		return Vec3(_X - v._X, _Y - v._Y, _Z - v._Z);
	}
	template <typename type2>
	Vec3 & operator-=(const Vec3<type2> &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		_Z -= v._Z;
		return *this;
	}
	template <typename type2>
	Vec3 CrossProduct(const Vec3<type2> &u, const Vec3 &v)
	{
		//It's just a 3x3 Matrix calculation:
		//		[  i	 j		k  |  i		j ]
		//		[u._X  u._Y	  u._Z |u._X  u._Y]
		//		[v._X  v._Y   v._Z |v._X  v._Y]
		//Where i, j and k will be the new _X, _Y and _Z, respectively.
		//Then, i = (u._Y * v._Z) - (u._Z * v._Y) | j = (u._Z * v._X) - (u._X * v._Z) | k = (u._X * v._Y) - (u._Y * v._X)
		_X = (u._Y * v._Z) - (u._Z * v._Y);
		_Y = (u._Z * v._X) - (u._X * v._Z);
		_Z = (u._X * v._Y) - (u._Y * v._X);
		return *this;
	}
	template <typename type2>
	Vec3 CrossProduct(const Vec3 &u, const Vec3<type2> &v)
	{
		//It's just a 3x3 Matrix calculation:
		//		[  i	 j		k  |  i		j ]
		//		[u._X  u._Y	  u._Z |u._X  u._Y]
		//		[v._X  v._Y   v._Z |v._X  v._Y]
		//Where i, j and k will be the new _X, _Y and _Z, respectively.
		//Then, i = (u._Y * v._Z) - (u._Z * v._Y) | j = (u._Z * v._X) - (u._X * v._Z) | k = (u._X * v._Y) - (u._Y * v._X)
		_X = (u._Y * v._Z) - (u._Z * v._Y);
		_Y = (u._Z * v._X) - (u._X * v._Z);
		_Z = (u._X * v._Y) - (u._Y * v._X);
		return *this;
	}
	template <typename type2>
	Vec3 CrossProduct(const Vec3<type2> &u, const Vec3<type2> &v)
	{
		//It's just a 3x3 Matrix calculation:
		//		[  i	 j		k  |  i		j ]
		//		[u._X  u._Y	  u._Z |u._X  u._Y]
		//		[v._X  v._Y   v._Z |v._X  v._Y]
		//Where i, j and k will be the new _X, _Y and _Z, respectively.
		//Then, i = (u._Y * v._Z) - (u._Z * v._Y) | j = (u._Z * v._X) - (u._X * v._Z) | k = (u._X * v._Y) - (u._Y * v._X)
		_X = (u._Y * v._Z) - (u._Z * v._Y);
		_Y = (u._Z * v._X) - (u._X * v._Z);
		_Z = (u._X * v._Y) - (u._Y * v._X);
		return *this;
	}

	//Statics

	static Vec3<type> zero(void)
	{
		return Vec3(0, 0, 0);
	}
};

template <class type>
class Vec3l : public Vec3 <type>
{
protected:
	double length = 0;					//The size of the vector. Only computed by the "Normalize" and "getLength" methods
public:
	Vec3l()
	{
		this->_X = 0;
		this->_Y = 0;
		this->_Z = 0;
	}
	Vec3l(Vec2<type> &v)
	{
		this->_X = v._X;
		this->_Y = v._Y;
		this->_Z = 0;
	}
	Vec3l(Vec2l<type> &v)
	{
		this->_X = v._X;
		this->_Y = v._Y;
		this->_Z = 0;
	}
	Vec3l(type X, type Y, type Z)
	{ 
		this->_X = X;
		this->_Y = Y;
		this->_Z = Z;
	}

	//Basic Operators ( + | - | * | / )
	Vec3l operator=(const type k)	//Absolute equal: Makes the entire vector have the same 'k' value
	{
		this->_X = k;
		this->_Y = k;
		this->_Z = k;
		return *this;
	}
	Vec3l operator=(const Vec3<type> &v)
	{
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
			this->_Z = v._Z;
		}
		return *this;
	}
	Vec3l operator=(const Vec3l &v)
	{
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
			this->_Z = v._Z;
		}
		return *this;
	}
	Vec3l operator--(const int k)
	{
		this->_X--;
		this->_Y--;
		this->_Z--;
		return *this;
	}
	Vec3l operator++(const int k)
	{
		this->_X++;
		this->_Y++;
		this->_Z++;
		return *this;
	}
	Vec3l operator-=(const type k)
	{
		this->_X -= k;
		this->_Y -= k;
		this->_Z -= k;
		return *this;
	}
	Vec3l operator+=(const type k)
	{
		this->_X += k;
		this->_Y += k;
		this->_Z += k;
		return *this;
	}
	bool operator==(const Vec3l &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y, this->_Z == v._Z);
	}
	bool operator!=(const Vec3l &v) const
	{
		return !(*this == v);
	}
	Vec3l operator+(const Vec3l &v) const
	{
		return Vec3l(this->_X + v._X, this->_Y + v._Y, this->_Z + v._Z);
	}
	Vec3l & operator+=(const Vec3l &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		this->_Z += v._Z;
		return *this;
	}
	Vec3l operator-(const Vec3l &v) const
	{
		return Vec3l(this->_X - v._X, this->_Y - v._Y, this->_Z - v._Z);
	}
	Vec3l & operator-=(const Vec3l &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		this->_Z -= v._Z;
		return *this;
	}
	Vec3l operator-() const		//Negates value (not logicaly)
	{
		return Vec3l(-this->_X, -this->_Y, -this->_Z);
	}
	//Scalar Multiplication
	Vec3l operator*(const type k) const
	{
		return Vec3l(this->_X * k, this->_Y * k, this->_Z * k);
	}
	Vec3l & operator*=(const type k)
	{
		this->_X *= k;
		this->_Y *= k;
		this->_Z *= k;
		return *this;
	}
	//Scalar Division
	Vec3l operator/(const type k) const
	{
		return Vec3l(this->_X / k, this->_Y / k, this->_Z / k);
	}
	Vec3l & operator/=(const type k)
	{
		this->_X /= k;
		this->_Y /= k;
		this->_Z /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec3l &v)
	{
		return ((this->_X * v._X) + (this->_Y * v._Y) + (this->_Z * v._Z));
	}

	Vec3<type> returnAsVec3(void)
	{
		return Vec3<type>(this->_X, this->_Y, this->_Z);
	}

	//Multi input type conversion

	template <typename type2>
	Vec3l operator=(const Vec3l<type2> &v)
	{
		this->_X = v._X;
		this->_Y = v._Y;
		this->_Z = v._Z;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec3l<type2> &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y, this->_Z == v._Z);
	}
	template <typename type2>
	bool operator!=(const Vec3l<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec3l operator+(const Vec3l<type2> &v) const
	{
		return Vec3l(this->_X + v._X, this->_Y + v._Y, this->_Z + v._Z);
	}
	template <typename type2>
	Vec3l & operator+=(const Vec3l<type2> &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		this->_Z += v._Z;
		return *this;
	}
	template <typename type2>
	Vec3l operator-(const Vec3l<type2> &v) const
	{
		return Vec3l(this->_X - v._X, this->_Y - v._Y, this->_Z - v._Z);
	}
	template <typename type2>
	Vec3l & operator-=(const Vec3l<type2> &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		this->_Z -= v._Z;
		return *this;
	}

	//Get the vector length (magnitude). If the current length is 0, it will compute it, if not, only return it
	double getLength(void)
	{
		if (length == 0)
		{
			computeLength();
		}
		return length;
	}
	//Computes the vector length (magnitude)
	double computeLength(void)
	{
		length = sqrt((this->_X * this->_X) + (this->_Y * this->_Y) + (this->_Z * this->_Z));
		return length;
	}
	//Normalize vector (Only Return)
	Vec3l getNormalize(void)
	{
		getLength();
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return a NULL vector
		{
			return Vec3l(0, 0, 0);
		}
		return Vec3l(this->_X / length, this->_Y / length, this->_Z / length);
	}
	//Normalize vector(Overwrite Values)
	Vec3l setNormalize(void)
	{
		getLength();
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return itself unchanged
		{
			return *this;
		}
		this->_X = this->_X / length;
		this->_Y = this->_Y / length;
		this->_Z = this->_Z / length;
		return *this;
	}

	//Statics

	static Vec3l<type> zero(void)
	{
		return Vec3l(0, 0, 0);
	}
};

#endif;