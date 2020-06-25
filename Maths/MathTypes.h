/*-------Mathematical Data Types-------
*	Defines math specific types of data such as Polinomials and Complex
*
*
*	Moon Wiz Studios (c) - 19/11/2019
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*	This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.5.0
*/

#ifndef _H_MATH_TYPES_
#define _H_MATH_TYPES_

//TODO: Implement dependencies from <math.h> to probably make this SA
//	*Current Dependecies:
//		-> sqrt()
#define MATH_ALLOW_UNSAFE	//Allow Unsafe, but faster, procedures

#include "../Control/CustomTypes-SA.h"

namespace Tables
{
	//Fatorial table up to 20!. Anything other than [0..20] returns 1;
	inline uint64 Fatorial(int i)
	{
		switch (i)
		{
		default:
		case 0:
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 6;
		case 4:
			return 24;
		case 5:
			return 120;
		case 6:
			return 720;
		case 7:
			return 5040;
		case 8:
			return 40320;
		case 9:
			return 362880;
		case 10:
			return 3628800;
		case 11:
			return 39916800;
		case 12:
			return 479001600;
		case 13:
			return 6227020800;
		case 14:
			return 87178291200;
		case 15:
			return 1307674368000;
		case 16:
			return 20922789888000;
		case 17:
			return 355687428096000;
		case 18:
			return 6402373705728000;
		case 19:
			return 121645100408832000;
		case 20:
			return 2432902008176640000;
		}
	}
}

namespace Math
{
	inline double Power(double, double);

	namespace Constants
	{
		constexpr double PI = 3.141592653589793;	//PI (15 decimal places)
		constexpr double PI2 = PI * 2;				//2 * PI
		constexpr double PI_RAD_DEG = PI / 180;		//PI  / 180
		constexpr double PI_DEG_RAD = 180 / PI;		//180 / PI
		constexpr double EULER = 2.718281828459045;	//Euler Constant (15 decimal places)
	}

#ifdef MATH_ALLOW_UNSAFE

	// !! WARNING !!
	//Assembly functions may or may not be incompatible and / or interfere with compiler optimizations!
	// !! USE ON YOUR OWN RISK !!

	//A safer sqrt, albeit slower
	/*double Sqrt_Fast(double n)
	{
		_asm
		{
			fld n
			fsqrt
		}
	}*/

	double inline __declspec (naked) __fastcall Sqrt_Fast(double n)
	{
		n;	//Hack to prevent warning from unused variable
		_asm
		{
			fld qword ptr[esp + 4]
			fsqrt
			ret 8
		}
	}

#endif	//Unsafe functions

	inline double Sqrt(double x, int precision)
	{
		for (double y = 1; true; y++)
		{
			double ySQ = y * y;	//Try to find a Y^2 that is equal or higher than 'x'
			if (ySQ == x)
			{
				return y;		//Perfect square!
			}
			else if (ySQ > x)
			{
				double i = y - 1, j = y;
				double middle = (i + j) * 0.5;
				double mult = middle * middle;
				while (true)
				{
					if (mult == x || Utils::isEqualRange(mult, x, precision))
					{
						return middle;
					}
					else if (mult < x)
					{
						i = middle;
					}
					else
					{
						j = middle;
					}
					middle = (i + j) * 0.5;
					mult = middle * middle;
				}
			}
		}
	}

	inline double Sqrt(double x)
	{
		return Sqrt(x, 5);
	}

	inline double Power(double X, int e)
	{
		if (e == 0)
		{
			return 1;
		}
		else if (e > 0)
		{
			return X * Power(X, e - 1);
		}
		return 1 / Power(X, -e);
	}

