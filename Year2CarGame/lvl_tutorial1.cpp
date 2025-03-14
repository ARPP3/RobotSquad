#include "stdafx.h"

void Tutorial1::Init()
{
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\ambientLevelLoop.mp3", FMOD_2D, 0, &backgroundAudio));
	backgroundAudio->setMode(FMOD_LOOP_NORMAL);
	//channel->setLoopCount(-1);
	FMod.CheckError(FMod.system->createChannelGroup(NULL, &Channels));

	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\01 Welcome.mp3",								FMOD_2D, 0, &voice[0]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\02 Your Goal.mp3",								FMOD_2D, 0, &voice[1]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\03 Use the right trigger.mp3",					FMOD_2D, 0, &voice[2]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\04 Nice.mp3",									FMOD_2D, 0, &voice[3]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\05 Navigate Forward.mp3",						FMOD_2D, 0, &voice[4]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\06 You have found a bomb.mp3",					FMOD_2D, 0, &voice[5]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\07 Bring It Back.mp3",							FMOD_2D, 0, &voice[6]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\08 Great Job, Notice.mp3",						FMOD_2D, 0, &voice[7]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\09 The Timer in the top right.mp3",				FMOD_2D, 0, &voice[8]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\10 Since this is a training simulation.mp3",	FMOD_2D, 0, &voice[9]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\11 Find the other bomb.mp3",					FMOD_2D, 0, &voice[10]));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\TrainingVoice\\12 Congratulations.mp3",						FMOD_2D, 0, &voice[11]));

	voiceChannel->setChannelGroup(currentLevel->Channels);

	physics.InitWorld();

	physics.LoadFromFile(".\\Assets\\PhysicsLevels\\tutorial1.PHYSICS");

	//Create Debug Shader Program 
	cShaderProgram *shader = new cShaderProgram();

	shader->vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	shader->frag.LoadShader(".\\Assets\\Shaders\\Debug.frag", GL_FRAGMENT_SHADER);
	shader->CreateProgram();
	shader->AddShaderToProgram(&shader->vert);
	shader->AddShaderToProgram(&shader->frag);
	shader->LinkProgram();

	physics.gDebugDrawer.sp = shader;

	timer = 0.f;

	//Create Default Shader Program 
	basicShaderProgram.vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	basicShaderProgram.frag.LoadShader(".\\Assets\\Shaders\\DirectionalBasic.frag", GL_FRAGMENT_SHADER);
	basicShaderProgram.CreateProgram();	
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.vert);
	basicShaderProgram.AddShaderToProgram(&basicShaderProgram.frag);
	basicShaderProgram.LinkProgram();

	//Create Toon Shader Program 
	toonProgram.vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	toonProgram.frag.LoadShader(".\\Assets\\Shaders\\Toon\\ToonBasic.frag", GL_FRAGMENT_SHADER);
	toonProgram.CreateProgram();
	toonProgram.AddShaderToProgram(&toonProgram.vert);
	toonProgram.AddShaderToProgram(&toonProgram.frag);
	toonProgram.setUniform("toonRamp", 1);
	toonProgram.LinkProgram();

	//Create PassThrough Shader Program 
	passThrough.vert.LoadShader(".\\Assets\\Shaders\\PassThrough.vert", GL_VERTEX_SHADER);
	passThrough.frag.LoadShader(".\\Assets\\Shaders\\PassThrough.frag", GL_FRAGMENT_SHADER);
	passThrough.CreateProgram();
	passThrough.AddShaderToProgram(&passThrough.vert);
	passThrough.AddShaderToProgram(&passThrough.frag);
	passThrough.LinkProgram();

	//Create Particle Shader
	billboard.vert.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.vert", GL_VERTEX_SHADER);
	billboard.geom.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.geom", GL_GEOMETRY_SHADER);
	billboard.frag.LoadShader(".\\Assets\\Shaders\\Particles\\Billboard.frag", GL_FRAGMENT_SHADER);
	billboard.CreateProgram();
	billboard.AddShaderToProgram(&billboard.vert);
	billboard.AddShaderToProgram(&billboard.geom);
	billboard.AddShaderToProgram(&billboard.frag);
	billboard.LinkProgram();

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

	player.SetShaderProgram(&basicShaderProgram);
	sky.SetShaderProgram(&basicShaderProgram);
	tnt[0].SetShaderProgram(&basicShaderProgram);

	//MultiSample FBO
	msaaBuffer.InitBuffer(1);	msaaBuffer.InitDepthTexture(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, true);
	msaaBuffer.InitColorTexture(0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE, true);
	if (!msaaBuffer.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//Init Buffers
	mainBuffer.InitBuffer(1);	mainBuffer.InitDepthTexture(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);
	mainBuffer.InitColorTexture(0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
	if (!mainBuffer.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//ShadowMap
	shadowMap.InitBuffer(0);	shadowMap.InitDepthTexture(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	if (!shadowMap.CheckFBO()) MessageBox(appMain.hWnd, L"FBO Failed to Initialize", L"FBO ERROR", MB_ICONERROR);

	//shadowProjection = glm::ortho(-75.f, 170.f, -45.f, 100.f, -30.f, 250.f);
	shadowProjection = glm::ortho(-110.f, 110.f, -110.f, 110.f, -30.f, 200.f);

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
	//walls = cObject("Tutorial_1_Walls.mesh", "Yellow.jpg", &basicShaderProgram);
	walls.model.LoadFromFile(".\\Assets\\Models\\Tutorial_1_Walls.mesh");
	walls.texture.LoadTexture(".\\Assets\\Textures\\Yellow.jpg", true);
	walls.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	walls.SetShaderProgram(&basicShaderProgram);
	
	tnt[0].model.LoadFromFile(".\\Assets\\Models\\tnt.mesh");
	tnt[0].texture.LoadTexture(".\\Assets\\Textures\\TNT.jpg", true);
	tnt[0].texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	for (int i = 0; i < numBombs - 1; i++)
	{
		tnt[i + 1].model = tnt[0].model;
		tnt[i + 1].texture = tnt[0].texture;
		tnt[i + 1].shaderProgram = tnt[0].shaderProgram;
	}

	tnt[0].Init(glm::vec3(30.0, 2.0, 30.0));
	tnt[1].Init(glm::vec3(-33.0, 2.0, 18.0));

	//Load Toon Ramp
	toonRamp.LoadTexture(".\\Assets\\Textures\\toonRamp.jpg", false);
	toonRamp.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	//float *verticies = new float[map.model._NumVerticies];
	//glBindBuffer(GL_ARRAY_BUFFER, map.model.VBO_Verticies);
	//glGetBufferSubData(GL_ARRAY_BUFFER, 0, map.model._NumVerticies * sizeof(float), verticies);
	//glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	//
	//for (unsigned int i = 0; i < map.model._NumVerticies; i += 9)
	//{
	//	_map.addTriangle(btVector3(verticies[i], verticies[i + 1], verticies[i + 2]), btVector3(verticies[i + 3], verticies[i + 4], verticies[i + 5]),
	//		btVector3(verticies[i + 6], verticies[i + 7], verticies[i + 8]), false);
	//}
	//_shape = new btBvhTriangleMeshShape(&_map, true);
	//btTransform t;
	//t.setIdentity();
	//t.setOrigin(btVector3(0.f, 0.f, 0.f));
	//_body = new btRigidBody(0, 0, _shape);
	//_body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	//_body->setWorldTransform(t);
	//_body->setCcdSweptSphereRadius(0);
	//_body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//physics->world->addRigidBody(_body);
	//physics->bodies.push_back(_body);

	if (!ringEffect.Init(".\\Assets\\Textures\\cubeParticle.png", 500, 60))
	{
		MessageBox(appMain.hWnd, L"Particle Failed to Initialize", L"PARTICLE ERROR", MB_ICONERROR);
	}

	ringEffect.LerpAlpha = glm::vec2(1.0f, 0.0f);
	ringEffect.LerpSize = glm::vec2(0.3f, 0.1f);
	ringEffect.RangeLifetime = glm::vec2(8.0f, 10.0f);

	ringEffect.RangeVelocityX = glm::vec2(0.0, 0.0);
	ringEffect.RangeVelocityY = glm::vec2(0.5, 0.55);
	ringEffect.RangeVelocityZ = glm::vec2(0.0, 0.0);

	ringEffect.AbsoluteVelocity = true;

	ringEffect.RangeY = glm::vec2(0.0, 0.0);
	ringEffect.RangeX = glm::vec2(sin((float)(GetTickCount() % 360)) * 9.7, sin((float)(GetTickCount() % 360)) * 9.7);
	ringEffect.RangeZ = glm::vec2(cos((float)(GetTickCount() % 360)) * 9.7, cos((float)(GetTickCount() % 360)) * 9.7);

	if (!sparks.Init(".\\Assets\\Textures\\sparkParticle.png", 30, 1))
	{
		MessageBox(appMain.hWnd, L"Particle Failed to Initialize", L"PARTICLE ERROR", MB_ICONERROR);
	}

	sparks.LerpAlpha = glm::vec2(1.0f, 0.8f);
	sparks.LerpSize = glm::vec2(0.4f, 0.15f);
	sparks.RangeLifetime = glm::vec2(0.3f, 0.5f);

	sparks.HasGravity = true;

	sparks.RangeVelocityX = glm::vec2(-3.0, 3.0);
	sparks.RangeVelocityY = glm::vec2(-3.0, 3.0);
	sparks.RangeVelocityZ = glm::vec2(-3.0, 3.0);

	sparks.RangeY = glm::vec2(0.0, 0.0);
	sparks.RangeX = glm::vec2(0.0, 0.0);
	sparks.RangeZ = glm::vec2(0.0, 0.0);

	player.Init();
	player.isTutorial = true;
	player.showTutorialMessage = false;
	player.numBombs = numBombs;

	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, voice[0], false, &voiceChannel));
}

void Tutorial1::HandleEvents()
{
	player.HandleEvents();

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
}

void Tutorial1::Update()
{
	for (int i = 0; i < numBombs; i++)	tnt[i].UpdateModelMatrix();
	player.Update();

	//Check Distances
	
	if (player.haveBomb == -1)
	{
		player.numBombsLeft = numBombs;
		bool win = true;

		btTransform t;
		glm::vec3 b = glm::vec3(0.0, 0.0, 0.0);

		for (int i = 0; i < numBombs; i++)
		{
			tnt[i]._motionState->getWorldTransform(t);
			glm::vec3 a = glm::vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());

			float distance = glm::sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
			if (distance > 10.f)
			{
				win = false;
				player.numBombsLeft++;
			}
			else
			{
				if (voicePlayed[7] == 0)
				{
					FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, voice[7], false, &voiceChannel));
					voicePlayed[7] = 1;
				}
			}
			player.numBombsLeft--;

		}
		if (win)
		{
			if (voicePlayed[11] == 0)
			{
				FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, voice[11], false, &voiceChannel));
				voicePlayed[11] = 1;
			}

			player.showTutorialMessage = true;
		}
	}

	ringEffect.Update(appMain.sof(1.0));
	ringEffect.RangeX = glm::vec2(sin((float)(GetTickCount() % 360)) * 9.7, sin((float)(GetTickCount() % 360)) * 9.7);
	ringEffect.RangeZ = glm::vec2(cos((float)(GetTickCount() % 360)) * 9.7, cos((float)(GetTickCount() % 360)) * 9.7);
	
	sparks.Update(appMain.sof(1.0));
}

