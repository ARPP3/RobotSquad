#include "stdafx.h"

Tutorial1 lvl_tutorial1;
Level1 lvl_level1;
BlowUp lvl_blowup;
mainMenu  menu;
//cLevel lvl_tutorial2;
//cLevel lvl_easy;
//cLevel lvl_medium;
//cLevel lvl_hard;
cLevel *currentLevel;

GLuint _FullScreenQuadVAO = GL_NONE;
GLuint _FullScreenQuadVBO = GL_NONE;

bool pause = false;
float gameSpeed = 1.0f;
float newGameSpeed = 1.0f;

//For the high pass, I set the frame rate history to be 1/60th of a second
float frameRateHistory[11] =
{
	0.016f, 0.016f, 0.016f, 0.016f, 0.016f,0.016f,
	0.016f, 0.016f, 0.016f, 0.016f, 0.016f
};

int currentFrame = 0;
float timeStep = 0.f, oldTimeStep = 0.016f;

void InitFullScreenQuad()
{
	float VBO_DATA[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f
	};

	int vertexSize = 6 * 3 * sizeof(float);
	int uvSize = 6 * 2 * sizeof(float);

	glGenVertexArrays(1, &_FullScreenQuadVAO);
	glBindVertexArray(_FullScreenQuadVAO);

	glEnableVertexAttribArray(0); //vertices
	glEnableVertexAttribArray(1); //UV coordinates

	glGenBuffers(1, &_FullScreenQuadVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, _FullScreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + uvSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void DrawFullScreenQuad()
{
	glBindVertexArray(_FullScreenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}

glm::mat4x4 ScreenRectangle(int x, int y, int w, int h)
{
	glm::mat4 m = glm::mat4x4(1.0);
	m = glm::translate(m, glm::vec3(-1 + (2 * ((float)x / MAX_WIDTH)), -1 + (2 * (1 - ((float)y / MAX_HEIGHT))), 0.f));
	return glm::scale(m, glm::vec3((float) w / MAX_WIDTH, (float) h / MAX_HEIGHT, 0.f));
}

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{
	newGameSpeed = 1.0f;

	if (((bulletObject*)obj2->getCollisionObject()->getUserPointer())->id != 0)
	{
		if (currentLevel != nullptr)
		{
			if (cp.getAppliedImpulse() >= 300.0f)
			{
				printf("\nCollision between %i and %i", ((bulletObject*)obj1->getCollisionObject()->getUserPointer())->id,
					((bulletObject*)obj2->getCollisionObject()->getUserPointer())->id);

				Controller->Vibrate(1.0, 1.0);
				FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, currentLevel->player.crash, false, &currentLevel->player.clawChannel));
			}
		}
	}
	
	return false;
}

void InitScene(LPVOID lpParam)
{
	FMod.Startup();
	menu.Init();
	gContactAddedCallback = callbackFunc;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Controller->SetWindow(appMain.hWnd);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_BACK, 'R');
	Controller->AddKeyMapping(XINPUT_GAMEPAD_A, VK_UP);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_B, 32);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_X, VK_DOWN);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_DPAD_LEFT, VK_LEFT);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_DPAD_RIGHT, VK_RIGHT);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_DPAD_UP, VK_UP);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_DPAD_DOWN, VK_DOWN);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_RIGHT_SHOULDER, 32);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_LEFT_SHOULDER, 32);
	Controller->AddKeyMapping(XINPUT_GAMEPAD_START, 'P');

	SceneCamera.SetPos(glm::vec3(0, 0.001, 10));
	SceneCamera.UpdateViewMatrix();

	InitFullScreenQuad();

	glClearColor(0.f, 0.f, 0.f, 0.f);
	// Cornflower Blue
	//glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
}

