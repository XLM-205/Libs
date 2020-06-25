/*-------Random Number Generator-------
*	??
*	-- ??
*
*
*	Moon Wiz Studios (c) - 16/08/2015
*	by: Ramon Darwich de Menezes
*
*	
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version --.--.--
*/


#ifndef _H_MWRNG_
#define _H_MWRNG_

#define MWRNG_MAX_DEFAULT 5e+100


enum RNGTechniques
{
	RNG_TECH_ALPHA,		//Constant output if the same seed and generation is supplied
	RNG_TECH_BETA,		//
	RNG_TECH_DELTA,
	RNG_TECH_GAMA,
	RNG_TECH_OMEGA
};

class RNG
{
private:
	//Number Generation Technique Table
	double GenAlpha(void)
	{
		static double factorA;
		static double factorB;
		factorA = m_Seed * 1.64868;
		factorB = m_Generations * 1.57;
		return normalizeOutput(factorA * m_Output + factorB);
	}

protected:
	//Seed and Generation count are booth double to prevent using typecasting when generating numbers
	double m_Seed = 1;					//Seed used for the Generation Techniques (defaults to 1)
	double m_Generations = 1;			//The amount of numbers generated so far
	double m_Output;					//The current output. When asked to generate a number, this value gets updated
	double m_Max = MWRNG_MAX_DEFAULT;	//Maximum allowed value. Used when normalizing the output. Should not be > 1.79769e+308
	double m_Min = 0;					//Minimum allowed value. Used when normalizing the output. Should not be <0
	bool is_seedLocked = false;			//If true, the seed cannot be changed
	
	double(RNG::*Generate)(void) = nullptr;

	void setTechnique(RNGTechniques Tech)
	{
		switch (Tech)
		{
		case RNG_TECH_ALPHA:
			Generate = &RNG::GenAlpha;
			break;
		case RNG_TECH_BETA:
			break;
		case RNG_TECH_DELTA:
			break;
		case RNG_TECH_GAMA:
			break;
		case RNG_TECH_OMEGA:
			break;
		}
	}
	double normalizeOutput(double In)
	{
		if (In >= m_Max)
		{
			return 1;
		}
		else if (In <= m_Min)
		{
			return 0;
		}
		return (In - m_Min) / (m_Max - m_Min);
	}
public:
	RNG(int32 Seed, RNGTechniques Tech, double Max, double Min) : m_Seed(Seed), m_Generations(0), m_Output(MWRNG_MAX_DEFAULT * 0.5), m_Max(Max), m_Min(Min) { setTechnique(Tech); };
	RNG(int32 Seed, uint32 Gen, RNGTechniques Tech, double Max, double Min) : m_Seed(Seed), m_Generations(Gen), m_Output(0.5), m_Max(Max), m_Min(Min) { setTechnique(Tech); };

	void setSeed(int32 NewSeed)
	{
		m_Seed = NewSeed;
	}
	int32 getSeed(void)
	{
		return m_Seed;
	}
	uint32 getGenerationTotal(void)
	{
		return m_Generations;
	}
	double getLastOutput(void)
	{
		return m_Output;
	}
	double getMax(void)
	{
		return m_Max;
	}
	double getMin(void)
	{
		return m_Min;
	}
	double generateNumber(void)
	{
		m_Output = (this->*Generate)();
		m_Generations++;
		return m_Output;
	}
	template <class T>
	T scaleValue(T scale)
	{
		return m_Output * scale;
	}
	template <class T>
	T scaleValueGen(T scale)
	{
		return generateNumber() * scale;
	}
	void setMax(double Max)
	{
		if (Max > m_Min && Max < 1.79769e+308)
		{
			m_Max = Max;
		}
	}
	void setMin(double Min)
	{
		if (Min < m_Min && Min >= 0)
		{
			m_Min = Min;
		}
	}

};

#endif;	//Moon Wiz Random Number Generator