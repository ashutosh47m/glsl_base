/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#include "glsl_base_core.h"
#include <stdio.h>

void CORE::enableConsole()
{
	FILE * stream;
	AllocConsole();
	freopen_s(&stream, "CONIN$", "r",stdin);
	freopen_s(&stream, "CONOUT$", "w",stdout);
	freopen_s(&stream, "CONOUT$", "w",stderr);
	
	fclose(stream);
}

CORE::~CORE()
{
	printf("des core\n");
	delete glrenderer;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	reactor->glrenderer->getScene()->keyProcess(key, scancode, action, mods);

	if (key == GLFW_KEY_ESCAPE)
	{
		reactor->shuttingDown = true;
		delete reactor;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lmCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	reactor->enableConsole();
#endif
	if (!glfwInit())
	{
		printf("glfw not initialized\n");
	}

	reactor->window->setWidth(1280);
	reactor->window->setHeight(720);
	reactor->window->setName("2am engine : glsl_base");
	reactor->window->fullscreen = false;

	GLFWwindow* window2;
	window2 = glfwCreateWindow(
							reactor->window->getWidth(), 
							reactor->window->getHeight(),
							reactor->window->getName(), NULL, NULL);
	if (!window2)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window2);

	// THIS ORDER IS REALLY IMPORTANT,
	// YOU FIRST HAVE TO CREATE OPENGL CONTEXT, AND THEN ONLY YOU CAN CALL GLEWINIT().
	// IF YOU TRY TO CALL GLEWINIT BEFORE CREATING CONTEXT, BAD THINGS WILL HAPPEN.
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		const GLubyte *s = glewGetErrorString(GlewInitResult);
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
	}

	reactor->glrenderer->initGL();
	reactor->glrenderer->resizeGL((float)reactor->window->getWidth(), (float)reactor->window->getHeight());
	
	glfwSetKeyCallback(window2, key_callback);

	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(window2))
	{
		// Render here 
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.4f, .4f, .4f, 1);

		if (! reactor->shuttingDown)
		{
			reactor->glrenderer->updateGL();
			reactor->glrenderer->displayGL();
		}
		else 
			break;

		glfwSwapBuffers(window2);
//		int state = glfwGetKey(window2, GLFW_KEY_ESCAPE);
//		if (state == GLFW_PRESS)
//			break;	

		glfwPollEvents();
	}
	return 0;
}