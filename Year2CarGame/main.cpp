#include "stdafx.h"

int WIDTH = 960;
int HEIGHT = 540;
int MAX_WIDTH = 960;
int MAX_HEIGHT = 540;

bool WIREFRAME = false;
bool BLOOM_ENABLED = true;
float BLOOM_THRESHOLD = 0.2f;
int BLOOM_BLUR_PASSES = 4;
bool PHYSICS_DEBUG = false;

bool isFullScreen = false;
int gameState = 0;

COpenGLWinApp appMain;
fMod FMod;

CXBOXController *Controller;

char Keys::kp[256] = { 0 };
bool Keys::locked = true;

void SetVSync(bool sync);

CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;

	SetButtons();
}

XINPUT_STATE CXBOXController::GetState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CXBOXController::Vibrate(float leftVal, float rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = (int)(leftVal * 65535.f);
	Vibration.wRightMotorSpeed = (int)(rightVal * 65535.f);

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);

	timer = 0.2f;
}

void CXBOXController::AddKeyMapping(WORD button, int key)
{
	keyMap.erase(button);
	keyMap.insert(std::map<WORD, int>::value_type(button, key));
}

void CXBOXController::RemoveKeyMapping(int key)
{
	for (auto it = keyMap.begin(); it != keyMap.end(); ++it)
		if (it->second == key)
		{
		keyMap.erase(it);
		break;
		}
}

void CXBOXController::RemoveKeyMappingByButton(WORD button)
{
	keyMap.erase(button);
}

void CXBOXController::ClearMappings()
{
	keyMap.clear();
}

float CXBOXController::GetStick(Gamepad input)
{
	float deadzoneX = 0.05f;
	float deadzoneY = 0.02f;

	if (input == Gamepad::LEFT_STICK_X)
	{
		float normLX = fmaxf(-1, (float)GetState().Gamepad.sThumbLX / 32767);
		float leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		if (deadzoneX > 0)
		{
			return leftStickX /= 1 - deadzoneX;
		}
	}
	if (input == Gamepad::LEFT_STICK_Y)
	{
		float normLY = fmaxf(-1, (float)GetState().Gamepad.sThumbLY / 32767);
		float leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));;
		if (deadzoneY > 0)
		{
			return leftStickY /= 1 - deadzoneY;
		}
	}

	if (input == Gamepad::RIGHT_STICK_X)
	{
		float normRX = fmaxf(-1, (float)GetState().Gamepad.sThumbRX / 32767);
		float rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		if (deadzoneX > 0)
		{
			return rightStickX /= 1 - deadzoneX;
		}
	}
	if (input == Gamepad::RIGHT_STICK_Y)
	{
		float normRY = fmaxf(-1, (float)GetState().Gamepad.sThumbRY / 32767);
		float rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));;
		if (deadzoneY > 0)
		{
			return rightStickY /= 1 - deadzoneY;
		}
	}
	return 0.f;
}

void CXBOXController::Refresh()
{
	XINPUT_STATE state = GetState();
	previous = state;

	if (targetWindow != NULL)
	{
		for (auto button : Buttons)
		{
			// If button is pushed
			if ((state.Gamepad.wButtons & button.first) != 0)
			{
				// Get key mapping or use XINPUT_GAMEPAD_* value if no mapping exists
				WORD mapping = (keyMap.find(button.first) != keyMap.end() ?
					keyMap.find(button.first)->second : button.first);

				// Send keyboard event
				SendMessage(targetWindow, WM_KEYDOWN, mapping,
					((previous.Gamepad.wButtons & button.first) == 0 ? 0 << 30 : 1 << 30));
			}

			// Checking for button release events, will cause the state
			// packet number to be incremented
			if (previous.dwPacketNumber < state.dwPacketNumber)
			{
				// If the button was pressed but is no longer pressed
				if ((state.Gamepad.wButtons & button.first) == 0
					&& (previous.Gamepad.wButtons & button.first) != 0)
				{
					// Get key mapping if one exists
					WORD mapping = (keyMap.find(button.first) != keyMap.end() ?
						keyMap.find(button.first)->second : button.first);

					// Send keyboard event
					SendMessage(targetWindow, WM_KEYUP, mapping, 0);
				}
			}
		}
	}

	if (timer <= 0.0)
	{
		Vibrate(0.0f, 0.0f);
	}
	timer -= appMain.sof(1.0f);
}

int Keys::Key(int iKey)
{
	XINPUT_STATE state = Controller->GetState();
	for (auto button : Controller->Buttons)
	{
		// If button is pushed
		if ((state.Gamepad.wButtons & button.first) != 0)
		{
			// Get key mapping or use XINPUT_GAMEPAD_* value if no mapping exists
			WORD mapping = (Controller->keyMap.find(button.first) != Controller->keyMap.end() ?
				Controller->keyMap.find(button.first)->second : button.first);

			// Send keyboard event
			if (mapping == iKey)
			{
				return 1;
			}
		}
	}

	return (GetAsyncKeyState(iKey) >> 15) & 1;
}

