/*
	GLSL_base
	Nov 2017, Ashutosh Morwal
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#include "window_handler.h"

//extern WINDOW_WIN32::input_manager ip_x;
using namespace WINDOW_WIN32;
LRESULT CALLBACK events(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern input_manager ip_x;
void keyProcess(WPARAM wParam, LPARAM lParam);

void win32_window_handler::updateLoadStats(int just_another_number)
{
	//scene.load_counter=just_another_number;
	UpdateWindow(splash_hwnd);
	InvalidateRect(splash_hwnd, 0, TRUE);
}

bool win32_window_handler::createDC(BYTE bits)
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),      // Size Of This Pixel Format Descriptor
		1,									// Version Number
		PFD_DRAW_TO_WINDOW |                // Format Must Support Window
		PFD_SUPPORT_OPENGL |                // Format Must Support OpenGL
		PFD_GENERIC_ACCELERATED | 
		PFD_DOUBLEBUFFER,                   // Must Support Double Buffering
		PFD_TYPE_RGBA,                      // Request An RGBA Format
		bits,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                   // Color Bits Ignored
		0,									// No Alpha Buffer
		0,				                    // Shift Bit Ignored
		0,					                // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		16,									// 16Bit Z-Buffer (Depth Buffer)
		0,									// No Stencil Buffer
		0,									// No Auxiliary Buffer
		0,				                    // Main Drawing Layer
		0,									// Reserved
		0, 0, 0                             // Layer Masks Ignored
	};
	

	/*	if (!(hdc = GetDC(hwnd)))             
		{
			closeWindow();                         
			MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;                           
		}
	*/
	
	hdc = GetDC(hwnd);
	/*if(!hdc == BeginPaint(hwnd, &ps))
	{
		closeWindow();                         
		MessageBox(NULL,L"Can't Create A GL Device Context.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           		
	}*/


	GLuint pixelformat;
	if (!(pixelformat=ChoosePixelFormat(hdc, &pfd)))             
	{
		closeWindow();                         
		MessageBoxW(NULL,L"Can't Find A Suitable PixelFormat.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                            
	}

	if(!SetPixelFormat(hdc, pixelformat, &pfd))               
	{
		closeWindow();                         
		dialog(L"Can't Set The PixelFormat.",L"ERROR", DIALOG_FLAGS::ERR);
		return FALSE;
	}

	if (!(hrc = wglCreateContext(hdc)))                   
	{
		closeWindow();                         
		dialog(L"Can't Create A GL Rendering Context.\nwglCreateContext failed",L"ERROR",DIALOG_FLAGS::ERR);
		return FALSE;
	}

	if(!wglMakeCurrent(hdc, hrc))                        
	{
		closeWindow();                         
		dialog(L"Can't Activate A GL Rendering Context.\nwglMakeCurrent failed",L"ERROR",DIALOG_FLAGS::ERR);
		return FALSE;                           
	}
	return TRUE;
}

bool win32_window_handler::createWindow(LPTSTR title, float w, float h, int bits, bool isFullscreen)
{
	WNDCLASSEX wndClass;

	DWORD winExStyle;
	DWORD winStyle;

	RECT rc;

	rc.left		= (long) 0;
	rc.top		= (long) 0;	
	rc.right	= (long) w;
	rc.bottom	= (long) h;

	fullscreen  = isFullscreen;

	hInstance = GetModuleHandle(NULL);

	wndClass.cbSize			= sizeof  (WNDCLASSEX);
	wndClass.style			= CS_OWNDC; 
	wndClass.cbClsExtra		= 0;
	
	wndClass.cbWndExtra		= 0;
	wndClass.lpszClassName  = TEXT("OpenGL_win32");
	wndClass.lpszMenuName	= NULL;
	
	wndClass.lpfnWndProc	= events;
	wndClass.hInstance		= hInstance;
	wndClass.hbrBackground	= NULL;
	
	HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100));
	HICON hMyIcon_sm = LoadIcon(hInstance, MAKEINTRESOURCE(101));
	wndClass.hIcon = hMyIcon;
	wndClass.hIconSm = hMyIcon_sm;
	wndClass.hCursor		= LoadCursor (NULL, IDC_ARROW);

	RegisterClassEx (&wndClass);

	if(fullscreen) //fullscrreen is supported
	{
		DEVMODE screensettings;
		memset(&screensettings, 0, sizeof(screensettings));
			
		screensettings.dmSize = sizeof(screensettings);
		screensettings.dmPelsWidth = (DWORD)w;
		screensettings.dmPelsHeight = (DWORD)h;
		screensettings.dmBitsPerPel = bits;
		screensettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&screensettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBoxW(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Ugrassg Windowed Mode", L"Fullscreen not supported", MB_OK | MB_ICONEXCLAMATION);
			fullscreen = false;
		}

		winExStyle = WS_EX_APPWINDOW 	|WS_EX_TOPMOST;
		winStyle = WS_POPUP;                       
		//ShowCursor(FALSE); 
	}
	else
	{
		winExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;// | WS_EX_TOPMOST;           
		winStyle = WS_OVERLAPPEDWINDOW;  
	}
	ShowCursor(FALSE);    

	//not applicable in fullscreen
	AdjustWindowRectEx(&rc, winStyle, FALSE, winExStyle);  
	hwnd = CreateWindowEx
		(
			winExStyle,
			TEXT("OpenGL_win32"), 
			title,
			winStyle,
			0,					 //x 
			0,					 //y
			rc.right-rc.left,	 //w
			rc.bottom-rc.top,	 //h
			NULL,
			NULL, 
			hInstance, 
			NULL);
	
	if(fullscreen)
		SetWindowLong(hwnd, GWL_STYLE, 0);
	