void Tutorial1::Draw()
{
	msaaBuffer.Clear();
	mainBuffer.Clear();
	shadowMap.Clear();
	workBuffer1.Clear();
	workBuffer2.Clear();

	glEnable(GL_DEPTH_TEST);

	//Generate ShadowMap
	/*glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);

	basicShaderProgram.UseProgram();

	basicShaderProgram.setUniform("viewMatrix", glm::inverse(shadowTransform));
	basicShaderProgram.setUniform("projectionMatrix", shadowProjection);
	basicShaderProgram.setUniform("gSampler", 0);
	basicShaderProgram.setUniform("gShadowMap", 1);
	basicShaderProgram.setUniform("viewToShadowMap", viewToShadowMap);

	shadowMap.Bind();
	{
		player.Draw();
		for (unsigned int i = 0; i < backgroundItems.size(); i++)
		{
			backgroundItems[i].Draw();
		}
		FOR(i, numBombs)	tnt[i].Draw();
	}

	shadowMap.UnBind();*/

	//Render Scene

	glViewport(0, 0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);

	basicShaderProgram.UseProgram();

	basicShaderProgram.setUniform("viewMatrix", SceneCamera.mView);
	basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
	basicShaderProgram.setUniform("gSampler", 0);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, shadowMap.GetDepthHandle());

	msaaBuffer.Bind();
	{
		glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME ? GL_LINE : GL_FILL);
		basicShaderProgram.setUniform("ambient", 1.0f);
		walls.Draw();
		basicShaderProgram.setUniform("ambient", 0.1f);
		player.Draw();
		for (int i = 0; i < numBombs; i++)		tnt[i].Draw();
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

	glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME ? GL_LINE : GL_FILL);

	billboard.UseProgram();
	billboard.setUniform("uTex", 0);
	billboard.setUniform("uModel", ringEffect.Transform);
	billboard.setUniform("uView", SceneCamera.mView);
	billboard.setUniform("uProj", SceneCamera.mProjection);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Normal Blend
	ringEffect.Render();

	billboard.setUniform("uModel", player.smokeEffect.Transform);
	player.smokeEffect.Render();

	for (int i = 0; i < numBombs; i++)
	{
		btTransform t;	float m[16];
		tnt[i].body->getMotionState()->getWorldTransform(t);
		t.getOpenGLMatrix(m);

		glm::mat4x4 matrix
			(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
			);

		matrix = glm::translate(matrix, glm::vec3(0.0, 0.0, 2.35));

		glm::vec3 pos = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

		billboard.setUniform("uModel", glm::translate(glm::mat4x4(1.0), pos));
		sparks.Render();
	}

	player.bombMat = glm::translate(player.bombMat, glm::vec3(player.bombMat[1][0], player.bombMat[1][1], player.bombMat[1][2]) * 2.35f);

	currentLevel->billboard.setUniform("uModel", player.matrix * player.bombMat);
	currentLevel->sparks.Render();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mainBuffer.UnBind();

	if (BLOOM_ENABLED)
	{
		//High Pass
		glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
		bloomHighPass.UseProgram();
		bloomHighPass.setUniform("uTex", 0);
		bloomHighPass.setUniform("uThreshold", BLOOM_THRESHOLD);

		workBuffer1.Bind();

		glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		workBuffer1.UnBind();

		//Compute Blur
		glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
		for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
		{
			//Horizontal
			blurHorizontal.UseProgram();
			blurHorizontal.setUniform("uTex", 0);
			blurHorizontal.setUniform("uPixelSize", 1.0f / ((MAX_WIDTH) / BLOOM_DOWNSCALE));

			workBuffer2.Bind();

			glBindTexture(GL_TEXTURE_2D, workBuffer1.GetColorHandle(0));
			DrawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			workBuffer2.UnBind();

			//Vertical
			blurVertical.UseProgram();
			blurVertical.setUniform("uTex", 0);
			blurVertical.setUniform("uPixelSize", 1.0f / ((MAX_HEIGHT) / BLOOM_DOWNSCALE));

			workBuffer1.Bind();

			glBindTexture(GL_TEXTURE_2D, workBuffer2.GetColorHandle(0));
			DrawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			workBuffer1.UnBind();
		}

		//Composite to back buffer
		int x = 0 - (EDGE_OFFSET * (MAX_WIDTH / WIDTH) / 2);
		int y = 0 - (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT) / 2);
		int w = WIDTH + (EDGE_OFFSET * (MAX_WIDTH / WIDTH));
		int h = HEIGHT + (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT));
		glViewport(x, y, w, h);

		bloomComposite.UseProgram();
		bloomComposite.setUniform("uScene", 0);
		bloomComposite.setUniform("uBloom", 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, workBuffer1.GetColorHandle(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
	else
	{
		mainBuffer.MoveToBackBuffer(MAX_WIDTH, MAX_HEIGHT);
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, WIREFRAME ? GL_LINE : GL_FILL);
	player.DrawHUD();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Tutorial1::Release()
{
	backgroundAudio->release();
	player.engineLoop->release();
}