int Keys::Onekey(int iKey)
{
	if (Key(iKey) && !kp[iKey]){ kp[iKey] = 1; return 1; }
	if (!Key(iKey))kp[iKey] = 0;
	return 0;
}

glm::vec2 Keys::MousePosition(bool client)
{
	POINT t1, t2;
	RECT r;
	GetCursorPos(&t1);

	GetWindowRect(appMain.hWnd, &r);

	if (locked)
	{
		t2.x = WIDTH / 2; t2.y = HEIGHT / 2;
		ClientToScreen(appMain.hWnd, &t2);
		SetCursorPos(t2.x, t2.y);
		return glm::vec2(t1.x - t2.x, t1.y - t2.y);
	}

	if (!client)
	{
		float ratiox = ((float)MAX_WIDTH / ((float)WIDTH));
		float ratioy = ((float)MAX_HEIGHT / ((float)HEIGHT));

		float x = (ratiox * (float)t1.x) - (ratiox * (float)r.left);
		float y = (ratioy * (float)t1.y) - (ratioy * (float)r.top);

		return glm::vec2(x - 8.f, y - 30.f);
	}

	ClientToScreen(appMain.hWnd, &t1);
	return glm::vec2(t1.x, t1.y);
}

void Keys::SetMouseLock(bool l)
{
	locked = l;
}

void COpenGLWinApp::ResetTimer()
{
	dwLastFrame = GetTickCount();
	fFrameInterval = 0.0f;
}

void COpenGLWinApp::UpdateTimer()
{
	DWORD dwCur = GetTickCount();
	fFrameInterval = float(dwCur - dwLastFrame)*0.001f;
	dwLastFrame = dwCur;
}

float COpenGLWinApp::sof(float fVal)
{
	return fVal*fFrameInterval;
}

bool COpenGLWinApp::InitializeApp(LPCWSTR a_sAppName)
{
	sAppName = a_sAppName;
	hMutex = CreateMutex(NULL, 1, sAppName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, (LPCWSTR)"This application already runs!", (LPCWSTR)"Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
	return 1;
}

LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
}

void COpenGLWinApp::RegisterAppClass(HINSTANCE a_hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = sAppName;

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

bool COpenGLWinApp::CreateAppWindow(LPCWSTR sTitle)
{
	hWnd = CreateWindowEx(0, sAppName, sTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

	if (!oglControl.InitOpenGL(hInstance, &hWnd, 4, 2, InitScene, RenderScene, NULL, &oglControl))
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);

	// Just to send WM_SIZE message again
	UpdateWindow(hWnd);

	appTitle = sTitle;

	_Icon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE); //changed here

	if (_Icon)
	{
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)_Icon);
	}
	else
	{
		MessageBox(NULL, TEXT("Problem while loading the Icon."), TEXT("EROOR !!!!"), MB_OK);
	}

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	MAX_WIDTH = desktop.right;
	MAX_HEIGHT = desktop.bottom;

	return true;
}

void COpenGLWinApp::AppBody()
{
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break; // If the message was WM_QUIT then exit application
			}
			else
			{
				TranslateMessage(&msg); // Otherwise send message to appropriate window
				DispatchMessage(&msg);
			}
		}
		else if (bAppActive)
		{
			UpdateTimer();
			oglControl.Render(&oglControl);
		}
		else Sleep(200); // Do not consume processor power if application isn't active
	}
}

void COpenGLWinApp::Shutdown()
{
	oglControl.ReleaseOpenGLControl(&oglControl);

	DestroyWindow(hWnd);
	UnregisterClass(sAppName, hInstance);
	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	ReleaseMutex(hMutex);
}

LRESULT CALLBACK COpenGLWinApp::MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (uiMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_ACTIVATE:
	{
		switch (LOWORD(wParam))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			bAppActive = true;
			break;
		case WA_INACTIVE:
			bAppActive = false;
			break;
		}
		break;
	}

	case WM_SIZE:
		oglControl.ResizeOpenGLViewportFull();
		SceneCamera.AR = float(LOWORD(lParam)) / float(HIWORD(lParam));
		SceneCamera.mProjection = glm::perspective(SceneCamera.FOV, float(LOWORD(lParam)) / float(HIWORD(lParam)), 0.01f, 1000.0f);
		SceneCamera.mDecalProjection = glm::perspective(SceneCamera.FOV, float(LOWORD(lParam)) / float(HIWORD(lParam)), 0.0101f, 1010.0f);
		SceneCamera.mDecalProjection2 = glm::perspective(SceneCamera.FOV, float(LOWORD(lParam)) / float(HIWORD(lParam)), 0.0102f, 1020.0f);
		WIDTH = LOWORD(lParam);// +16;
		HEIGHT = HIWORD(lParam);// +38;
		break;

	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}

HINSTANCE COpenGLWinApp::GetInstance()
{
	return hInstance;
}

#if _DEBUG
	int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
#else
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
#endif
{
	SetVSync(false);

	if (!appMain.InitializeApp(L"car_game"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	Controller = new CXBOXController(1);

	if (!appMain.CreateAppWindow(L"Robot-Squad"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();
	
	return 0;
}

void SetVSync(bool sync)
{
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = NULL;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(sync);
	}
}