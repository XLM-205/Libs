/*-------4D Vector representation-------
*	--4D template Vector representation and operators
*		--Requires "Vec3d.h" library for extra functionality (Extra constructor)
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

#ifndef _MOON_PHYSICS_VEC4_H_
#define _MOON_PHYSICS_VEC4_H_

#include "Vec3d.h"

#ifndef HIDE_ALL_VEC					//Hides all the vectors macros
	#ifndef HIDE_VEC4					//Hide this vector's macros, to prevent polluting the IDE

		#define mwVec4i Vec4<int>		//4D INT Vector, without length (magnitude)
		#define mwVec4f Vec4<float>		//4D FLOAT Vector, without length (magnitude)
		#define mwVec4d Vec4<double>	//4D DOUBLE Vector, without length (magnitude)

		#define mwVec4li Vec4l<int>		//4D INT Vector, with length (magnitude)
		#define mwVec4lf Vec4l<float>	//4D FLOAT Vector, with length (magnitude)
		#define mwVec4ld Vec4l<double>	//4D DOUBLE Vector, with length (magnitude)

	#endif
#endif

template <class type>
class Vec4
{
public:
	type _X, _Y, _Z, _W;
	Vec4() : _X(0), _Y(0), _Z(0), _W(0) { };
	Vec4(Vec2<type> &v) : _X(v._X), _Y(v._Y), _Z(0), _W(0) { };
	Vec4(Vec3<type> &v) : _X(v._X), _Y(v._Y), _Z(v._Z), _W(0) { };
	Vec4(type X, type Y, type Z, type W) : _X(X), _Y(Y), _Z(Z), _W(W) { };

	//Basic Operators ( + | - | * | / )
	Vec4 operator=(const type k)
	{
		_X = k;
		_Y = k;
		_Z = k;
		_W = k;
		return *this;
	}
	Vec4 operator=(const Vec4 &v)
	{
		if (this != &v)
		{
			_X = v._X;
			_Y = v._Y;
			_Z = v._Z;
			_W = v._W;
		}
		return *this;
	}
	Vec4 operator--(const int k)
	{
		_X--;
		_Y--;
		_Z--;
		_W--;
		return *this;
	}
	Vec4 operator++(const int k)
	{
		_X++;
		_Y++;
		_Z++;
		_W++;
		return *this;
	}
	Vec4 operator-=(const type k)
	{
		_X -= k;
		_Y -= k;
		_Z -= k;
		_W -= k;
		return *this;
	}
	Vec4 operator+=(const type k)
	{
		_X += k;
		_Y += k;
		_Z += k;
		_W += k;
		return *this;
	}
	bool operator==(const type k) const
	{
		return (_X == k, _Y == k, _Z == k, _W == k);
	}
	bool operator!=(const type k) const
	{
		return !(*this == k);
	}
	bool operator<(const type k) const
	{
		return (_X < k, _Y < k, _Z < k, _W < k);
	}
	bool operator<=(const type k) const
	{
		return (_X <= k, _Y <= k, _Z <= k, _W <= 4);
	}
	bool operator>(const type k) const
	{
		return (_X > k, _Y > k, _Z > k, _W > k);
	}
	bool operator>=(const type k) const
	{
		return (_X >= k, _Y >= k, _Z >= k, _W >= k);
	}
	bool operator==(const Vec4 &v) const
	{
		return (_X == v._X, _Y == v._Y, _Z == v._Z, _W == v._W);
	}
	bool operator!=(const Vec4 &v) const
	{
		return !(*this == v);
	}
	bool operator<(const Vec4 &v) const
	{
		return (_X < v._X && _Y < v._Y && _Z < v._Z && _W < v._W);
	}
	bool operator<=(const Vec4 &v) const
	{
		return (_X <= v._X && _Y <= v._Y && _Z <= v._Z && _W <= v._W);
	}
	bool operator>(const Vec4 &v) const
	{
		return (_X > v._X && _Y > v._Y && _Z > v._Z && _W > v._W);
	}
	bool operator>=(const Vec4 &v) const
	{
		return (_X >= v._X && _Y >= v._Y && _Z >= v._Z && _W >= v._W);
	}
	Vec4 operator+(const Vec4 &v) const
	{
		return Vec4(_X + v._X, _Y + v._Y, _Z + v._Z, _W + v._W);
	}
	Vec4 & operator+=(const Vec4 &v)
	{
		_X += v._X;
		_Y += v._Y;
		_Z += v._Z;
		_W += v._W;
		return *this;
	}
	Vec4 operator-(const Vec4 &v) const
	{
		return Vec4(_X - v._X, _Y - v._Y, _Z - v._Z, this->_W - v._W);
	}
	Vec4 & operator-=(const Vec4 &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		_Z -= v._Z;
		_W -= v._W;
		return *this;
	}
	Vec4 operator-() const		//Negates value (not logicaly)
	{
		return Vec4(-_X, -_Y, -_Z, -_W);
	}
	//Scalar Multiplication
	Vec4 operator*(const type k) const
	{
		return Vec4(_X * k, _Y * k, _Z * k), _W * k;
	}
	Vec4 & operator*=(const type k)
	{
		_X *= k;
		_Y *= k;
		_Z *= k;
		_W *= k;
		return *this;
	}
	//Scalar Division
	Vec4 operator/(const type k) const
	{
		return Vec4(_X / k, _Y / k, _Z / k, _W / k);
	}
	Vec4 & operator/=(const type k)
	{
		_X /= k;
		_Y /= k;
		_Z /= k;
		_W /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec4 &v)
	{
		return ((_X * v._X) + (_Y * v._Y) + (_Z * v._Z) + (_W * v._W));
	}
	//Side note: There are no Cross Product for 4D vectors
	virtual double getLength(void)
	{
		return sqrt((_X * _X) + (_Y * _Y) + (_Z * _Z) + (_W * _W));
	}
	//Return the vector as an array
	type *returnAsArray(void)
	{
		type TempArray[4] = { _X, _Y, _Z, _W};
		return TempArray;
	}
	template <typename T, typename T2, typename T3, typename T4>
	void setValues(T X, T2 Y, T3 Z, T4 W)
	{
		_X = X;
		_Y = Y;
		_Z = Z;
		_W = W;
	}

	//Multi input type conversion

	template <typename type2>
	Vec4 operator=(const Vec4<type2> &v)
	{
		_X = v._X;
		_Y = v._Y;
		_Z = v._Z;
		_W = v._W;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec4<type2> &v) const
	{
		return (_X == v._X, _Y == v._Y, _Z == v._Z, _W == v._W);
	}
	template <typename type2>
	bool operator!=(const Vec4<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec4 operator+(const Vec4<type2> &v) const
	{
		return Vec4(_X + v._X, _Y + v._Y, _Z + v._Z, _W + v._W);
	}
	template <typename type2>
	Vec4 & operator+=(const Vec4<type2> &v)
	{
		_X += v._X;
		_Y += v._Y;
		_Z += v._Z;
		_W += v._W;
		return *this;
	}
	template <typename type2>
	Vec4 operator-(const Vec4<type2> &v) const
	{
		return Vec4(_X - v._X, _Y - v._Y, _Z - v._Z, _W - v._W);
	}
	template <typename type2>
	Vec4 & operator-=(const Vec4<type2> &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		_Z -= v._Z;
		_W -= v._W;
		return *this;
	}

	//Statics

	static Vec4<type> zero(void)
	{
		return Vec4(0, 0, 0, 0);
	}
};

template <class type>
class Vec4l : public Vec4<type>
{
protected:
	double length = 0;
public:
	Vec4l()
	{
		this->_X = 0;
		this->_Y = 0;
		this->_Z = 0;
		this->_W = 0;
	}
	Vec4l(Vec2<type> &v)
	{
		this->_X = v.X;
		this->_Y = v.Y;
		this->_Z = 0;
		this->_W = 0;
	}
	Vec4l(Vec3<type> &v)
	{
		this->_X = v.X;
		this->_Y = v.Y;
		this->_Z = v.Z;
		this->_W = 0;
	}
	Vec4l(Vec2l<type> &v)
	{
		this->_X = v.X;
		this->_Y = v.Y;
		this->_Z = 0;
		this->_W = 0;
	}
	Vec4l(Vec3l<type> &v)
	{
		this->_X = v.X;
		this->_Y = v.Y;
		this->_Z = v.Z;
		this->_W = 0;
	}
	Vec4l(type X, type Y, type Z, type W)
	{
		this->_X = X;
		this->_Y = Y;
		this->_Z = Z;
		this->_W = W;
	}

	Vec4l operator=(const type k)
	{
		this->_X = k;
		this->_Y = k;
		this->_Z = k;
		this->_W = k;
		return *this;
	}
	Vec4l operator=(const Vec4l &v)
	{
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
			this->_Z = v._Z;
			this->_W = v._W;
		}
		return *this;
	}
	Vec4l operator=(const Vec4<type> &v)
	{
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
			this->_Z = v._Z;
			this->_W = v._W;
		}
		return *this;
	}
	Vec4l operator--(const int k)
	{
		this->_X--;
		this->_Y--;
		this->_Z--;
		this->_W--;
		return *this;
	}
	Vec4l operator++(const int k)
	{
		this->_X++;
		this->_Y++;
		this->_Z++;
		this->_W++;
		return *this;
	}
	Vec4l operator-=(const type k)
	{
		this->_X -= k;
		this->_Y -= k;
		this->_Z -= k;
		this->_W -= k;
		return *this;
	}
	Vec4l operator+=(const type k)
	{
		this->_X += k;
		this->_Y += k;
		this->_Z += k;
		this->_W += k;
		return *this;
	}
	bool operator==(const Vec4l &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y, this->_Z == v._Z, this->_W == v._W);
	}
	bool operator!=(const Vec4l &v) const
	{
		return !(*this == v);
	}
	Vec4l operator+(const Vec4l &v) const
	{
		return Vec4l(this->_X + v._X, this->_Y + v._Y, this->_Z + v._Z, this->_W + v._W);
	}
	Vec4l & operator+=(const Vec4l &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		this->_Z += v._Z;
		this->_W += v._W;
		return *this;
	}
	Vec4l operator-(const Vec4l &v) const
	{
		return Vec4l(this->_X - v._X, this->_Y - v._Y, this->_Z - v._Z, this->_W - v._W);
	}
	Vec4l & operator-=(const Vec4l &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		this->_Z -= v._Z;
		this->_W -= v._W;
		return *this;
	}
	Vec4l operator-() const		//Negates value (not logicaly)
	{
		return Vec4l(-this->_X, -this->_Y, -this->_Z, -this->_W);
	}
	//Scalar Multiplication
	Vec4l operator*(const type k) const
	{
		return Vec4l(this->_X * k, this->_Y * k, this->_Z * k), this->_W * k;
	}
	Vec4l & operator*=(const type k)
	{
		this->_X *= k;
		this->_Y *= k;
		this->_Z *= k;
		this->_W *= k;
		return *this;
	}
	//Scalar Division
	Vec4l operator/(const type k) const
	{
		return Vec4l(this->_X / k, this->_Y / k, this->_Z / k, this->_W / k);
	}
	Vec4l & operator/=(const type k)
	{
		this->_X /= k;
		this->_Y /= k;
		this->_Z /= k;
		this->_W /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec4l &v)
	{
		return ((this->_X * v._X) + (this->_Y * v._Y) + (this->_Z * v._Z) + (this->_W * v._W));
	}
	//Side note: There are no Cross Product for 4D vectors

	Vec4<type> returnAsVec4(void)
	{
		return Vec4<type>(this->_X, this->_Y, this->_Z, this->_W);
	}

	//Multi input type conversion

	template <typename type2>
	Vec4l operator=(const Vec4l<type2> &v)
	{
		this->_X = v._X;
		this->_Y = v._Y;
		this->_Z = v._Z;
		this->_W = v._W;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec4l<type2> &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y, this->_Z == v._Z, this->_W == v._W);
	}
	template <typename type2>
	bool operator!=(const Vec4l<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec4l operator+(const Vec4l<type2> &v) const
	{
		return Vec4l(this->_X + v._X, this->_Y + v._Y, this->_Z + v._Z, this->_W + v._W);
	}
	template <typename type2>
	Vec4l & operator+=(const Vec4l<type2> &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		this->_Z += v._Z;
		this->_W += v._W;
		return *this;
	}
	template <typename type2>
	Vec4l operator-(const Vec4l<type2> &v) const
	{
		return Vec4l(this->_X - v._X, this->_Y - v._Y, this->_Z - v._Z, this->_W - v._W);
	}
	template <typename type2>
	Vec4l & operator-=(const Vec4l<type2> &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		this->_Z -= v._Z;
		this->_W -= v._W;
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
		length = sqrt((this->_X * this->_X) + (this->_Y * this->_Y) + (this->_Z * this->_Z) + (this->_W * this->_W));
		return length;
	}
	//Normalize vector (Only Return)
	Vec4l getNormalize(void)
	{
		getLength();
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return a NULL vector
		{
			return Vec4l(0, 0, 0, 0);;
		}
		return Vec4l(this->_X / length, this->_Y / length, this->_Z / length, this->_W / length);
	}
	//Normalize vector(Overwrite Values)
	Vec4l setNormalize(void)
	{
		getLength();
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return itself unchanged
		{
			return *this;
		}
		this->_X = this->_X / length;
		this->_Y = this->_Y / length;
		this->_Z = this->_Z / length;
		this->_W = this->_W / length;
		return *this;
	}

	//Statics

	static Vec4l<type> zero(void)
	{
		return Vec4l(0, 0, 0, 0);
	}
};

#endif;