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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lmCmdLine, int nCmdShow)
{

#ifdef _DEBUG
	reactor->enableConsole();
#endif

	MSG msg;

	/*
		get the resolution for full screen here.. and set w and h accordingly..
		also get all the possible resolutions on a given screen,. and show it to the user if no fullscreen is selected.
	*/
	 
	//	if (reactor->window.adialog(L"Do you want the application to run in FullScreen??", L"You can always switch from fullscreen to window mode by presgrassg <F1> while running the program." ,YES_NO_EXCLAMATION)==IDNO)
	{ 
		// if you want the resolution to be anything other that the default system full screen resolution then uncomment code below.
		reactor->window->setWidth((float)1280);
		reactor->window->setHeight((float)720);

		reactor->window->fullscreen = false;

		if (!reactor->window->createWindow(reactor->window->getName(), reactor->window->getWidth(), reactor->window->getHeight(), 32, reactor->window->fullscreen))
		{
			OutputDebugStringA("window not created exiting\n");
			exit(0);                           
		}
		
		reactor->window->createSplashWindow();

		reactor->glrenderer->initGL();
		
		ShowWindow(reactor->window->splash_hwnd, SW_HIDE);
	 
		// test if the window is getting created
		if(reactor->window->hwnd != NULL)
			ShowWindow(reactor->window->hwnd,SW_SHOW);

		reactor->glrenderer->resizeGL(reactor->window->getWidth(), reactor->window->getHeight());
	} 
/*	else
	{
	RegisterClass
		reactor->window.setWidth(1366);
		reactor->window.setHeight(768);

		reactor->window.fullscreen = true;	
		if (!reactor->window.createWindow(reactor->window.getName(), reactor->window.getWidth(), reactor->window.getHeight(), 32, reactor->window.fullscreen))
		{
			exit(0);                           
		}
		reactor->window.initGL();
		reactor->window.resizeGL(reactor->window.getWidth(), reactor->window.getHeight());	
	}
*/
	while(TRUE)
	{
		while(PeekMessage(&msg, NULL, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		if(msg.message == WM_QUIT)
		{
			reactor->window->closeWindow();
			delete reactor;
			break;
		}

		reactor->glrenderer->updateGL();
		reactor->glrenderer->displayGL();
		SwapBuffers(reactor->window->hdc);
	}
	return ((int)msg.wParam);
}