#include "stdafx.h"

void BlowUp::Init()
{
	/*FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\ambientLevelLoop.mp3", FMOD_2D, 0, &backgroundAudio));
	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, backgroundAudio, false, &backgroundChannel));*/

	physics.InitWorld();

	//Create Debug Shader Program 
	cShaderProgram *shader = new cShaderProgram();

	shader->vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	shader->frag.LoadShader(".\\Assets\\Shaders\\Debug.frag", GL_FRAGMENT_SHADER);
	shader->CreateProgram();
	shader->AddShaderToProgram(&shader->vert);
	shader->AddShaderToProgram(&shader->frag);
	shader->LinkProgram();

	physics.gDebugDrawer.sp = shader;

	//Create Default Shader Program 
	basicShaderProgram.vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	basicShaderProgram.frag.LoadShader(".\\Assets\\Shaders\\DirectionalBasic.frag", GL_FRAGMENT_SHADER);
	basicShaderProgram.CreateProgram();
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.vert);
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.frag);
	basicShaderProgram.LinkProgram();

	//Create Particle Shader
	billboard.vert.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.vert", GL_VERTEX_SHADER);
	billboard.geom.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.geom", GL_GEOMETRY_SHADER);
	billboard.frag.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.frag", GL_FRAGMENT_SHADER);
	billboard.CreateProgram();
	billboard.AddShaderToProgram(&billboard.vert);
	billboard.AddShaderToProgram(&billboard.geom);
	billboard.AddShaderToProgram(&billboard.frag);
	billboard.LinkProgram();

	//Create PassThrough Shader Program 
	passThrough.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	passThrough.frag.LoadShader(".\\Assets\\Shaders\\PassThrough.frag", GL_FRAGMENT_SHADER);
	passThrough.CreateProgram();
	passThrough.AddShaderToProgram(&passThrough.vert);
	passThrough.AddShaderToProgram(&passThrough.frag);
	passThrough.LinkProgram();

	//Create Bloom Shaders
	bloomHighPass.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	bloomHighPass.frag.LoadShader(".\\Assets\\Shaders\\Bloom\\BloomHighPass.frag", GL_FRAGMENT_SHADER);
	bloomHighPass.CreateProgram();
	bloomHighPass.AddShaderToProgram(&bloomHighPass.vert);
	bloomHighPass.AddShaderToProgram(&bloomHighPass.frag);
	bloomHighPass.LinkProgram();

	blurHorizontal.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	blurHorizontal.frag.LoadShader(".\\Assets\\Shaders\\Bloom\\BlurHorizontal.frag", GL_FRAGMENT_SHADER);
	blurHorizontal.CreateProgram();
	blurHorizontal.AddShaderToProgram(&blurHorizontal.vert);
	blurHorizontal.AddShaderToProgram(&blurHorizontal.frag);
	blurHorizontal.LinkProgram();

	blurVertical.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	blurVertical.frag.LoadShader(".\\Assets\\Shaders\\Bloom\\BlurVertical.frag", GL_FRAGMENT_SHADER);
	blurVertical.CreateProgram();
	blurVertical.AddShaderToProgram(&blurVertical.vert);
	blurVertical.AddShaderToProgram(&blurVertical.frag);
	blurVertical.LinkProgram();

	bloomComposite.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	bloomComposite.frag.LoadShader(".\\Assets\\Shaders\\Bloom\\BloomComposite.frag", GL_FRAGMENT_SHADER);
	bloomComposite.CreateProgram();
	bloomComposite.AddShaderToProgram(&bloomComposite.vert);
	bloomComposite.AddShaderToProgram(&bloomComposite.frag);
	bloomComposite.LinkProgram();

	sky.SetShaderProgram(&basicShaderProgram);

	for (int i = 0; i < 10; i++)
	{
		planet[i].SetShaderProgram(&basicShaderProgram);
	}

	//MultiSample FBO
	msaaBuffer.InitBuffer(1);	msaaBuffer.InitDepthTexture(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, true);
	msaaBuffer.InitColorTexture(0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, true);
	if (!msaaBuffer.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//Init Buffers
	mainBuffer.InitBuffer(1);	mainBuffer.InitDepthTexture(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);
	mainBuffer.InitColorTexture(0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!mainBuffer.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//Init Work Buffers
	workBuffer1.InitBuffer(1);
	workBuffer1.InitColorTexture(0, (GLuint)((MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE),
		(GLuint)((MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE), GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!workBuffer1.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	workBuffer2.InitBuffer(1);
	workBuffer2.InitColorTexture(0, (GLuint)((MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE),
		(GLuint)((MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE), GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
	if (!workBuffer2.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//Load Models
	sky.skyMesh.LoadFromFile(".\\Assets\\Models\\skysphere.mesh");
	sky.skyTexture.LoadTexture(".\\Assets\\Textures\\SpaceSkySphere.jpg", true);
	sky.skyTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);

	planet[0].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth1.mesh");
	planet[1].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth2.mesh");
	planet[2].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth3.mesh");
	planet[3].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth4.mesh");
	planet[4].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth5.mesh");
	planet[5].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth6.mesh");
	planet[6].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth7.mesh");
	planet[7].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth8.mesh");
	planet[8].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth9.mesh");
	planet[9].model.LoadFromFile(".\\Assets\\Models\\planet\\Earth10.mesh");

	
	planet[0].texture.LoadTexture(".\\Assets\\Textures\\earth.jpg", true);
	planet[0].texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);

	for (int i = 1; i < 10; i++)
	{
		planet[i].texture = planet[0].texture;
	}

	SceneCamera.SetPos(glm::vec3(0.0, 30.0, -40.0));
	SceneCamera.mView = glm::lookAt(SceneCamera.GetPos(), glm::vec3(0.0, 0.0, 0.0), glm::normalize(glm::vec3(1.0, 2.0, 0.0)));

	centers[0] = glm::vec3(-1.133, 5.180, -0.510);
	centers[1] = glm::vec3(1.008, 2.610, 0.296);
	centers[2] = glm::vec3(-0.732, 1.041, -2.217);
	centers[3] = glm::vec3(0.937, 1.875, 1.170);
	centers[4] = glm::vec3(2.330, 0.154, 1.454);
	centers[5] = glm::vec3(-2.478, 2.99, -1.262);
	centers[6] = glm::vec3(-3.115, 0.315, -0.459);
	centers[7] = glm::vec3(-1.760, 0.378, 4.003);
	centers[8] = glm::vec3(-0.786, -3.326, -1.092);
	centers[9] = glm::vec3(0.052, -2.038, 2.420);

	if (!explosion.Init(".\\Assets\\Textures\\fireParticle.png", 500, 300))
	{
		MessageBox(appMain.hWnd, L"Particle Failed to Initialize", L"PARTICLE ERROR", MB_ICONERROR);
	}

	explosion.LerpAlpha = glm::vec2(1.0f, 0.0f);
	explosion.LerpSize = glm::vec2(5.f, 0.1f);
	explosion.RangeLifetime = glm::vec2(8.0f, 10.0f);

	explosion.RangeVelocityX = glm::vec2(-3.f, 3.f);
	explosion.RangeVelocityY = glm::vec2(0.f, 0.f);
	explosion.RangeVelocityZ = glm::vec2(-3.f, 3.f);

	explosion.RangeY = glm::vec2(0.0, 0.0);
	explosion.RangeX = glm::vec2(0.0, 0.0);
	explosion.RangeZ = glm::vec2(0.0, 0.0);

	endMessage.LoadTexture(".\\Assets\\Textures\\msg_loseScreen.png", false);
	endMessage.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	mouse.LoadTexture(".\\Assets\\Textures\\mouseArrow.png", false);
	mouse.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR);

	timer = 4.f;
}

void BlowUp::HandleEvents()
{
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

	if (Keys::Onekey(27)) { gameState = 1; }

	if (Keys::Onekey(VK_OEM_3))
	{
		HWND hwnd = GetConsoleWindow();
		SetForegroundWindow(hwnd);
		char paramater[80];

		do
		{
			system("cls");
			int iValue;
			float fValue;

			printf("----- CHANGE ENGINE PARAMETERS -----\n\n");
			printf("PARAMATERS:        VALUES:\n\n");
			printf("WIREFRAME          %d\n", WIREFRAME);
			printf("PHYSICS_DEBUG      %d\n", PHYSICS_DEBUG);
			printf("BLOOM_ENABLED      %d\n", BLOOM_ENABLED);
			printf("BLOOM_THRESHOLD    %f\n", BLOOM_THRESHOLD);
			printf("BLOOM_BLUR_PASSES  %d\n", BLOOM_BLUR_PASSES);
			printf("\n------------------------------------\n\n");

			printf("ENTER PARAMATER: ");
			scanf("%79s", paramater);

			printf("\nENTER VALUE:     ");
			if (strcmp(paramater, "WIREFRAME") == 0 || strcmp(paramater, "BLOOM_ENABLED") == 0 || strcmp(paramater, "BLOOM_BLUR_PASSES") == 0
				|| strcmp(paramater, "PHYSICS_DEBUG") == 0)
			{
				scanf("%d", &iValue);
				if (strcmp(paramater, "WIREFRAME") == 0 ? WIREFRAME = !!(iValue) :
					strcmp(paramater, "BLOOM_ENABLED") == 0 ? BLOOM_ENABLED = !!(iValue) :
					strcmp(paramater, "PHYSICS_DEBUG") == 0 ? PHYSICS_DEBUG = !!(iValue) :
					strcmp(paramater, "BLOOM_BLUR_PASSES") == 0 ? BLOOM_BLUR_PASSES = iValue : 0);

			}
			else if (strcmp(paramater, "BLOOM_THRESHOLD") == 0)
			{
				scanf("%f", &fValue);
				if (strcmp(paramater, "BLOOM_THRESHOLD") == 0 ? BLOOM_THRESHOLD = fValue : 0);
			}
			else if (strcmp(paramater, "CLOWN_CAR") == 0)
			{
				scanf("%d", &iValue);
				if (iValue == 0)
				{
					currentLevel->player.robotTexture.LoadTexture(".\\Assets\\Textures\\robotTexture.jpg", true);
				}
				else
				{
					currentLevel->player.robotTexture.LoadTexture(".\\Assets\\Textures\\robotTexture_clown.jpg", true);
				}
			}
			else
			{
				printf("\nERROR: PARAMATER NOT REGISTERED!");
			}

			printf("\n\nCHANGE NEW PARAMATER? (Y / N)  ");
			scanf("%79s", paramater);
		} while (strcmp(paramater, "Y") == 0);
		system("cls");

		SetForegroundWindow(appMain.hWnd);
	}

	if (timer < 0.0f)
	{
		if (Keys::Onekey(VK_LBUTTON))
		{
			if (Keys::MousePosition(false).x > X_PERCENT(22.0625f) && Keys::MousePosition(false).x < X_PERCENT(44.8125f) &&
				Keys::MousePosition(false).y > Y_PERCENT(41.777f) && Keys::MousePosition(false).y < Y_PERCENT(53.666f))
			{
				gameState = 1;
			}
			if (Keys::MousePosition(false).x > X_PERCENT(55.f) && Keys::MousePosition(false).x < X_PERCENT(78.f) &&
				Keys::MousePosition(false).y > Y_PERCENT(41.777f) && Keys::MousePosition(false).y < Y_PERCENT(53.666f))
			{
				gameState = 3;
			}
		}
	}
}

void BlowUp::Update()
{
	//planet[0].matrix;
	explosion.Update(appMain.sof(1.0));
	explosion._Rate = 0.0f;

	for (int i = 0; i < 10; i++)
	{
		planet[i].matrix = glm::translate(planet[i].matrix, centers[i] * 0.01f);
	}

	timer -= appMain.sof(0.4f);
}

void BlowUp::Draw()
{
	msaaBuffer.Clear();
	mainBuffer.Clear();
	workBuffer1.Clear();
	workBuffer2.Clear();

	glEnable(GL_DEPTH_TEST);
	//Render Scene

	glViewport(0, 0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);

	basicShaderProgram.UseProgram();

	basicShaderProgram.setUniform("viewMatrix", SceneCamera.mView);
	basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
	basicShaderProgram.setUniform("gSampler", 0);

	msaaBuffer.Bind();
	{
		glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME ? GL_LINE : GL_FILL);
		basicShaderProgram.setUniform("ambient", 1.0f);
		sky.RenderSkybox();

		basicShaderProgram.setUniform("ambient", 0.05f);
		for (int i = 0; i < 10; i++)
		{
			planet[i].Draw();
		}

		//Particles in here!
		billboard.UseProgram();
		billboard.setUniform("uTex", 0);
		billboard.setUniform("uModel", explosion.Transform);
		billboard.setUniform("uView", SceneCamera.mView);
		billboard.setUniform("uProj", SceneCamera.mProjection);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Normal Blend
		explosion.Render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//just incase
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);

	msaaBuffer.UnBind();

	msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer);
	msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer, true);

	mainBuffer.Bind();

	ShowCursor(FALSE);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	currentLevel->passThrough.UseProgram();
	currentLevel->passThrough.setUniform("uTex", 0);
	currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

	if (timer < 0.f)
	{
		glViewport(0, 0, MAX_WIDTH, MAX_HEIGHT);

		currentLevel->passThrough.setUniform("modelMatrix", glm::mat4(1.0));
		endMessage.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		Keys::locked = false;
		glm::mat4x4 modelMatrix = ScreenRectangle((int)Keys::MousePosition(false).x, (int)Keys::MousePosition(false).y, X_PERCENT(5.25f), Y_PERCENT(8.666f));
		currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
		mouse.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	mainBuffer.UnBind();

	//if (BLOOM_ENABLED)
	//{
	//	//High Pass
	//	glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
	//	bloomHighPass.UseProgram();
	//	bloomHighPass.setUniform("uTex", 0);
	//	bloomHighPass.setUniform("uThreshold", BLOOM_THRESHOLD);

	//	workBuffer1.Bind();

	//	glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
	//	DrawFullScreenQuad();
	//	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	//	workBuffer1.UnBind();

	//	//Compute Blur
	//	glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
	//	for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
	//	{
	//		//Horizontal
	//		blurHorizontal.UseProgram();
	//		blurHorizontal.setUniform("uTex", 0);
	//		blurHorizontal.setUniform("uPixelSize", 1.0f / ((MAX_WIDTH) / BLOOM_DOWNSCALE));

	//		workBuffer2.Bind();

	//		glBindTexture(GL_TEXTURE_2D, workBuffer1.GetColorHandle(0));
	//		DrawFullScreenQuad();
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);

	//		workBuffer2.UnBind();

	//		//Vertical
	//		blurVertical.UseProgram();
	//		blurVertical.setUniform("uTex", 0);
	//		blurVertical.setUniform("uPixelSize", 1.0f / ((MAX_HEIGHT) / BLOOM_DOWNSCALE));

	//		workBuffer1.Bind();

	//		glBindTexture(GL_TEXTURE_2D, workBuffer2.GetColorHandle(0));
	//		DrawFullScreenQuad();
	//		glBindTexture(GL_TEXTURE_2D, GL_NONE);

	//		workBuffer1.UnBind();
	//	}

	//	//Composite to back buffer
	//	int x = 0 - (EDGE_OFFSET * (MAX_WIDTH / WIDTH) / 2);
	//	int y = 0 - (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT) / 2);
	//	int w = WIDTH + (EDGE_OFFSET * (MAX_WIDTH / WIDTH));
	//	int h = HEIGHT + (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT));
	//	glViewport(x, y, w, h);

	//	bloomComposite.UseProgram();
	//	bloomComposite.setUniform("uScene", 0);
	//	bloomComposite.setUniform("uBloom", 1);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, workBuffer1.GetColorHandle(0));
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
	//	DrawFullScreenQuad();
	//	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	//}
	//else
	//{
		mainBuffer.MoveToBackBuffer(MAX_WIDTH, MAX_HEIGHT);
	//}
}

void BlowUp::Release()
{ 
	//backgroundAudio->release();
}