/*
GLSL_base
May 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/


//this class will host such variables which can be easily incremented or decremented.
//these variables will typically be changed from user input.

#ifndef _VARS_HH_
#define _VARS_HH_

#include <iostream>
template<class T>
class variable
{
	std::string name;
	T m_var;
	T m_varMax = 30767;
	T m_varMin =-30767;
public:
	// toggle = 0
	// toggle = 1 
	// toggle = 2 
	int Toggle;

	variable() {}
	variable(std::string nme, T tx)
		: m_var(tx), name(nme)
	{}
	variable(std::string nme, T tx, T min, T max)
		: m_var(tx), m_varMin(min), m_varMax(max), name(nme)
	{}
	

	// the value supplied to update function is the amount by which the variable will be changed.
	// as this is a template class the param could be anything. ;) 
	void update(T tx)
	{
		if (Toggle > 0)
		{
			if (Toggle == 1)
			{
				if (m_var >= m_varMax)
					return;
				incr(tx);
			}
			else if (Toggle == 2)
			{
				if (m_var <= m_varMin)
					return;
				decr(tx);
			}
			std::cout << name <<" " << m_var << std::endl;
		}
	}

	inline T getValue() { return m_var; }

	inline void setValue(T tx) { m_var = tx; }

	inline void incr(T inc) { m_var += inc; }

	inline void decr(T dec) { m_var -= dec; }
};

#endif