/*
GLSL_base
Apr 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#include <windows.h>

#ifndef _WINDOW_INPUT
#define _WINDOW_INPUT

namespace W_INPUT
{
	class win32_input
	{
		WPARAM wParam;
		LPARAM lParam;

	public:
		void keyProcess(WPARAM wp, LPARAM lp)
		{
			wParam = wp;
			lParam = lp;
		}

		WPARAM getWParam() { return wParam; }
		LPARAM getLParam() { return lParam; }

		win32_input() : wParam(NULL), lParam(NULL) {}
		~win32_input() {}
	};
}

#endif
