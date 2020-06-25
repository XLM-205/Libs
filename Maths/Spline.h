/*------- ?? -------
*	--
*
*
*	Moon Wiz Studios (c) - 30/08/2018
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version -.-.-
*/

#ifndef _MW_SPLINE_
#define _MW_SPLINE_

#include "Vec2d.h"
#include "..\Control\DataTypes.h"
#include "..\Maths\MathTypes.h"

class BaseCurve
{
protected:
	int m_points;			//How many points this spline crosses
	int m_samples;			//"Points Per Polinomial" A.K.A.: How many points each polinomial generate for drawing graphs. Defaults to 100
	double *m_axis;			//The start (and end) between each point, with the respective step respecting samples value
	Polinomial *m_poli;		//The resulting quadratic polinomial for the whole curve
	List<mwVec2d> m_path;	//The resulting X, Y pair from start to finish

public:
	BaseCurve(int axis, int samples, double step)
	{
		Rebuild(axis, samples);
	}
	BaseCurve(int axis) : m_points(axis), m_samples(100)
	{
		Rebuild(axis);
	}
	BaseCurve() : m_points(0), m_samples(100), m_axis(nullptr), m_poli(nullptr) { }
	~BaseCurve()
	{
		Clear();
	}

	void Clear(void)
	{
		if (m_axis)
		{
			delete[] m_axis;
			m_axis = nullptr;
		}
		if (m_poli)
		{
			delete[] m_poli;
			m_poli = nullptr;
		}
		m_path.Clear();
	}
	void Rebuild(int axis)
	{
		Rebuild(axis, 100);
	}
	void Rebuild(int axis, int samples)
	{
		m_points = axis;
		m_samples = samples;
		if (m_axis)
		{
			delete[] m_axis;
		}
		if (m_poli)
		{
			delete[] m_poli;
		}
		m_axis = new double[m_points]();
		m_poli = new Polinomial[m_points - 1];
		m_path.Clear();
	}

	int Resolution(void)
	{
		return m_samples;
	}
	int Points(void)
	{
		return m_points;
	}
	
	double getStopAxis(int index)
	{
		if (index >= m_points)
		{
			return 0;
		}
		return m_axis[index];
	}
	double* getStopAxis(void)
	{
		return m_axis;
	}
	Polinomial getPolinomial(int index)
	{
		if (index >= m_points)
		{
			return Polinomial();
		}
		return m_poli[index];
	}
	Polinomial* getPolinomial(void)
	{
		return m_poli;
	}
	mwVec2d getPath(uint32 index)
	{
		if (index > m_path.Count())
		{
			return mwVec2d(0, 0);
		}
		return *m_path.GetAt(index);
	}
	List<mwVec2d>* getPath(void)
	{
		return &m_path;
	}
};

class SplineNatural : public BaseCurve
{
public:
	SplineNatural(int axis, int samples, double step) : BaseCurve(axis, samples, step) { }
	SplineNatural(int axis) : BaseCurve(axis) { }
	SplineNatural() : BaseCurve() { }

