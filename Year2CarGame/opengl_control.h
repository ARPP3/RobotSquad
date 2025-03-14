#pragma once

//Based on the Megabyte OpenGL Tutorial

#include "stdafx.h"

#define SIMPLE_OPENGL_CLASS_NAME "Simple_openGL_class"

class COpenGLControl
{
public:
	bool InitOpenGL(HINSTANCE hInstance, HWND* a_hWnd, int majorVersion, int minorVersion, void(*a_InitScene)
		(LPVOID), void(*a_RenderScene) (LPVOID), void(*a_ReleaseScene)(LPVOID), LPVOID lpParam);

	void ResizeOpenGLViewportFull();
	
	void Render(LPVOID lpParam);
	void ReleaseOpenGLControl(LPVOID lpParam);

	static void RegisterSimpleOpenGLClass(HINSTANCE hInstance);
	static void UnregisterSimpleOpenGLClass(HINSTANCE hInstance);

	void MakeCurrent();
	void SwapBuffersM();

	HWND*	get_hWnd()	{ return hWnd; }
	HDC		get_hDC()	{ return hDC; }
	HGLRC	get_hRC()	{ return hRC; }

private:
	bool InitGLEW(HINSTANCE hInstance);

	HDC		hDC;	//Handle to the Context Device
	HWND*	hWnd;	//Handle to the Window
	HGLRC	hRC;	//Handle to the Rendering Context

	static bool bClassRegistered;
	static bool bGlewInitialized;
	int iMajorVersion, iMinorVersion;

	void(*InitScene)(LPVOID lpParam), (*RenderScene)(LPVOID lpParam), (*ReleaseScene)(LPVOID lpParam);
};

LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND, UINT, WPARAM, LPARAM);