#ifdef _DEBUG	
	SetWindowPos(
		hwnd, 
		HWND_TOPMOST, 
		300,
		169, 
		0, 0, 
		SWP_NOSIZE);
#endif

	if(hwnd == NULL)
	{
		closeWindow();
		MessageBox(NULL, TEXT("Window not created"), TEXT("error.."), 0);
		return FALSE;
	}

	if(!createDC(bits))
		return false;
	else
		return true;
}//create window END

void win32_window_handler::createSplashWindow()
{
		// splash 
		WNDCLASSEX wc;

		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = 0;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = TEXT("GLSL Engine");
		wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc))
		{
			MessageBoxW(NULL, L"Window Registration Failed!", L"Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}

		// Step 2: Creating the Window
		splash_hwnd = CreateWindowExW(
			WS_EX_CLIENTEDGE,
			L"OpenGL_win32",
			L"GLSL Engine",
			WS_POPUPWINDOW,
			(int)GetSystemMetrics(SM_CXSCREEN)/2-300,
			(int)GetSystemMetrics(SM_CYSCREEN)/2-169, 
			600,
			338,
			NULL, 
			NULL, 
			hInstance, 
			NULL);

		if(splash_hwnd == NULL)
		{
			MessageBoxW(NULL, L"Window Creation Failed!", L"Error!",
				MB_ICONEXCLAMATION | MB_OK);
		}
		SetWindowPos(splash_hwnd, HWND_TOPMOST, 10, 10, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		//hide the splash window while debugging.
#ifndef _DEBUG
		ShowWindow(splash_hwnd, SW_SHOW);
#endif
//		InvalidateRect(splash_hwnd, 0, TRUE);
		UpdateWindow(splash_hwnd);
}

win32_window_handler::~win32_window_handler()
{
	//renderer->shutdownGL();
	//closeWindow();
}


void win32_window_handler::closeWindow()
{
	if(fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}

	if (!wglMakeCurrent(NULL,NULL))                 
	{
		dialog(L"Release Of DC And RC Failed.",L"SHUTDOWN ERROR",1);
	}

	if (!wglDeleteContext(hrc))                 
	{
	   dialog(L"Release Rendering Context Failed.",L"SHUTDOWN ERROR",1);
	}

	hrc = NULL;

	ReleaseDC(hwnd, hdc);
	UnregisterClassW(L"OpenGL_win32", hInstance);
	DestroyWindow(hwnd); //sends message WM_DESTROY in which we call PostQuitMessage(0);
}

LPTSTR win32_window_handler::getName(void) { return m_windowName; }

void win32_window_handler::setName(LPTSTR title) { m_windowName = title; }

// ToDo: this function will be a part of renderer.
// void win32_window_handler::resizeGL(float w, float h) { resize(w,h); }

float win32_window_handler::getWidth() { return m_width; }
	
void win32_window_handler::setWidth(float w) { m_width = w; }

float win32_window_handler::getHeight() { return m_height; }
	
void win32_window_handler::setHeight(float h) { m_height = h; }


int win32_window_handler::dialog(LPCWSTR title, LPCWSTR msg, int flag)
{
	int response = 0;
	switch(flag)
	{
		case ERR:
			response = MessageBoxW(hwnd, msg, title, MB_OK | MB_ICONERROR);
			break;
		case INFO:
			response = MessageBoxW(hwnd, msg, title, MB_OK | MB_ICONINFORMATION);
			break;
		case YES_NO_INFO:
			response = MessageBoxW(hwnd, msg, title, MB_YESNO | MB_ICONINFORMATION);
			break;
		case YES_NO_EXCLAMATION:
			response = MessageBoxW(NULL, msg, title, MB_YESNO | MB_ICONEXCLAMATION);
			break;
	}
	return response;
}

LRESULT CALLBACK events(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{	
		case WM_CREATE:	
			break;
		case WM_KEYDOWN:
			keyProcess(wParam, lParam);
			break;
		case WM_KEYUP:
			switch (wParam)
			{
				/*
			case 'W':	
				printf("keyup w\n");
				w = false; 
				break;
			case 'S':	
				printf("keyup s\n");
				s = false;
				break;
			case 'A':	
				printf("keyup a\n");
				a = false;
				break;
			case 'D':	
				printf("keyup d\n");
				d = false;
				break;
				*/
			}
			break;
	}
	return (DefWindowProc(hwnd, msg, wParam, lParam));
} 

void keyProcess(WPARAM wParam, LPARAM lParam)
{
	printf("keydwn \n");
//	renderer->getScene();
//	ip_x.keyProcess(wParam, lParam);
//	reactor->glrenderer->getScene();

	switch(wParam )
	{
		/*
		case 'W':	
			printf("keydwn w\n");
			w = true;
			break;
		case 'S':	
			printf("keydwn s\n");
			s = true;
			break;
		case 'A':	
			printf("keydwn a\n");
			a = true;
			break;
		case 'D':	
			printf("keydwn d\n");
			d = true;
			break;
		*/
		case 27: PostQuitMessage(0); break;
	}
}

