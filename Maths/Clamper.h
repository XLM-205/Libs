/*-------Automatic Clamped Variable-------
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
*	Version 0.9
*/

#ifndef _MOON_PHYSICS_CLAMP_H_
#define _MOON_PHYSICS_CLAMP_H_

#ifndef HIDE_CLAMP								//Hide the clamp's macros, to prevent polluting the IDE

	#define mwClampi Clamper<int>				//SIGNED INT Clamped variable
	#define mwClampf Clamper<float>				//SIGNED FLOAT Clamped variable
	#define mwClampd Clamper<double>			//SIGNED DOUBLE Clamped variable

	#define mwClampui uClamper<unsigned int>	//UNSIGNED INT Clamped variable
	#define mwClampuf uClamper<float>			//0 MIN ("unsigned") FLOAT Clamped variable
	#define mwClampud uClamper<double>			//0 MIN ("unsigned") DOUBLE Clamped variable

#endif

template <class type>
//Unsigned Clamping (Max must be defined / Min is always 0)
class uClamper
{
private:

	//Clampers (used to make code smaller)(inlined to make it faster)
	inline type uClampCheck(void)
	{
		if (val > mx)
		{
			val = mx;
		}
		else if (val < 0)
		{
			val = 0;
		}
		return val;
	}
	inline type uClampCheck(const type k)
	{
		val = k;
		if (val > mx)
		{
			val = mx;
		}
		else if (val < 0)
		{
			val = 0;
		}
		return val;
	}
	inline type uClampCheck(const uClamper<type> &c)
	{
		val = c.getValue();
		if (val > mx)
		{
			val = mx;
		}
		else if (val < 0)
		{
			val = 0;
		}
		return val;
	}

protected:
	type val;
	type mx;
	
public:

	uClamper() : val(0), mx(0) {};
	uClamper(type Value, type Clamp)
	{
		mx = Clamp;
		val = Value;
		uClampCheck();
	}

	//Getters & Setters
	inline void setClampMax(type ClampMx) 
	{ 
		if (ClampMx > 0)	//Only allow if the value is over 0
		{
			mx = ClampMx;
			uClampCheck();
		}
	}
	inline void setValue(type Value) 
	{ 
		uClampCheck(Value);
	}
	inline type getMax(void) const 
	{
		return mx; 
	}
	inline type getValue(void) const
	{ 
		return val; 
	}

	//Operators

	uClamper & operator= (const type k)
	{
		uClampCheck(k);
		return *this;
	}

	uClamper & operator= (const uClamper<type> &c)
	{
		if (this != &c)
		{
			uClampCheck(c);
		}
		return *this;
	}

	uClamper & operator++ (const int k)
	{
		val++;
		uClampCheck();
		return *this;
	}

	uClamper & operator-- (const int k)
	{
		val--;
		uClampCheck();
		return *this;
	}

	uClamper & operator+ (const type k)
	{
		val += k;
		uClampCheck();
		return *this;
	}

	uClamper & operator- (const type k)
	{
		val -= k;
		uClampCheck();
		return *this;
	}

	uClamper & operator* (const type k)
	{
		val *= k;
		uClampCheck();
		return *this;
	}

	uClamper & operator/ (const type k)
	{
		if (k != 0)
		{
			val /= k;
			uClampCheck();
		}
		return *this;
	}

