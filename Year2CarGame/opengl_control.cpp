#include "stdafx.h"

#include "include\GL\wglew.h"

bool COpenGLControl::bClassRegistered = false, COpenGLControl::bGlewInitialized = false;

bool COpenGLControl::InitGLEW(HINSTANCE hInstance)
{
	if (bGlewInitialized)
	{
		return true;
	}

	HWND hWndFake = CreateWindow((LPCWSTR)SIMPLE_OPENGL_CLASS_NAME, (LPCWSTR)"FAKE_WINDOW", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	hDC = GetDC(hWndFake);

	// Choose the False pixel format

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.cColorBits	= 32;
	pfd.cDepthBits	= 32;
	pfd.iLayerType	= PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0)
	{
		return false;
	}

	// Create the false, old style context
	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(hWndFake);

	return true;
}

bool COpenGLControl::InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int iMajorVersion, int iMinorVersion, void(*a_InitScene)(LPVOID),
	void(*a_RenderScene)(LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam)
{
	if (!InitGLEW(hInstance))
	{
		return false;
	}

	hWnd = a_hWnd;
	hDC = GetDC(*hWnd);

	bool bError = false;
	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	if (iMajorVersion <= 2)
	{
		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (iPixelFormat == 0)return false;

		if (!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		// Create the old style context (OpenGL 2.1 and before)
		hRC = wglCreateContext(hDC);
		if (hRC)wglMakeCurrent(hDC, hRC);
		else bError = true;
	}
	else //if (wglewIsSupported("WGL_ARB_pixel_format") == 1)
	{
		//const int iPixelFormatAttribList[] =
		//{
		//	WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		//	WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		//	WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		//	WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		//	WGL_COLOR_BITS_ARB, 32,
		//	WGL_DEPTH_BITS_ARB, 24,
		//	WGL_STENCIL_BITS_ARB, 8,
		//	0 // End of attributes list
		//};

		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0 // End of attributes list
		};

		//int iPixelFormat, iNumFormats;

		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		//wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

		// PFD seems to be only redundant parameter now
		if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
		{
			return false;
		}

		HGLRC tmpContext = wglCreateContext(hDC);
		//Make it current
		wglMakeCurrent(hDC, tmpContext);

		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);

		//Delete the temporary context
		wglDeleteContext(tmpContext);
	
		// If everything went OK
		wglMakeCurrent(hDC, hRC);
	}

	bool bResult = true;

	glewExperimental = GL_TRUE; // Thanks Cameron
	if (!bGlewInitialized)
	{
		if (glewInit() != GLEW_OK)
		{
			//MessageBox(*hWnd, L"Couldn't initialize GLEW!",
			//	L"Fatal Error", MB_ICONERROR);
			bResult = false;
		}
		bGlewInitialized = true;
	}

#if _DEBUG
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	printf("Using OpenGL version %d.%d.\n", OpenGLVersion[0], OpenGLVersion[1]);
#endif

	//else
	//{
	//	bError = true;
	//}

	//if (bError)
	//{
	//	// Generate error messages
	//	wchar_t sErrorMessage[255], sErrorTitle[255];
	//	swprintf(sErrorMessage, L"OpenGL %d.%d is not supported! Please download latest GPU drivers!", iMajorVersion, iMinorVersion);
	//	swprintf(sErrorTitle, L"OpenGL %d.%d Not Supported", iMajorVersion, iMinorVersion);
	//	MessageBox(*hWnd, sErrorMessage, sErrorTitle, MB_ICONSTOP);
	//	return false;
	//}

	RenderScene = a_RenderScene;
	InitScene = a_InitScene;
	ReleaseScene = a_ReleaseScene;

	if (InitScene != NULL)InitScene(lpParam);

	return true;
}

void COpenGLControl::ResizeOpenGLViewportFull()
{
	if (hWnd == NULL)return;
	RECT rRect; GetClientRect(*hWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}

void COpenGLControl::RegisterSimpleOpenGLClass(HINSTANCE hInstance)
{
	if (bClassRegistered)return;
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MsgHandlerSimpleOpenGLClass;
	wc.cbClsExtra = 0; wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)SIMPLE_OPENGL_CLASS_NAME;

	RegisterClassEx(&wc);

	bClassRegistered = true;
}

void COpenGLControl::UnregisterSimpleOpenGLClass(HINSTANCE hInstance)
{
	if (bClassRegistered)
	{
		UnregisterClass((LPCWSTR)SIMPLE_OPENGL_CLASS_NAME, hInstance);
		bClassRegistered = false;
	}
}

LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (uiMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam); // Default window procedure
	}
	return 0;
}

void COpenGLControl::SwapBuffersM()
{
	SwapBuffers(hDC);
}

void COpenGLControl::MakeCurrent()
{
	wglMakeCurrent(hDC, hRC);
}

void COpenGLControl::Render(LPVOID lpParam)
{
	if (RenderScene)RenderScene(lpParam);
}

void COpenGLControl::ReleaseOpenGLControl(LPVOID lpParam)
{
	if (ReleaseScene)ReleaseScene(lpParam);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(*hWnd, hDC);

	hWnd = NULL;
}