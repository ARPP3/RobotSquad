#pragma once

#include "stdafx.h"

class COpenGLWinApp
{
public:
	HWND hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	LPCWSTR sAppName;
	LPCWSTR appTitle;

	void ResetTimer();
	void UpdateTimer();
	float sof(float fVal);

	bool InitializeApp(LPCWSTR a_sAppName);
	void RegisterAppClass(HINSTANCE hAppInstance);
	bool CreateAppWindow(LPCWSTR sTitle);

	void AppBody();
	void Shutdown();

	HINSTANCE GetInstance();

	LRESULT CALLBACK MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance; // Application's instance;
	HANDLE hMutex;
	HICON _Icon;

	bool bAppActive; // To check if application is active (not minimized)
	DWORD dwLastFrame;
	float fFrameInterval;
};

enum class Gamepad { LEFT_STICK_X, LEFT_STICK_Y, RIGHT_STICK_X, RIGHT_STICK_Y };

// XBOX Controller Class Definition
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;

	// Mapping of controller buttons to keys
	HWND targetWindow;

	void SetButtons()
	{
		std::map<WORD, string> bn;
		bn[XINPUT_GAMEPAD_A] = "A";
		bn[XINPUT_GAMEPAD_B] = "B";
		bn[XINPUT_GAMEPAD_X] = "X";
		bn[XINPUT_GAMEPAD_Y] = "Y";
		bn[XINPUT_GAMEPAD_DPAD_LEFT] = "Left";
		bn[XINPUT_GAMEPAD_DPAD_RIGHT] = "Right";
		bn[XINPUT_GAMEPAD_DPAD_UP] = "Up";
		bn[XINPUT_GAMEPAD_DPAD_DOWN] = "Down";
		bn[XINPUT_GAMEPAD_LEFT_SHOULDER] = "LB";
		bn[XINPUT_GAMEPAD_RIGHT_SHOULDER] = "RB";
		bn[XINPUT_GAMEPAD_BACK] = "Back";
		bn[XINPUT_GAMEPAD_START] = "Start";
		bn[XINPUT_GAMEPAD_LEFT_THUMB] = "LS";
		bn[XINPUT_GAMEPAD_RIGHT_THUMB] = "RS";

		Buttons = bn;
	}

	float timer = 0.0;

	XINPUT_STATE previous;

public:
	CXBOXController(int playerNumber);

	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(float leftVal = 0, float rightVal = 0);

	// Add a key translation mapping from XINPUT_GAMEPAD_* to a virtual key code (VK_*)
	void AddKeyMapping(WORD, int);
	// Remove a key translation mapping from XINPUT_GAMEPAD_* button
	void RemoveKeyMappingByButton(WORD);
	// Remove a key translation mapping from a virtual key code (VK_*)
	void RemoveKeyMapping(int);
	// Remove all key translation mappings
	void ClearMappings();

	void SetWindow(HWND hWnd)
	{
		targetWindow = hWnd;
	}

	float GetStick(Gamepad input);
	void Refresh();
	std::map<WORD, string> Buttons;
	std::map<WORD, int> keyMap;
};

namespace Keys
{
	int Key(int iKey);
	int Onekey(int iKey);
	extern char kp[256];

	glm::vec2 MousePosition(bool client);
	void SetMouseLock(bool l);
	extern bool locked;
}

extern COpenGLWinApp appMain;

void InitScene(LPVOID), RenderScene(LPVOID);