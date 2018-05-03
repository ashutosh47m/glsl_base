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
#include <windowsx.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <stdio.h>

//you need these for OpenGL linking..
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

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
		float		m_width, m_height;
		LPTSTR		m_windowName;
	public:
		win32_window_handler() {}
		win32_window_handler(LPTSTR name, float w, float h) : hrc(NULL), hdc(NULL), hwnd(NULL), hInstance(NULL)
		{
			//printf("e %d", e);
			m_windowName = name;
			m_width = w;
			m_height = h;
		}

		char		*version;
		char		*vendor;
		char		*renderer;
		char		*glslver;

		bool		fullscreen;

		HGLRC		hrc;
		HDC			hdc;
		HWND		hwnd;
		HINSTANCE	hInstance;
		HWND		splash_hwnd;
		HBITMAP		hBitmap;

		LPSYSTEMTIME time;

		~win32_window_handler();

		bool		createDC(BYTE);
		bool		createWindow(LPTSTR title, float w, float h, int bits, bool isFullscreen);
		void		createSplashWindow();
		int			dialog(LPCWSTR title, LPCWSTR msg, int flag);
		void		closeWindow();
		void		updateLoadStats(int);
		LPTSTR		getName(void);
		void		setName(LPTSTR title);
		float		getWidth();
		void		setWidth(float w);
		float		getHeight();
		void		setHeight(float h);

		// ##### enums 
		// dialog box flags
		enum DIALOG_FLAGS { ERR, INFO, YES_NO_INFO, YES_NO_EXCLAMATION };

	};
}
#endif
