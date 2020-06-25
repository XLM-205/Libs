/*-------2D Vector representation-------
*	--Stand-alone 2D template Vector representation and operators 
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

#ifndef _MOON_PHYSICS_VEC2_H_
#define _MOON_PHYSICS_VEC2_H_

#include "..\Control\CustomTypes-SA.h"
#include <math.h>

#ifndef HIDE_ALL_VEC					//Hides all the vectors macros
	#ifndef HIDE_VEC2					//Hide this vector's macros, to prevent polluting the IDE

		#define mwVec2i Vec2<int>		//2D INT Vector, without length (magnitude)
		#define mwVec2f Vec2<float>		//2D FLOAT Vector, without length (magnitude)
		#define mwVec2d Vec2<double>	//2D DOUBLE Vector, without length (magnitude)

		#define mwVec2li Vec2l<int>		//2D INT Vector, with length (magnitude)
		#define mwVec2lf Vec2l<float>	//2D FLOAT Vector, with length (magnitude)
		#define mwVec2ld Vec2l<double>	//2D DOUBLE Vector, with length (magnitude)

	#endif
#endif

//template <int>		//Formas padrão para funções.
//template <float>		//Isso permite que a mesma classe funcione com modos diferentes.
//template <double>		//Nesse caso, a classe terá suporte para int, float e doubles.

template <class type>
class Vec2
{
public:
	type _X, _Y;
	Vec2() : _X(0), _Y(0) {};
	Vec2(type X, type Y) : _X(X), _Y(Y) {};

	//Basic Operators ( + | - | * | / )
	Vec2 operator=(const type k)
	{
		_X = k;
		_Y = k;
		return *this;
	}
	Vec2 operator=(const Vec2 &v)
	{
		//To prevent future errors (where, when reading, the lhs will deallocate, but the rhs is the lhs (same adress), generating errors and possible memory leaks)
		if (this != &v)
		{
			_X = v._X;
			_Y = v._Y;
		}
		return *this;
	}
	Vec2 operator--(const int k)
	{
		_X--;
		_Y--;
		return *this;
	}
	Vec2 operator++(const int k)
	{
		_X++;
		_Y++;
		return *this;
	}
	Vec2 operator-=(const type k)
	{
		_X -= k;
		_Y -= k;
		return *this;
	}
	Vec2 operator+=(const type k)
	{
		_X += k;
		_Y += k;
		return *this;
	}
	bool operator==(const type k) const
	{
		return (_X == k, _Y == k);
	}
	bool operator!=(const type k) const
	{
		return !(*this == k);
	}
	bool operator<(const type k) const
	{
		return (_X < k, _Y < k);
	}
	bool operator<=(const type k) const
	{
		return (_X <= k, _Y <= k);
	}
	bool operator>(const type k) const
	{
		return (_X > k, _Y > k);
	}
	bool operator>=(const type k) const
	{
		return (_X >= k, _Y >= k);
	}
	bool operator==(const Vec2 &v) const
	{
		return (_X == v._X, _Y == v._Y);
	}
	bool operator!=(const Vec2 &v) const
	{
		return !(*this == v);
	}
	Vec2 operator+(const Vec2 &v) const
	{
		return Vec2(_X + v._X, _Y + v._Y);
	}
	Vec2 & operator+=(const Vec2 &v)
	{
		_X += v._X;
		_Y += v._Y;
		return *this;
	}
	Vec2 operator-(const Vec2 &v) const
	{
		return Vec2(_X - v._X, _Y - v._Y);
	}
	Vec2 & operator-=(const Vec2 &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		return *this;
	}
	Vec2 operator-() const		//Negates value (not logicaly)
	{
		return Vec2(-_X, -_Y);
	}
	//Scalar Multiplication
	Vec2 operator*(const type k) const
	{
		return Vec2(_X * k, _Y * k);
	}
	Vec2 & operator*=(const type k)
	{
		_X *= k;
		_Y *= k;
		return *this;
	}
	//Scalar division
	Vec2 operator/(const type k) const
	{
		return Vec2(_X / k, _Y / k);
	}
	Vec2 & operator/=(const type k)
	{
		_X /= k;
		_Y /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec2 &v)
	{
		return ((_X * v._X) + (_Y * v._Y));
	}
	//Length Calculation
	virtual double getLength(void)
	{
		return sqrt((_X * _X) + (_Y * _Y));
	}
	//Return vector as an array
	type *returnAsArray(void)
	{
		type TempArray[2] = { _X, _Y };
		return TempArray;
	}
	template <typename T, typename T2>
	void setValues(T X, T2 Y)
	{
		_X = X;
		_Y = Y;
	}

	//Multi input type conversion

	template <typename type2>
	Vec2 operator=(const Vec2<type2> &v)
	{
		_X = v._X;
		_Y = v._Y;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec2<type2> &v) const
	{
		return (_X == v._X, _Y == v._Y);
	}
	template <typename type2>
	bool operator!=(const Vec2<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec2 operator+(const Vec2<type2> &v) const
	{
		return Vec2(_X + v._X, _Y + v._Y);
	}
	template <typename type2>
	Vec2 & operator+=(const Vec2<type2> &v)
	{
		_X += v._X;
		_Y += v._Y;
		return *this;
	}
	template <typename type2>
	Vec2 operator-(const Vec2<type2> &v) const
	{
		return Vec2(_X - v._X, _Y - v._Y);
	}
	template <typename type2>
	Vec2 & operator-=(const Vec2<type2> &v)
	{
		_X -= v._X;
		_Y -= v._Y;
		return *this;
	}

	Vec2<double> centralizeOnScreen(const uint16 Width, const uint16 Heigth, const double OrthoTotal)
	{
		mwVec2d TheoricCenter(((float)_X / Width) - 0.5, ((float)_Y / Heigth) - 0.5);
		return mwVec2d(TheoricCenter._X * OrthoTotal, -TheoricCenter._Y * OrthoTotal);
	}

	//MUST BY SUPPLIED WITH THE PRODUCT OF 'Ortho * Ratio'. Made this way to increase performance
	Vec2<double> centralizeOnScreenRatio(const uint16 Width, const uint16 Heigth, const float OrthoTotal, const float OrthoXRatio)
	{
		mwVec2d TheoricCenter(((float)_X / Width) - 0.5, ((float)_Y / Heigth) - 0.5);
		return mwVec2d(TheoricCenter._X * OrthoXRatio, -TheoricCenter._Y * OrthoTotal);
	}

	//Statics

	static Vec2<float> centralizeOnScreen(const Vec2<type> &MousePos, const uint16 Width, const uint16 Heigth, const float OrthoTotal)
	{
		mwVec2f TheoricCenter((((float)MousePos._X / Width) - 0.5), (((float)MousePos._Y / Width) - 0.5));
		return mwVec2f(TheoricCenter._X * OrthoTotal, -TheoricCenter._Y * OrthoTotal);
	}

	//MUST BY SUPPLIED WITH THE PRODUCT OF 'Ortho * Ratio'. Made this way to increase performance
	static Vec2<float> centralizeOnScreenRatio(const Vec2<type> &MousePos, const uint16 Width, const uint16 Heigth, const float OrthoTotal, const float OrthoXRatio)
	{
		mwVec2f TheoricCenter(((float)MousePos._X / Width) - 0.5, ((float)MousePos._Y / Heigth) - 0.5);
		return mwVec2f(TheoricCenter._X * OrthoXRatio, -TheoricCenter._Y * OrthoTotal);
	}

	static Vec2<type> zero(void)
	{
		return Vec2(0, 0);
	}
};

template <class type>
class Vec2l : public Vec2<type>
{
protected:
	double length = 0;

public:
	Vec2l()
	{
		this->_X = 0;
		this->_Y = 0;
	}
	Vec2l(type X, type Y)
	{
		this->_X = X;
		this->_Y = Y;
	}

	//Basic Operators ( + | - | * | / )
	Vec2l operator=(const type k)
	{
		this->_X = k;
		this->_Y = k;
		return *this;
	}
	Vec2l operator=(const Vec2l &v)
	{
		//To prevent future errors (where, when reading, the lhs will deallocate, but the rhs is the lhs (same adress), generating errors and possible memory leaks)
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
		}
		return *this;
	}
	Vec2l operator=(const Vec2<type> &v)
	{
		//To prevent future errors (where, when reading, the lhs will deallocate, but the rhs is the lhs (same adress), generating errors and possible memory leaks)
		if (this != &v)
		{
			this->_X = v._X;
			this->_Y = v._Y;
		}
		return *this;
	}
	Vec2l operator--(const int k)
	{
		this->_X--;
		this->_Y--;
		return *this;
	}
	Vec2l operator++(const int k)
	{
		this->_X++;
		this->_Y++;
		return *this;
	}
	Vec2l operator-=(const type k)
	{
		this->_X -= k;
		this->_Y -= k;
		return *this;
	}
	Vec2l operator+=(const type k)
	{
		this->_X += k;
		this->_Y += k;
		return *this;
	}
	bool operator==(const Vec2l &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y);
	}
	bool operator!=(const Vec2l &v) const
	{
		return !(*this == v);
	}
	Vec2l operator+(const Vec2l &v) const
	{
		return Vec2l(this->_X + v._X, this->_Y + v._Y);
	}
	Vec2l & operator+=(const Vec2l &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		return *this;
	}
	Vec2l operator-(const Vec2l &v) const
	{
		return Vec2l(this->_X - v._X, this->_Y - v._Y);
	}
	Vec2l & operator-=(const Vec2l &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		return *this;
	}
	Vec2l operator-() const		//Negates value (not logicaly)
	{
		return Vec2l(-this->_X, -this->_Y);
	}
	//Scalar Multiplication
	Vec2l operator*(const type k) const
	{
		return Vec2l(this->_X * k, this->_Y * k);
	}
	Vec2l & operator*=(const type k)
	{
		this->_X *= k;
		this->_Y *= k;
		return *this;
	}
	//Scalar division
	Vec2l operator/(const type k) const
	{
		return Vec2l(this->_X / k, this->_Y / k);
	}
	Vec2l & operator/=(const type k)
	{
		this->_X /= k;
		this->_Y /= k;
		return *this;
	}
	//Produto escalar (Dot Product)
	type operator*(const Vec2l &v)
	{
		return ((this->_X * v._X) + (this->_Y * v._Y));
	}

	Vec2<type> returnAsVec2(void)
	{
		return Vec2<type>(this->_X, this->_Y);
	}

	//Multi input type conversion

	template <typename type2>
	Vec2l operator=(const Vec2l<type2> &v)
	{
		this->_X = v._X;
		this->_Y = v._Y;
		return *this;
	}
	template <typename type2>
	bool operator==(const Vec2l<type2> &v) const
	{
		return (this->_X == v._X, this->_Y == v._Y);
	}
	template <typename type2>
	bool operator!=(const Vec2l<type2> &v) const
	{
		return !(*this == v);
	}
	template <typename type2>
	Vec2l operator+(const Vec2l<type2> &v) const
	{
		return Vec2l(this->_X + v._X, this->_Y + v._Y);
	}
	template <typename type2>
	Vec2l & operator+=(const Vec2l<type2> &v)
	{
		this->_X += v._X;
		this->_Y += v._Y;
		return *this;
	}
	template <typename type2>
	Vec2l operator-(const Vec2l<type2> &v) const
	{
		return Vec2l(this->_X - v._X, this->_Y - v._Y);
	}
	template <typename type2>
	Vec2l & operator-=(const Vec2l<type2> &v)
	{
		this->_X -= v._X;
		this->_Y -= v._Y;
		return *this;
	}

	//Get the vector length (magnitude). If the current length is 0, it will compute it, if not, only return it
	double getLength(void)
	{
		if (length == 0)
		{
			return computeLength();
		}
		return length;
	}
	//Computes the vector length (magnitude)
	double computeLength(void)
	{
		length = sqrt((this->_X * this->_X) + (this->_Y * this->_Y));
		return length;
	}
	//Normalize vector (Only Return)
	Vec2l getNormalize(void)
	{
		getLength();	
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return a NULL vector
		{
			return Vec2l(0, 0);
		}
		return Vec2l(this->_X / length, this->_Y / length);
	}
	//Normalize vector(Overwrite Values)
	Vec2l setNormalize(void)
	{
		getLength();
		if (length == 0)	//Protection against NULL vectors (And division by 0): if length is 0, return itself unchanged
		{
			return *this;
		}
		this->_X = this->_X / length;
		this->_Y = this->_Y / length;
		return *this;
	}

	Vec2l<float> centralizeOnScreen(const uint16 Width, const uint16 Heigth, const float Ortho)
	{
		mwVec2lf TheoricCenter((((float)this->_X / Width) - 0.5), (((float)this->_Y / Width) - 0.5));
		mwVec2lf RealCenter(0, 0);

		RealCenter._X = TheoricCenter._X * (Ortho * 2);
		RealCenter._Y = -TheoricCenter._Y * (Ortho * 2);

		return RealCenter;
	}

	//Statics

	static Vec2l<float> centralizeOnScreen(const Vec2l<type> &MousePos, const uint16 Width, const uint16 Heigth, const float Ortho)
	{
		mwVec2lf TheoricCenter((((float)MousePos._X / Width) - 0.5), (((float)MousePos._Y / Width) - 0.5));
		mwVec2lf RealCenter(0, 0);

		RealCenter._X = TheoricCenter._X * (Ortho * 2);
		RealCenter._Y = -TheoricCenter._Y * (Ortho * 2);

		return RealCenter;
	}

	static Vec2l<type> zero(void)
	{
		return Vec2l<type>(0, 0);
	}
};

#endif;