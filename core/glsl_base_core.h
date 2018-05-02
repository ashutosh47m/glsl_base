/*
	GLSL_base
	Nov 2017, Ashutosh Morwal
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef GLSL_BASE_CORE_H
#define GLSL_BASE_CORE_H

#include "window_handler.h"
#include "..\renderer\glrenderer.h"
#include "..\core\window_input.h"
class CORE
{
public:
	
	CORE() 
	{
		glrenderer = new opengl_renderer::renderer();

		window = new WINDOW_WIN32::win32_window_handler
			(TEXT("2am Engine 2.0"), 
				(float)GetSystemMetrics(SM_CXSCREEN), 
				(float)GetSystemMetrics(SM_CYSCREEN)
				);
	}
	void enableConsole();

	~CORE();

	opengl_renderer::renderer *glrenderer;
	WINDOW_WIN32::win32_window_handler *window;
	//W_INPUT::win32_input *input;
};
CORE *reactor = new CORE();



// this class will describe the configuration of the system on which glsl_base engine is running.

class SystemConfiguration
{
	GLuint sys_width, sys_height;
public:
	GLuint getWidth() { return sys_width; }
	GLuint getHeight() { return sys_height; }
};
#endif