	//Typical Trigonometric functions
	inline double sinRad(double n, uint32 iter)
	{
		double a = Utils::ModClamped(n, Constants::PI2);
		double x = -a * a * a, out = a;
		if (iter > 20)	//Max out at 20 iterations, as 21! cannot be hold in a uint64
		{
			iter = 20;
		}
		for (uint32 i = 3; i < iter; i += 2)
		{
			out += x / Tables::Fatorial(i);
			x = a * a * -x;
		}
		return out;
	}
	inline double sinDeg(double n, uint32 iter)
	{
		double a = Utils::degToRadClamped(n);
		double x = -a * a * a, out = a;
		if (iter > 20)
		{
			iter = 20;
		}
		for (uint32 i = 3; i < iter; i += 2)
		{
			out += x / Tables::Fatorial(i);
			x = a * a * -x;
		}
		return out;
	}
	inline double sinRad(double n)
	{
		// 10 iteractions seems to match the standard sin by 8 decimal places, AT LEAST
		return Math::sinRad(n, 10);
	}
	inline double sinDeg(double n)
	{
		return Math::sinDeg(n, 10);
	}

	inline double cosRad(double n, uint32 iter)
	{
		double a = Utils::ModClamped(n, Constants::PI2);
		double x = -a * a, out = 1;
		if (iter > 20)
		{
			iter = 20;
		}
		for (uint32 i = 2; i < iter; i += 2)
		{
			out += x / Tables::Fatorial(i);
			x = a * a * -x;
		}
		return out;
	}
	inline double cosDeg(double n, uint32 iter)
	{
		double a = Utils::degToRadClamped(n);
		double x = -a * a , out = 1;
		if (iter > 20)
		{
			iter = 20;
		}
		for (uint32 i = 2; i < iter; i += 2)
		{
			out += x / Tables::Fatorial(i);
			x = a * a * -x;
		}
		return out;
	}
	inline double cosRad(double n)
	{
		return cosRad(n, 10);
	}
	inline double cosDeg(double n)
	{
		return cosDeg(n, 10);
	}

	inline double tanRad(double n, uint32 iter)
	{
		return Math::sinRad(n, iter) / Math::cosRad(n, iter);
	}
	inline double tanDeg(double n, uint32 iter)
	{
		return Math::sinDeg(n, iter) / Math::cosDeg(n, iter);
	}
	inline double tanRad(double n)
	{
		return Math::sinRad(n, 10) / Math::cosRad(n, 10);
	}
	inline double tanDeg(double n)
	{
		return Math::sinDeg(n, 10) / Math::cosDeg(n, 10);
	}

	inline double coSecRad(double n, uint32 iter)
	{
		return 1 / Math::sinRad(n, iter);
	}
	inline double coSecDeg(double n, uint32 iter)
	{
		return 1 / Math::sinDeg(n, iter);
	}
	inline double coSecRad(double n)
	{
		return 1 / Math::sinRad(n, 10);
	}
	inline double coSecDeg(double n)
	{
		return 1 / Math::sinDeg(n, 10);
	}

	inline double secRad(double n, uint32 iter)
	{
		return 1 / Math::cosRad(n, iter);
	}
	inline double secDeg(double n, uint32 iter)
	{
		return 1 / Math::cosDeg(n, iter);
	}
	inline double secRad(double n)
	{
		return 1 / Math::cosRad(n, 10);
	}
	inline double secDeg(double n)
	{
		return 1 / Math::cosRad(n, 10);
	}

	inline double coTanRad(double n, uint32 iter)
	{
		return Math::cosRad(n, iter) / Math::sinRad(n, iter);
	}
	inline double coTanDeg(double n, uint32 iter)
	{
		return Math::cosDeg(n, iter) / Math::sinDeg(n, iter);
	}
	inline double coTanRad(double n)
	{
		return Math::cosRad(n, 10) / Math::sinRad(n, 10);
	}
	inline double coTanDeg(double n)
	{
		return Math::cosDeg(n, 10) / Math::sinDeg(n, 10);
	}