void RenderScene(LPVOID lpParam)
{
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// We just clear color
	if (!pause)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	gameSpeed = Lerp(gameSpeed, newGameSpeed, appMain.sof(1.0f));

	switch (gameState)
	{
	case 1:
		//This is the main menu
		if (currentLevel != nullptr)
		{
			currentLevel->Release();
			currentLevel = nullptr;
			//currentLevel->physics.Unload();
			
			
			gameState = 0; //set to Default
			menu = mainMenu();
			menu.Init();
		}
		break;
	case 2:
		//This is tutorial 1
		lvl_tutorial1 = Tutorial1();
		currentLevel = &lvl_tutorial1;
		lvl_tutorial1.Init();
		gameState = 0; //set to Default
		break;
	case 3:
		//This is level 1
		lvl_level1 = Level1();
		currentLevel = &lvl_level1;
		lvl_level1.Init();
		gameState = 0; //set to Default
		break;
	case 4:	
		//This is the apocolypse cutscene
		currentLevel->Release();
		lvl_blowup = BlowUp();
		currentLevel = &lvl_blowup;
		lvl_blowup.Init();
		gameState = 0; //set to Default
		break;
	}

	if (Keys::Onekey('P') && currentLevel != nullptr)
	{
		pause = !pause;
		currentLevel->Channels->setPaused(pause);
	}

	if (currentLevel != nullptr)
	{
		Controller->Refresh();

		if (!pause)
		{
			currentLevel->Update();
			currentLevel->HandleEvents();
			currentLevel->Update();

			SceneCamera.UpdateProjMatrix();
			currentLevel->Draw();
		}
	}

	if (PHYSICS_DEBUG && currentLevel != nullptr && (!pause))
	{
		glDisable(GL_DEPTH_TEST);
		currentLevel->physics.world->debugDrawWorld();
		glEnable(GL_DEPTH_TEST);
	}

	if (currentLevel == nullptr)
	{
		Controller->Refresh();

		menu.Update();
		menu.Draw();
	}

	if (!pause)
	{
		oglControl->SwapBuffersM();
	}

	if (currentLevel != nullptr && (!pause))
	{
		//currentLevel->physics.world->stepSimulation(glm::min(appMain.sof(1.0), 1.f / framerate_limit), 10, btScalar(1.f / 60.f));
		//currentLevel->physics.world->stepSimulation(1.f / framerate_limit, 10, btScalar(1.f / 60.f));
		//printf("\nSOF: %f", appMain.sof(1.0));

		frameRateHistory[currentFrame] = appMain.sof(1.0f);
		currentFrame = (currentFrame + 1) % 11;

		float high[2] = { 0.0f, 0.0f }, low[2] = { 5000.0f, 5000.0f };
		float remaining1[9];
		float remaining2[7];
		int counter = 0;


		for (int i = 0; i < 11; i++)
		{
			if (frameRateHistory[i] > high[0])
			{
				if (high[1] != 0.0f)
				{
					remaining1[counter] = high[1];
					counter++;
				}
				high[1] = high[0];
				high[0] = frameRateHistory[i];
			}
			else if (frameRateHistory[i] > high[1])
			{
				if (high[1] != 0.0f)
				{
					remaining1[counter] = high[1];
					counter++;
				}
				high[1] = frameRateHistory[i];
			}
			else
			{
				remaining1[counter] = frameRateHistory[i];
				counter++;
			}
		}
		counter = 0;

		for (int i = 0; i < 9; i++)
		{
			if (remaining1[i] < low[0])
			{
				if (low[1] != 5000.0f)
				{
					remaining2[counter] = low[1];
					counter++;
				}
				low[1] = low[0];
				low[0] = remaining1[i];
			}
			else if (remaining1[i] < low[1])
			{
				if (low[1] != 5000.0f)
				{
					remaining2[counter] = low[1];
					counter++;
				}
				low[1] = remaining1[i];
			}
			else
			{
				remaining2[counter] = remaining1[i];
				counter++;
			}
		}
		counter = 0;

		float mean = (remaining2[0] + remaining2[1] + remaining2[2] + remaining2[3] +
			remaining2[4] + remaining2[5] + remaining2[6]) / 7.f;

		timeStep = Lerp(oldTimeStep, mean, appMain.sof(1.0f));
		oldTimeStep = timeStep;
		
		
		if (Keys::Key('9'))
		{
			currentLevel->physics.world->stepSimulation((1.f / 60.f) * gameSpeed, 1, btScalar(1.f / 60.f));
		}
		else if (Keys::Key('8'))
		{
			currentLevel->physics.world->stepSimulation(appMain.sof(1.0f) * gameSpeed, 1, btScalar(1.f / 60.f));
		}
		else
		{
			currentLevel->physics.world->stepSimulation(timeStep * gameSpeed, 1, btScalar(1.f / 60.f));
		}
		//float fps = (appMain.sof(1.0) + 0.017f + 0.017f) / 3.f;
		//currentLevel->physics.world->stepSimulation(appMain.sof(1.0), 3.f, btScalar(1.f / 60.f));
	}

	if (!pause)
	{
		FMod.system->update();
	}
}