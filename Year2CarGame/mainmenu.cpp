#include "stdafx.h"

void mainMenu::Init()
{
	//Create PassThrough Shader Program 
	passThrough.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	passThrough.frag.LoadShader(".\\Assets\\Shaders\\PassThrough.frag", GL_FRAGMENT_SHADER);
	passThrough.CreateProgram();
	passThrough.AddShaderToProgram(&passThrough.vert);
	passThrough.AddShaderToProgram(&passThrough.frag);
	passThrough.LinkProgram();

	//Create Default Shader Program 
	basicShaderProgram.vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	basicShaderProgram.frag.LoadShader(".\\Assets\\Shaders\\DirectionalBasic.frag", GL_FRAGMENT_SHADER);
	basicShaderProgram.CreateProgram();
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.vert);
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.frag);
	basicShaderProgram.LinkProgram();

	/*mainMenuBackground.LoadTexture(".\\Assets\\Texture\\Background.png", true);
	mainMenuBackground.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);
	mousePointer.LoadTexture(".\\Assets\\Texture\\mouseArrow.png", true);
	mousePointer.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);*/

	background.LoadTexture(".\\Assets\\Textures\\mainMenu.jpg", false);
	background.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);


	levelSelectTex[0].LoadTexture(".\\Assets\\Textures\\tutorial1_level_s.png", false);
	levelSelectTex[0].SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	levelSelectTex[1].LoadTexture(".\\Assets\\Textures\\level1_level_s.png", false);
	levelSelectTex[1].SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	controlsTex[0].LoadTexture(".\\Assets\\Textures\\controls_controller.png", false);
	controlsTex[0].SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	controlsTex[1].LoadTexture(".\\Assets\\Textures\\controls_keyboard.png", false);
	controlsTex[1].SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	creditTex.LoadTexture(".\\Assets\\Textures\\credits.png", false);
	creditTex.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);


	mouse.LoadTexture(".\\Assets\\Textures\\mouseArrow.png", false);
	mouse.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	robot.model.LoadFromFile(".\\Assets\\Models\\fullRobot.mesh");
	robot.texture.LoadTexture(".\\Assets\\Textures\\RobotTexture.jpg", true);
	robot.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	robot.SetShaderProgram(&basicShaderProgram);

	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\backgroundLoop.mp3", FMOD_3D, 0, &backgroundAudio));
	backgroundAudio->setMode(FMOD_LOOP_NORMAL);
	//channel->setLoopCount(-1);
	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, backgroundAudio, false, &FMod.channel));
}