	uClamper & operator+ (const uClamper<type> &c)
	{
		val += c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator- (const uClamper<type> &c)
	{
		val -= c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator* (const uClamper<type> &c)
	{
		val *= c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator/ (const uClamper<type> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			uClampCheck();
		}
		return *this;
	}

	uClamper & operator+= (const type k)
	{
		val += k;
		uClampCheck();
		return *this;
	}

	uClamper & operator-= (const type k)
	{
		val -= k;
		uClampCheck();
		return *this;
	}

	uClamper & operator*= (const type k)
	{
		val *= k;
		uClampCheck();
		return *this;
	}

	uClamper & operator/= (const type k)
	{
		if (k != 0)
		{
			val /= k;
			uClampCheck();
		}
		return *this;
	}

	uClamper & operator+= (const uClamper<type> &c)
	{
		val += c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator-= (const uClamper<type> &c)
	{
		val -= c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator*= (const uClamper<type> &c)
	{
		val *= c.getValue();
		uClampCheck();
		return *this;
	}

	uClamper & operator/= (const uClamper<type> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			uClampCheck();
		}
		return *this;
	}

	//Logic Operators
	bool operator==(const type k) const
	{
		return (val == k);
	}

	bool operator!=(const type k) const
	{
		return val != k;
	}

	bool operator<(const type k) const
	{
		return (val < k);
	}

	bool operator<=(const type k) const
	{
		return (val <= k);
	}

	bool operator>(const type k) const
	{
		return (val > k);
	}

	bool operator>=(const type k) const
	{
		return (val >= k);
	}

	bool operator==(const uClamper &c) const
	{
		return (val == c.getValue());
	}

	bool operator!=(const uClamper &c) const
	{
		return !(*this == c);
	}

	//Multi type support

	template <typename type2>
	uClamper & operator=(const uClamper<type2> &c)
	{
		uClampCheck(c.getValue());
		return *this;
	}

	template <typename type2>
	uClamper & operator+ (const uClamper<type2> &c)
	{
		val += c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator- (const uClamper<type2> &c)
	{
		val -= c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator* (const uClamper<type2> &c)
	{
		val *= c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator/ (const uClamper<type2> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			uClampCheck();
		}
		return *this;
	}

	template <typename type2>
	uClamper & operator+= (const uClamper<type2> &c)
	{
		val += c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator-= (const uClamper<type2> &c)
	{
		val -= c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator*= (const uClamper<type2> &c)
	{
		val *= c.getValue();
		uClampCheck();
		return *this;
	}

	template <typename type2>
	uClamper & operator/= (const uClamper<type2> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			uClampCheck();
		}
		return *this;
	}

	//Logic Operators
	template <typename type2>
	bool operator==(const uClamper<type2> &c) const
	{
		return (val == c.getValue());
	}

	template <typename type2>
	bool operator!=(const uClamper<type2> &c) const
	{
		return !(*this == c);
	}


};

template <class type>
//Signed Clamping (Max and Min must be defined. If Min is undefined, default's to -Max)
class Clamper : public uClamper<type>
{
private:
	//Clampers (used to make code smaller)(inlined to make it faster)
	inline type ClampCheck(void) 
	{
		if (val > mx)
		{
			val = mx;
		}
		else if (val < mn)
		{
			val = mn;
		}
		return val;
	}
	inline type ClampCheck(const type k)
	{
		val = k;
		if (val > mx)
		{
			val = mx;
		}
		else if (val < mn)
		{
			val = mn;
		}
		return val;
	}
	inline type ClampCheck(const Clamper<type> &c)
	{
		val = c.getValue();
		if (val > mx)
		{
			val = mx;
		}
		else if (val < mn)
		{
			val = mn;
		}
		return val;
	}

protected:
	type mn;

public:

	Clamper() : uClamper<type>(0,0), mn(0) {};
	Clamper(type Value, type Clamp)
	{
		val = Value;
		mx = Clamp;
		mn = -Clamp;
	}
	Clamper(type Value, type ClampMx, type ClampMn)
	{
		val = Value;
		mx = ClampMx;
		mn = ClampMn;
	}

	//Getters & Setters
	inline void setClampMin(type ClampMn) 
	{
		if (ClampMn < mx)
		{
			mn = ClampMn;
			ClampCheck();
		}
	}
	inline type getMin(void) const 
	{
		return mn; 
	}

	//Operators

	Clamper & operator= (const type k)
	{
		ClampCheck(k);
		return *this;
	}

	Clamper & operator= (const Clamper<type> &c)
	{
		if (this != &c)
		{
			ClampCheck(c);
		}
		return *this;
	}

	Clamper & operator++ (const int k)
	{
		val++;
		ClampCheck();
		return *this;
	}

	Clamper & operator-- (const int k)
	{
		val--;
		ClampCheck();
		return *this;
	}

	Clamper & operator+ (const type k)
	{
		val += k;
		ClampCheck();
		return *this;
	}

	Clamper & operator- (const type k)
	{
		val -= k;
		ClampCheck();
		return *this;
	}

	Clamper & operator* (const type k)
	{
		val *= k;
		ClampCheck();
		return *this;
	}

	Clamper & operator/ (const type k)
	{
		if (k != 0)
		{
			val /= k;
			ClampCheck();
		}
		return *this;
	}

	Clamper & operator+ (const Clamper<type> &c)
	{
		val += c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator- (const Clamper<type> &c)
	{
		val -= c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator* (const Clamper<type> &c)
	{
		val *= c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator/ (const Clamper<type> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			ClampCheck();
		}
		return *this;
	}

	Clamper & operator+= (const type k)
	{
		val += k;
		ClampCheck();
		return *this;
	}

	Clamper & operator-= (const type k)
	{
		val -= k;
		ClampCheck();
		return *this;
	}

	Clamper & operator*= (const type k)
	{
		val *= k;
		ClampCheck();
		return *this;
	}

	Clamper & operator/= (const type k)
	{
		if (k != 0)
		{
			val /= k;
		}
		ClampCheck();
		return *this;
	}

	Clamper & operator+= (const Clamper<type> &c)
	{
		val += c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator-= (const Clamper<type> &c)
	{
		val -= c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator*= (const Clamper<type> &c)
	{
		val *= c.getValue();
		ClampCheck();
		return *this;
	}

	Clamper & operator/= (const Clamper<type> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
		}
		ClampCheck();
		return *this;
	}

	//Logic Operators
	bool operator==(const type k) const
	{
		return (val == k);
	}

	bool operator!=(const type k) const
	{
		return val != k;
	}

	bool operator<(const type k) const
	{
		return (val < k);
	}

	bool operator<=(const type k) const
	{
		return (val <= k);
	}

	bool operator>(const type k) const
	{
		return (val > k);
	}

	bool operator>=(const type k) const
	{
		return (val >= k);
	}

	bool operator==(const Clamper &c) const
	{
		return (val == c.getValue());
	}

	bool operator!=(const Clamper &c) const
	{
		return !(*this == c);
	}

	//Multi type support

	template <typename type2>
	Clamper & operator=(const Clamper<type2> &c)
	{
		ClampCheck(c.getValue());
		return *this;
	}

	template <typename type2>
	Clamper & operator+ (const Clamper<type2> &c)
	{
		val += c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator- (const Clamper<type2> &c)
	{
		val -= c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator* (const Clamper<type2> &c)
	{
		val *= c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator/ (const Clamper<type2> &c)
	{
		val /= c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator+= (const Clamper<type2> &c)
	{
		val += c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator-= (const Clamper<type2> &c)
	{
		val -= c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator*= (const Clamper<type2> &c)
	{
		val *= c.getValue();
		ClampCheck();
		return *this;
	}

	template <typename type2>
	Clamper & operator/= (const Clamper<type2> &c)
	{
		if (c.getValue() != 0)
		{
			val /= c.getValue();
			ClampCheck();
		}
		return *this;
	}

	//Logic Operators
	template <typename type2>
	bool operator==(const Clamper<type2> &c) const
	{
		return (val == c.getValue());
	}

	template <typename type2>
	bool operator!=(const Clamper<type2> &c) const
	{
		return !(*this == c);
	}

};

#endif;	//Clamper.h