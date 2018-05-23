/*
GLSL_base
May 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/


//this class will host such variables which can be easily incremented or decremented.
//these variables will typically be changed from user input.



template<class T>
class variable
{
	T x;
public:
	variable() {}
	variable(T tx) : x(tx) {}
	
	inline T getValue() { return T; }

	inline void setValue(T tx) { x = tx; }

	inline void incr(T inc) { x += inc; }

	inline void decr(T dec) { x += dec; }
};