	//Trigonometric form points, given a point P(x, y) centered at (0, 0)
	inline double sinPoint(double X, double Y)
	{
		return Y /  Math::Sqrt(X * X + Y * Y);
	}
	inline double cosPoint(double X, double Y)
	{
		return X / Math::Sqrt(X * X + Y * Y);
	}
	inline double tanPoint(double X, double Y)
	{
		return Y / X;
	}
	inline double secPoint(double X, double Y)
	{
		return Math::Sqrt(X * X + Y * Y) / Y;
	}
	inline double coSecPoint(double X, double Y)
	{
		return Math::Sqrt(X * X + Y * Y) / X;
	}
	inline double coTanPoint(double X, double Y)
	{
		return X / Y;
	}
}

//Defines a rational number, with both numerator and denominator
class Rational
{
private:
	double m_Num;
	double m_Deno;

public:
	Rational() : m_Num(1), m_Deno(1) { };
	Rational(double Numerator, double Denominator) : m_Num(Numerator), m_Deno(Denominator) { };

	double getNumerator(void)
	{
		return m_Num;
	}
	double getDenominator(void)
	{
		return m_Deno;
	}
	double getValue(void)
	{
		if (m_Deno)
		{
			return m_Num / m_Deno;
		}
		return 0;
	}

	void setNumerator(double Num)
	{
		m_Num = Num;
	}
	void setDenominator(double Deno)
	{
		m_Deno = Deno;
	}

	//TODO: Make a function to rationalize any number

	Rational operator+(const Rational &r) const
	{
		return Rational(m_Num * r.m_Deno + r.m_Num * m_Deno, m_Deno * r.m_Deno);
	}
	Rational operator-(const Rational &r) const
	{
		return Rational(m_Num * r.m_Deno - r.m_Num * m_Deno, m_Deno * r.m_Deno);
	}
	Rational operator*(const Rational &r) const
	{
		return Rational(m_Num * r.m_Num, m_Deno * r.m_Deno);
	}
	Rational operator/(const Rational &r) const
	{
		return Rational(m_Num * r.m_Deno, m_Deno * r.m_Num);
	}
	Rational operator=(const Rational &r)
	{
		m_Num = r.m_Num;
		m_Deno = r.m_Deno;
		return *this;
	}
	bool operator==(const Rational &r) const
	{
		return (m_Num * r.m_Deno) == (m_Deno * r.m_Num);
	}
	bool operator!=(const Rational &r) const
	{
		return !(*this == r);
	}
};

//Defines a complex number, with both real and imaginary parts
class Complex
{
private:
	double m_Real;
	double m_Im;

public:
	Complex() : m_Real(0), m_Im(0) { };
	Complex(double Real, double Imaginary) : m_Real(Real), m_Im(Imaginary) { };

	void setReal(double Real)
	{
		m_Real = Real;
	}
	void setImaginary(double Imaginary)
	{
		m_Im = Imaginary;
	}

	double getReal(void)
	{
		return m_Real;
	}
	double getImaginary(void)
	{
		return m_Im;
	}

	Complex operator=(const Complex &c)
	{
		m_Real = c.m_Real;
		m_Im = c.m_Im;
		return *this;
	}
	Complex operator+(const Complex &c) const
	{
		return Complex(m_Real + c.m_Real, m_Im + c.m_Im);
	}
	Complex & operator+=(const Complex &c)
	{
		m_Real += c.m_Real;
		m_Im += c.m_Im;
		return *this;
	}
	Complex operator-(const Complex &c) const
	{
		return Complex(m_Real - c.m_Real, m_Im - c.m_Im);
	}
	Complex & operator-=(const Complex &c)
	{
		m_Real -= c.m_Real;
		m_Im -= c.m_Im;
		return *this;
	}
	Complex operator*(const Complex &c) const
	{
		return Complex(m_Real * c.m_Real - m_Im * c.m_Im, m_Im * c.m_Real + m_Real * c.m_Im);
	}
	Complex & operator*=(const Complex &c)
	{
		double tempR = m_Real, tempI = m_Im;
		m_Real = tempR * c.m_Real - tempI * c.m_Im;
		m_Im = tempI * c.m_Real + tempR * c.m_Im;
		return *this;
	}
	Complex operator/(const Complex &c) const
	{
		double SQSum = c.m_Real * c.m_Real + c.m_Im * c.m_Im;
		return Complex((m_Real * c.m_Real + m_Im * c.m_Im) / SQSum, (m_Im * c.m_Real - m_Real * c.m_Im) / SQSum);
	}
	Complex & operator/=(const Complex &c)
	{
		double SQSum = c.m_Real * c.m_Real + c.m_Im * c.m_Im;
		double tempR = m_Real, tempI = m_Im;
		m_Real = (tempR * c.m_Real + tempI * c.m_Im) / SQSum;
		m_Im = (tempI * c.m_Real - tempR * c.m_Im) / SQSum;
		return *this;
	}
	bool operator==(const Complex &c) const
	{
		return (m_Real == c.m_Real) && (m_Im == c.m_Im);
	}
	bool operator!=(const Complex &c) const
	{
		return !(*this == c);
	}
};