	List<mwVec2d>* Compute(mwVec2d *points, int amount, int samples)
	{
		Rebuild(amount, samples);
		return Compute(points, amount);
	}
	List<mwVec2d>* Compute(mwVec2d *points, int amount)
	{
		m_path.Clear();
		int n = m_points - 1;								//Last possible point
		double refCo[4] = { 0 };							//Cofactor Reference
		ArrayAuto<double> s(n, 0.0);
		ArrayAuto<double> b(n, 0.0), c(m_points, 0.0), d(n, 0.0);
		ArrayAuto<double> h(n, 0.0), alpha(n, 0.0), l(m_points, 1), u(m_points, 0.0), z(m_points, 0.0);
		// ----------------------------------------------------------------------------------- STEP 1 -
		for (int i = 0; i < n; i++)
		{
			h[i] = points[i + 1]._X - points[i]._X;
		}
		// ----------------------------------------------------------------------------------- STEP 2 -
		for (int i = 1; i < n; i++)
		{
			alpha[i] = (3 * (points[i + 1]._Y - points[i]._Y) / h[i]) - (3 * (points[i]._Y - points[i - 1]._Y) / h[i - 1]);
		}
		// ----------------------------------------------------------------------------------- STEP 3 -
		//l[0] is already 1 by class initialization
		//u[0] and z[0] are already 0 by class initialization
		// ----------------------------------------------------------------------------------- STEP 4 -
		for (int i = 1; i < n; i++)
		{
			l[i] = 2 * (points[i + 1]._X - points[i - 1]._X) - h[i - 1] * u[i - 1];
			u[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
		}
		// ----------------------------------------------------------------------------------- STEP 5 -
		l[n] = 1;
		z[n] = c[n] = 0;
		// ----------------------------------------------------------------------------------- STEP 6 -
		for (int i = n - 1; i >= 0; i--)
		{
			c[i] = z[i] - u[i] * c[i + 1];
			b[i] = ((points[i + 1]._Y - points[i]._Y) / h[i]) - (h[i] * (c[i + 1] + 2 * c[i]) / 3);
			d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
		}
		// ----------------------------------------------------------------------------------- STEP 7 -
		for (int i = 0; i < n; i++)
		{
			double step = (points[i + 1]._X - points[i]._X) / m_samples;
			double start = points[i]._X + step;
			double stop = points[i + 1]._X;
			refCo[3] = points[i]._Y;
			refCo[2] = b[i];
			refCo[1] = c[i];
			refCo[0] = d[i];
			m_poli[i].SetPoli(4, refCo);
			//printf("Poli(%3d) -> ", i + 1);
			//m_poli[i].print(FORMAT_DECIMAL, "\n");
			for (double s = start; s < stop; s += step)
			{
				m_path.Add(new mwVec2d(s, m_poli[i].solveFor(s - points[i]._X))); //produce a graph point for X and f(X - Xj)
			}
		}
		return &m_path;
	}
	List<mwVec2d>* Compute(List<mwVec2d> *points)
	{
		if (m_points != points->Count())
		{
			Rebuild(points->Count());
		}
		m_path.Clear();
		int n = m_points - 1;								//Last possible point
		double refCo[4] = { 0 };							//Cofactor Reference
		ArrayAuto<double> s(n, 0.0);
		ArrayAuto<double> b(n, 0.0), c(m_points, 0.0), d(n, 0.0);
		ArrayAuto<double> h(n, 0.0), alpha(n, 0.0), l(m_points, 1), u(m_points, 0.0), z(m_points, 0.0);
		Node<mwVec2d> *runner = points->RetrieveFirst();
		// ----------------------------------------------------------------------------------- STEP 1 -
		for (int i = 0; i < n; i++)
		{
			h[i] = runner->getNext()->getData()->_X - runner->getData()->_X;
			runner = runner->getNext();
		}
		// ----------------------------------------------------------------------------------- STEP 2 -
		runner = points->RetrieveFirst()->getNext();
		for (int i = 1; i < n; i++)
		{
			alpha[i] = (3 * (runner->getNext()->getData()->_Y - runner->getData()->_Y) / h[i]) - (3 * (runner->getData()->_Y - runner->getPrevious()->getData()->_Y) / h[i - 1]);
		}
		// ----------------------------------------------------------------------------------- STEP 3 -
		//l[0] is already 1 by class initialization
		//u[0] and z[0] are already 0 by class initialization
		// ----------------------------------------------------------------------------------- STEP 4 -
		runner = points->RetrieveFirst()->getNext();
		for (int i = 1; i < n; i++)
		{
			l[i] = 2 * (runner->getNext()->getData()->_X - runner->getPrevious()->getData()->_X) - h[i - 1] * u[i - 1];
			u[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
			runner = runner->getNext();
		}
		// ----------------------------------------------------------------------------------- STEP 5 -
		l[n] = 1;
		z[n] = c[n] = 0;
		// ----------------------------------------------------------------------------------- STEP 6 -
		runner = points->RetrieveLast()->getPrevious();
		for (int i = n - 1; i >= 0; i--)
		{
			c[i] = z[i] - u[i] * c[i + 1];
			b[i] = ((runner->getNext()->getData()->_Y - runner->getData()->_Y) / h[i]) - (h[i] * (c[i + 1] + 2 * c[i]) / 3);
			d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
			runner = runner->getPrevious();
		}
		// ----------------------------------------------------------------------------------- STEP 7 -
		runner = points->RetrieveFirst();
		for (int i = 0; i < n; i++, runner = runner->getNext())
		{
			double step = (runner->getNext()->getData()->_X - runner->getData()->_X) / m_samples;
			double start = runner->getData()->_X + step;
			double stop = runner->getNext()->getData()->_X;
			refCo[3] = runner->getData()->_Y;
			refCo[2] = b[i];
			refCo[1] = c[i];
			refCo[0] = d[i];
			m_poli[i].SetPoli(4, refCo);
			//printf("Poli(%3d) -> ", i + 1);
			//m_poli[i].print(FORMAT_DECIMAL, "\n");
			for (double s = start; s < stop; s += step)
			{
				m_path.Add(new mwVec2d(s, m_poli[i].solveFor(s - runner->getData()->_X))); //produce a graph point for X and f(X - Xj)
			}
		}
		return &m_path;
	}
	
};

class SplineClamped : public BaseCurve
{
private:
	
};

class BezierCurve : public BaseCurve
{

};

#endif	//MW Spline