void mainMenu::Update()
{
	Controller->AddKeyMapping(XINPUT_GAMEPAD_A, VK_RETURN);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_X, 32);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_START, VK_RETURN);

	Controller->Refresh();

	if (Keys::Onekey(VK_DOWN) || Keys::Onekey('S'))
	{
		positionY += 1;
		positionY = positionY % 4;

		POINT p;
		float ratioX = ((float)WIDTH / (float)MAX_WIDTH);
		float ratioY = ((float)HEIGHT / (float)MAX_HEIGHT);

		switch (positionY)
		{
		case 0:
			p.x = 230.f * ratioX; p.y = 308.f * ratioY;
			break;
		case 1:
			p.x = 230.f * ratioX; p.y = 450.f * ratioY;
			break;
		case 2:
			p.x = 230.f * ratioX; p.y = 596.f * ratioY;
			break;
		case 3:
			p.x = 230.f * ratioX; p.y = 743.f * ratioY;
			break;
		}

		ClientToScreen(appMain.hWnd, &p);
		SetCursorPos(p.x, p.y);
	}
	if (Keys::Onekey(VK_UP) || Keys::Onekey('W'))
	{
		positionY += 3;
		positionY = positionY % 4;

		POINT p;
		float ratioX = ((float)WIDTH / (float)MAX_WIDTH);
		float ratioY = ((float)HEIGHT / (float)MAX_HEIGHT);

		switch (positionY)
		{
		case 0:
			p.x = 230.f * ratioX; p.y = 308.f * ratioY;
			break;
		case 1:
			p.x = 230.f * ratioX; p.y = 450.f * ratioY;
			break;
		case 2:
			p.x = 230.f * ratioX; p.y = 596.f * ratioY;
			break;
		case 3:
			p.x = 230.f * ratioX; p.y = 743.f * ratioY;
			break;
		}

		ClientToScreen(appMain.hWnd, &p);
		SetCursorPos(p.x, p.y);
	}

	if (menuState == 1 || menuState == 2)
	{
		if (Keys::Onekey(VK_LEFT) || Keys::Onekey(VK_RIGHT) || Keys::Onekey('A') || Keys::Onekey('D'))
		{
			if (menuState == 1)
			{
				levelSelect += 1;
				levelSelect = levelSelect % 2;
			}
			else if (menuState == 2)
			{
				controlsSelect += 1;
				controlsSelect = controlsSelect % 2;
			}
		}
	}

	if (Keys::Onekey(48))
	{
		isFullScreen = !isFullScreen;
		switch (isFullScreen)
		{
		case 0:
			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = 960;
			rect.bottom = 540;
			SetWindowLongPtr(appMain.hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(appMain.hWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);

			WIDTH = 960;
			HEIGHT = 540;
			break;

		case 1: // This is the commonly used style for FullScreen
			SetWindowLongPtr(appMain.hWnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(appMain.hWnd, 0, 0, MAX_WIDTH, MAX_HEIGHT, TRUE);

			WIDTH = MAX_WIDTH;
			HEIGHT = MAX_HEIGHT;
			break;
		}
	}

	if (Keys::Onekey(VK_LBUTTON) || Keys::Onekey(VK_RETURN))
	{
		if (Keys::MousePosition(false).x > X_PERCENT(3.75f) && Keys::MousePosition(false).x < X_PERCENT(25.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(28.333f) && Keys::MousePosition(false).y < Y_PERCENT(41.f))
		{//Play Button
			menuState = 1;
			controlsSelect = -1;
			if (levelSelect == 0)
			{
				gameState = 2;	///TEMPORARY
				backgroundAudio->release();
			}
			if (levelSelect == 1)
			{
				gameState = 3;	///TEMPORARY
				backgroundAudio->release();
			}
			else
			{
				levelSelect = 0;
			}
		}
		if (Keys::MousePosition(false).x > X_PERCENT(3.75f) && Keys::MousePosition(false).x < X_PERCENT(25.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(44.f) && Keys::MousePosition(false).y < Y_PERCENT(57.f))
		{//Instructions Button
			menuState = 2;
			controlsSelect = 0;
			levelSelect = -1;
		}
		if (Keys::MousePosition(false).x > X_PERCENT(3.75f) && Keys::MousePosition(false).x < X_PERCENT(25.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(60.f) && Keys::MousePosition(false).y < Y_PERCENT(73.f))
		{//Credits Button
			menuState = 3;
			levelSelect = -1;
			controlsSelect = -1;
		}

		if (Keys::MousePosition(false).x > X_PERCENT(3.75f) && Keys::MousePosition(false).x < X_PERCENT(25.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(75.88f) && Keys::MousePosition(false).y < Y_PERCENT(89.111f))
		{//Quit Button
			ExitProcess(0);
		}

		if ((Keys::MousePosition(false).x > X_PERCENT(33.4f) && Keys::MousePosition(false).x < X_PERCENT(44.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(82.f) && Keys::MousePosition(false).y < Y_PERCENT(95.f)))
		{//LevelSelectLeft Button
			if (menuState == 1)
			{	//Level Selection
				levelSelect += 1;
				levelSelect = levelSelect % 2;
			}
			if (menuState == 2)
			{	//Controls Selection
				controlsSelect += 1;
				controlsSelect = controlsSelect % 2;
			}
		}
		if ((Keys::MousePosition(false).x > X_PERCENT(82.f) && Keys::MousePosition(false).x < X_PERCENT(92.f) &&
			Keys::MousePosition(false).y > Y_PERCENT(82.f) && Keys::MousePosition(false).y < Y_PERCENT(95.f)))
		{//LevelSelectLeft Button
			if (menuState == 1)
			{	//Level Selection
				levelSelect += 1;
				levelSelect = levelSelect % 2;
			}
			if (menuState == 2)
			{	//Controls Selection
				controlsSelect += 1;
				controlsSelect = controlsSelect % 2;
			}
		}
	}

	if (Keys::Onekey(27)) { ExitProcess(0); }
	
	Controller->AddKeyMapping(XINPUT_GAMEPAD_START, 'P');
	Controller->AddKeyMapping(XINPUT_GAMEPAD_A, VK_UP);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_X, VK_DOWN);

	Controller->Refresh();
}

void mainMenu::Draw()
{
	ShowCursor(FALSE);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	passThrough.UseProgram();
	passThrough.setUniform("uTex", 0);
	passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

	background.BindTexture(0);
	DrawFullScreenQuad();


	basicShaderProgram.UseProgram();
	basicShaderProgram.setUniform("viewMatrix", glm::lookAt(glm::vec3(1.25, 1.35, -6.0), glm::vec3(1.25, 0.85, 0.0), glm::vec3(0.0, 1.0, 0.0)));
	basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
	basicShaderProgram.setUniform("gSampler", 0);
	basicShaderProgram.setUniform("ambient", 0.2f);

	glEnable(GL_DEPTH_TEST);
	robot.matrix = glm::rotate(glm::mat4x4(1.0), (float)GetTickCount() / 15.f, glm::vec3(0.0, 1.0, 0.0));
	robot.Draw();
	glDisable(GL_DEPTH_TEST);

	passThrough.UseProgram();

	if (menuState == 1)
	{
		passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

		levelSelectTex[levelSelect].BindTexture(0);
		DrawFullScreenQuad();
	}
	
	if (menuState == 2)
	{
		passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

		controlsTex[controlsSelect].BindTexture(0);
		DrawFullScreenQuad();
	}

	if (menuState == 3)
	{
		passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

		creditTex.BindTexture(0);
		DrawFullScreenQuad();
	}

	Keys::locked = false;
	glm::mat4x4 modelMatrix = ScreenRectangle((int)Keys::MousePosition(false).x, (int)Keys::MousePosition(false).y, X_PERCENT(5.25f), Y_PERCENT(8.666f));
	passThrough.setUniform("modelMatrix", modelMatrix);
	mouse.BindTexture(0);
	DrawFullScreenQuad();
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}