/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#include "glsl_base_core.h"

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

//resize viewport if window size changed
void window_size_callback(GLFWwindow* window, int width, int height)
{
	if(width > 0 && height > 0)
		reactor->glrenderer->resizeGL(width, height);
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

	int systemWidth  = GetSystemMetrics(SM_CXSCREEN);
	int systemHeight = GetSystemMetrics(SM_CYSCREEN);

#ifdef _DEBUG
	reactor->window->setWidth(1280);
	reactor->window->setHeight(720);
#endif

	//reactor->window->setWidth(systemWidth);
	//reactor->window->setHeight(systemHeight);
	reactor->window->setName("2am engine : glsl_base");
	reactor->window->fullscreen = false;

	GLFWwindow* window2;
	window2 = glfwCreateWindow(
							reactor->window->getWidth(), 
							reactor->window->getHeight(),
							reactor->window->getName(), 
							NULL, //glfwGetPrimaryMonitor(), 
							NULL);
	
	if(systemWidth == reactor->window->getWidth())
		glfwSetWindowPos(window2, 0, 0);
	else
		glfwSetWindowPos(window2, 500, 100);

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

	reactor->glrenderer->getScene()->setResolution(reactor->window->getWidth(), reactor->window->getHeight());
	reactor->glrenderer->initGL();
	reactor->glrenderer->resizeGL(reactor->window->getWidth(), reactor->window->getHeight());
	
	glfwSetKeyCallback(window2, key_callback);
	glfwSetWindowSizeCallback(window2, window_size_callback);
	glfwSetInputMode(window2, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(window2))
	{
		if (! reactor->shuttingDown)
		{
			reactor->glrenderer->updateGL();
			reactor->glrenderer->displayGL();
		}
		else 
			break;

		glfwSwapBuffers(window2);
		glfwPollEvents();
	}
	return 0;
}

