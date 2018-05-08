/*
	GLSL_base
	Nov 2017, Ashutosh Morwal
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#include "window_handler.h"

using namespace WINDOW_WIN32;

win32_window_handler::~win32_window_handler(){}

const char * win32_window_handler::getName(void) { return m_windowName; }

void win32_window_handler::setName(const char *  title) { m_windowName = title; }

int win32_window_handler::getWidth() { return m_width; }
	
void win32_window_handler::setWidth(int  w) { m_width = w; }

int win32_window_handler::getHeight() { return m_height; }
	
void win32_window_handler::setHeight(int h) { m_height = h; }
