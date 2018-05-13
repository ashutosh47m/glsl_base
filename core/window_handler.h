/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef GL_WINDOW_HANDLER_H
#define GL_WINDOW_HANDLER_H


//  move windows headers to win32_window_handler
#include <windows.h>
//#include <windowsx.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

//you need these for OpenGL linking..
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")
#pragma comment(lib, "glfw3.lib")

/*
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "freetype253.lib")
#pragma comment(lib, "fmodex64_vc.lib")
*/

namespace WINDOW_WIN32
{

	class window_manager
	{
	public:
		window_manager() {};
		~window_manager() {};
	};

	class win32_window_handler : public window_manager
	{
		int			m_width, m_height;
		const char* m_windowName;
	public:

		win32_window_handler() {}
		win32_window_handler(const char *name, int w, int h)  
		{
			m_windowName = name;
			m_width = w;
			m_height = h;
		}

		char		*version;
		char		*vendor;
		char		*renderer;
		char		*glslver;

		bool		fullscreen;

		~win32_window_handler();

		const char *getName(void);
		void		setName(const char *title);
		int			getWidth();
		void		setWidth(int w);
		int			getHeight();
		void		setHeight(int h);
	};
}
#endif
