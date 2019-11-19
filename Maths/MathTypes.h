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

	double inPow(double x, double e)	//Internal Pow
	{
		if (e == 0.0)
		{
			return 1;
		}
		//else if (e > 0.0)
		//{
		return x * inPow(x, e - 1);
		//}
		//return 1 / inPow(x, -e);
	}

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
		double ret = 0.0, superPow = x;
		if (m_lastX != x)	//If we have evaluated this same x last time, don't waste CPU doing it again
		{
			m_lastX = x;
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