//Defines a Polinomial. It's built following the order of the array supplied.
//e.g.: take double *a = {A, B, -C, D}
// Will Produce this Polinomial -> Ax^3 + Bx^2 -Cx + D
class Polinomial
{
private:
	double *m_elements;
	double m_lastSolve;
	double m_lastX;
	int m_count;

public:
	Polinomial() : m_count(0), m_lastSolve(0), m_lastX(0), m_elements(nullptr) { }

	Polinomial(int count, double *a)
	{
		m_lastSolve = m_lastX = 0;
		m_count = count;
		m_elements = new double[m_count];
		for (int i = 0; i < m_count; i++)
		{
			m_elements[i] = a[i];
		}
	}

	~Polinomial()
	{
		if (m_elements)
		{
			delete[] m_elements;
			m_elements = nullptr;
		}
	}

	void SetPoli(int size, double *in)
	{
		m_lastSolve = m_lastX = 0;
		m_count = size;
		if (m_elements)
		{
			delete[] m_elements;
		}
		m_elements = new double[m_count];
		for (int i = 0; i < m_count; i++)
		{
			m_elements[i] = in[i];
		}
	}

	double solveFor(double x)
	{
		if (x == 0.0)		//If 'x' is 0 then polinomial also equals to 0
		{
			return 0;
		}
		int stop = m_count - 1;
		double superPow = x;
		if (m_lastX != x)	//If we have evaluated this same x last time, don't waste CPU doing it again
		{
			m_lastX = x;
			m_lastSolve = 0;
			for (int i = stop - 1; i >= 0; i--)
			{
				m_lastSolve += m_elements[i] * superPow;
				superPow *= x;
			}
			m_lastSolve += m_elements[stop];
		}
		return m_lastSolve;
	}

#ifdef _INC_STDIO	//If <stdio.h> is included, allow print function
	//'format' must be a valid printf format
	//'onEnd' is what it should do when finishing printing. Also follows printf rules
	void print(const char *format, const char *onEnd)
	{
		int power = m_count - 1;
		for (int i = 0; i < m_count; power--, i++)
		{
			if (m_elements[i] != 1.0 || !power)
			{
				printf(format, m_elements[i]);
			}
			if (power > 1)
			{
				printf("X^%d", power);
			}
			else if (power == 1)
			{
				printf("X");
			}
			if ((i + 1) < m_count && m_elements[i + 1] >= 0)
			{
				printf("+");
			}
		}
		printf(onEnd);
	}

	void print(double x, const char *format, const char *onEnd)
	{
		printf("P(%.2lf) = ", x);
		print(format, onEnd);
		printf("\t= %.4lf\n", solveFor(x));
	}

	void print(double x)
	{
		printf("P(%.2lf) = ", x);
		print("%.2lf", "\n");
		printf("\t= %.4lf\n", solveFor(x));
	}
#endif
};

#endif //MathTypes
