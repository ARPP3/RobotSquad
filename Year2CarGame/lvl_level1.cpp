#include "stdafx.h"

ParticleEffect explosion;

void Level1::Init()
{
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\ambientLevelLoop.wav", FMOD_3D, 0, &backgroundAudio));
	backgroundAudio->setMode(FMOD_LOOP_NORMAL);
	//channel->setLoopCount(-1);
	FMod.CheckError(FMod.system->createChannelGroup(NULL, &Channels));

	physics.InitWorld();
	physics.LoadFromFile(".\\Assets\\PhysicsLevels\\Warehouse.PHYSICS");

	//Create Debug Shader Program 
	cShaderProgram *shader = new cShaderProgram();

	shader->vert.LoadShader(".\\Assets\\Shaders\\StaticGeometry.vert", GL_VERTEX_SHADER);
	shader->frag.LoadShader(".\\Assets\\Shaders\\Debug.frag", GL_FRAGMENT_SHADER);
	shader->CreateProgram();
	shader->AddShaderToProgram(&shader->vert);
	shader->AddShaderToProgram(&shader->frag);
	shader->LinkProgram();

	physics.gDebugDrawer.sp = shader;

	timer = 120.f;

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
	brick.SetShaderProgram(&basicShaderProgram);
	door.SetShaderProgram(&basicShaderProgram);
	walls.SetShaderProgram(&basicShaderProgram);
	floor.SetShaderProgram(&basicShaderProgram);
	moss.SetShaderProgram(&basicShaderProgram);
	vent.SetShaderProgram(&basicShaderProgram);
	puddle.SetShaderProgram(&basicShaderProgram);
	oil.SetShaderProgram(&basicShaderProgram);
	fire.SetShaderProgram(&basicShaderProgram);
	forklift.SetShaderProgram(&basicShaderProgram);
	orangefence.SetShaderProgram(&basicShaderProgram);
	helicopter.SetShaderProgram(&basicShaderProgram);
	ring.SetShaderProgram(&basicShaderProgram);
	boxes.SetShaderProgram(&basicShaderProgram);
	tree[0].SetShaderProgram(&basicShaderProgram);
	tree[1].SetShaderProgram(&basicShaderProgram);

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
	sky.skyMesh.LoadFromFile(".\\Assets\\Models\\skysphere.mesh");
	sky.skyTexture.LoadTexture(".\\Assets\\Textures\\skysphere.jpg", true);
	sky.skyTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);

	brick.model.LoadFromFile(".\\Assets\\Models\\brick.mesh");
	brick.texture.LoadTexture(".\\Assets\\Textures\\brick_metal.jpg", true);
	brick.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	door.model.LoadFromFile(".\\Assets\\Models\\doors.mesh");
	door.texture.LoadTexture(".\\Assets\\Textures\\door.jpg", true);
	door.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	walls.model.LoadFromFile(".\\Assets\\Models\\walls.mesh");
	walls.texture.LoadTexture(".\\Assets\\Textures\\wall.jpg", true);
	walls.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	floor.model.LoadFromFile(".\\Assets\\Models\\concrete.mesh");
	floor.texture.LoadTexture(".\\Assets\\Textures\\concrete.jpg", true);
	floor.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	moss.model.LoadFromFile(".\\Assets\\Models\\grime.mesh");
	moss.texture.LoadTexture(".\\Assets\\Textures\\moss.png", true);
	moss.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	vent.model.LoadFromFile(".\\Assets\\Models\\crates.mesh");
	vent.texture.LoadTexture(".\\Assets\\Textures\\vent_window_crate.png", true);
	vent.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	puddle.model.LoadFromFile(".\\Assets\\Models\\puddles.mesh");
	puddle.texture.LoadTexture(".\\Assets\\Textures\\puddle.png", true);
	puddle.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	fire.model.LoadFromFile(".\\Assets\\Models\\fire.mesh");
	fire.texture.LoadTexture(".\\Assets\\Textures\\fire.jpg", true);
	fire.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	oil.model.LoadFromFile(".\\Assets\\Models\\oil.mesh");
	oil.texture.LoadTexture(".\\Assets\\Textures\\oil.jpg", true);
	oil.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	orangefence.model.LoadFromFile(".\\Assets\\Models\\orangefence.mesh");
	orangefence.texture.LoadTexture(".\\Assets\\Textures\\fence.png", true);
	orangefence.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	forklift.model.LoadFromFile(".\\Assets\\Models\\forklift.mesh");
	forklift.texture.LoadTexture(".\\Assets\\Textures\\forklift.jpg", true);
	forklift.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	tree[0].model.LoadFromFile(".\\Assets\\Models\\tree.mesh");
	tree[0].texture.LoadTexture(".\\Assets\\Textures\\tree.png", true);
	tree[0].texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);
	tree[1].model.LoadFromFile(".\\Assets\\Models\\treeLeaves.mesh");
	tree[1].texture = tree[0].texture;

	//End Ring
	ring.model.LoadFromFile(".\\Assets\\Models\\endRing.mesh");
	ring.texture.LoadTexture(".\\Assets\\Textures\\Yellow.jpg", true);
	ring.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_TRILINEAR);

	//Bombs and Enemies
	helicopter.bodyModel.LoadFromFile(".\\Assets\\Models\\bodyHelicopter.mesh");
	helicopter.bladeModel.LoadFromFile(".\\Assets\\Models\\bladeHelicopter.mesh");
	helicopter.magnetModel.LoadFromFile(".\\Assets\\Models\\MagnetHelicopter.mesh");
	helicopter.texture.LoadTexture(".\\Assets\\Textures\\Heli.jpg", true);
	helicopter.texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);


	tnt[0].model.LoadFromFile(".\\Assets\\Models\\tnt.mesh");
	tnt[0].texture.LoadTexture(".\\Assets\\Textures\\TNT.jpg", true);
	tnt[0].texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	//FadeScreen Tex
	black.LoadTexture(".\\Assets\\Textures\\black.jpg", false);
	black.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);


	for (int i = 0; i < numBombs - 1; i++)
	{
		tnt[i + 1].model = tnt[0].model;
		tnt[i + 1].texture = tnt[0].texture;
		tnt[i + 1].shaderProgram = tnt[0].shaderProgram;
	}
	glm::vec3 randomSpawn[21];

	randomSpawn[0] = glm::vec3(173.919, 2.0, 43.407);
	randomSpawn[1] = glm::vec3(158.979, 9.0, 183.661);
	randomSpawn[2] = glm::vec3(164.786, 7.0, 201.017);
	randomSpawn[3] = glm::vec3(168.787, 1.0, 198.909);
	randomSpawn[4] = glm::vec3(130.418, 1.0, 243.396);
	randomSpawn[5] = glm::vec3(121.941, 18.0, 223.901);
	randomSpawn[6] = glm::vec3(91.654, 9.0, 155.042);
	randomSpawn[7] = glm::vec3(43.308, 7.0, 114.119);
	randomSpawn[8] = glm::vec3(91.518, 1.0, 118.268);
	randomSpawn[9] = glm::vec3(150.77, 2.0, 78.285);
	randomSpawn[10] = glm::vec3(-3.855, 2.0, 106.278);
	randomSpawn[11] = glm::vec3(72.393, 1.0, 273.979);
	randomSpawn[12] = glm::vec3(-42.602, 1.0, -10.573);
	randomSpawn[13] = glm::vec3(40.73, 1.0, 61.921);
	randomSpawn[14] = glm::vec3(92.464, 1.0, 91.097);
	randomSpawn[15] = glm::vec3(-32.1, 1.0, 103.662);
	randomSpawn[16] = glm::vec3(-20.25, 2.0, -3.77);
	randomSpawn[17] = glm::vec3(-7.711, 1.0, -10.236);
	randomSpawn[18] = glm::vec3(121.776, 2.0, 154.169);
	randomSpawn[19] = glm::vec3(118.121, 2.0, 183.691);
	randomSpawn[20] = glm::vec3(92.606, 9.0, 247.146);

	SeedRandomNumberGenerator();
	int random = RandomRangei(0, 20);

	for (int i = 0; i < numBombs; i++)
	{
		int num = (random + (i * RandomRangei(1, 7))) % 21;
		printf("\nBombToSpawn: %i", num);
		tnt[i].Init(randomSpawn[num]);
	}

	//Load Toon Ramp
	toonRamp.LoadTexture(".\\Assets\\Textures\\toonRamp.jpg", false);
	toonRamp.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	player.Init();
	boxes.Init(".\\Assets\\PhysicsLevels\\Warehouse.DYNAMIC");

	ring.SetPos(glm::vec3(-100.0, 0.0, 48.0));
	ring.matrix = glm::translate(glm::mat4x4(1.0), ring.GetPos());

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

	ringEffect.RangeY = glm::vec2(0.0, 0.0);
	ringEffect.RangeX = glm::vec2((sin((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().x, (sin((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().x);
	ringEffect.RangeZ = glm::vec2((cos((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().z, (cos((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().z);
	
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

	if (!explosion.Init(".\\Assets\\Textures\\sparkParticle.png", 3000, 30))
	{
		MessageBox(appMain.hWnd, L"Particle Failed to Initialize", L"PARTICLE ERROR", MB_ICONERROR);
	}

	explosion.LerpAlpha = glm::vec2(1.0f, 0.8f);
	explosion.LerpSize = glm::vec2(0.4f, 0.15f);
	explosion.RangeLifetime = glm::vec2(100.f, 100.f);

	explosion.HasGravity = true;
	explosion.AbsoluteVelocity = false;

	explosion.RangeVelocityX = glm::vec2(-0.1, 0.1);
	explosion.RangeVelocityY = glm::vec2(0.0, 0.1);
	explosion.RangeVelocityZ = glm::vec2(-0.1, 0.1);

	explosion.RangeX = glm::vec2(0.0, 0.0);
	explosion.RangeY = glm::vec2(1.0, 1.0);
	explosion.RangeZ = glm::vec2(0.0, 0.0);

	helicopter.Init();
	player.numBombs = numBombs;

	//Start the music at the level start!
	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, backgroundAudio, false, &backgroundChannel));
	backgroundChannel->setChannelGroup(Channels);
}

void Level1::HandleEvents()
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

void Level1::Update()
{
	for (int i = 0; i < numBombs; i++)	tnt[i].UpdateModelMatrix();
	player.Update();
	helicopter.Update();

	shadowTransform = glm::mat4x4(1.0);
	shadowTransform = glm::rotate(shadowTransform, 82.f, glm::vec3(0.0, 1.0, 0.0));
	shadowTransform = glm::translate(shadowTransform, glm::vec3(35.0, 25.0, 5.0));
	shadowTransform = glm::rotate(shadowTransform, -30.f, glm::vec3(1.0, 0.0, 0.0));

	glm::mat4x4 bias = glm::mat4x4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);

	viewToShadowMap = glm::mat4x4(1.0);
	viewToShadowMap = bias * shadowProjection * glm::inverse(shadowTransform) * glm::inverse(SceneCamera.mView);

	//Check Distances
	
	if (player.haveBomb == -1)
	{
		player.numBombsLeft = numBombs;
		bool win = true;

		btTransform t;
		glm::vec3 b = ring.GetPos();

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
				if (!tnt[i].body->isStaticObject())
				{
					btTransform t = btTransform(btQuaternion(), btVector3(ring.GetPos().x, 1000, ring.GetPos().z));

					tnt[i].body->setWorldTransform(t);
					tnt[i].body->setCollisionFlags(tnt[i].body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
				}
			}
			player.numBombsLeft--;
		}
		/*if (win)
		{
			player.showTutorialMessage = true;
		}*/
		if (win)
		{
			player.showLevelMessage = true;
		}
	}

	ringEffect.Update(appMain.sof(1.0));
	ringEffect.RangeX = glm::vec2((sin((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().x, (sin((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().x);
	ringEffect.RangeZ = glm::vec2((cos((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().z, (cos((float)(GetTickCount() % 360)) * 9.7) + ring.GetPos().z);
	
	sparks.Update(appMain.sof(1.0));
	explosion.Update(appMain.sof(1.0));

	if (timer < 10.f)
	{
		//backgroundAudio->setMusicSpeed(2.0f);
	}

	if (timer < -1.5f)
	{
		gameState = 4;
	}

	FMOD_VECTOR POS = { SceneCamera.GetPos().x, SceneCamera.GetPos().y, SceneCamera.GetPos().z};
	FMOD_VECTOR VEL = { 0.0, 0.0, 0.0 };
	backgroundChannel->set3DAttributes(&POS, &VEL);
}

void Level1::Draw()
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

		brick.Draw();
		door.Draw();
		walls.Draw();
		floor.Draw();
		helicopter.Draw();
		vent.Draw();

		for (int i = 0; i < numBombs; i++)
		{
			tnt[i].Draw();
		}
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
			sky.RenderSkybox();

		basicShaderProgram.setUniform("ambient", 0.2f);
			brick.Draw();
			door.Draw();
			walls.Draw();
			floor.Draw();

		glEnable(GL_BLEND);
		basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mDecalProjection);
			moss.Draw();
			puddle.Draw();
			ring.Draw();
		glDisable(GL_BLEND);

		basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
			forklift.Draw();
			oil.Draw();
			boxes.Draw();
			tree[0].Draw();
			helicopter.Draw();
			player.Draw();

		for (int i = 0; i < numBombs; i++)
		{
			tnt[i].Draw();
		}

		glEnable(GL_BLEND);
		basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mDecalProjection2);
			fire.Draw();
			tree[1].Draw();
			vent.Draw();
		basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
			orangefence.Draw();
		glDisable(GL_BLEND);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	msaaBuffer.UnBind();

	//Draw just the bombs onto the main buffer
	mainBuffer.Bind();
	basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mBombProj);
	basicShaderProgram.setUniform("ambient", 1.0f);
	for (int i = 0; i < numBombs; i++)
	{
		tnt[i].Draw();
	}
	basicShaderProgram.setUniform("projectionMatrix", SceneCamera.mProjection);
	basicShaderProgram.setUniform("ambient", -1.0f);
	player.Draw();
	mainBuffer.UnBind();

	{
		//High Pass
		glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
		bloomHighPass.UseProgram();
		bloomHighPass.setUniform("uTex", 0);
		bloomHighPass.setUniform("uThreshold", 0.0f);

		workBuffer1.Bind();

		glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		workBuffer1.UnBind();

		//Compute Blur
		glViewport(0, 0, (MAX_WIDTH + EDGE_OFFSET) / BLOOM_DOWNSCALE, (MAX_HEIGHT + EDGE_OFFSET) / BLOOM_DOWNSCALE);
		for (int i = 0; i < 2; i++)
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
		/*int x = 0 - (EDGE_OFFSET * (MAX_WIDTH / WIDTH) / 2);
		int y = 0 - (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT) / 2);
		int w = WIDTH + (EDGE_OFFSET * (MAX_WIDTH / WIDTH));
		int h = HEIGHT + (EDGE_OFFSET * (MAX_HEIGHT / HEIGHT));
		glViewport(x, y, w, h);*/

		glViewport(0, 0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);

		bloomComposite.UseProgram();
		bloomComposite.setUniform("uScene", 0);
		bloomComposite.setUniform("uBloom", 1);

		mainBuffer.Clear();
		msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer);
		msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer, true);

		msaaBuffer.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, workBuffer1.GetColorHandle(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mainBuffer.GetColorHandle(0));
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		msaaBuffer.UnBind();
	}
	//mainBuffer.Clear();
	workBuffer1.Clear();
	workBuffer2.Clear();

	msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer);
	/*msaaBuffer.BlitToBuffer(MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET, &mainBuffer, true);*/

	//just incase
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glActiveTexture(GL_TEXTURE0);

	glViewport(0, 0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);

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

		matrix = glm::translate(matrix, glm::vec3(0.0, 2.35, 0.0));

		glm::vec3 pos = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

		billboard.setUniform("uModel", glm::translate(glm::mat4x4(1.0), pos));
		sparks.Render();
	}

	player.bombMat = glm::translate(player.bombMat, glm::vec3(player.bombMat[1][0], player.bombMat[1][1], player.bombMat[1][2]) * 2.35f);

	currentLevel->billboard.setUniform("uModel", player.matrix * player.bombMat);
	currentLevel->sparks.Render();

	//explosion.Render();

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

	if (helicopter.hasCar)
	{
		ShowCursor(FALSE);

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		passThrough.UseProgram();
		passThrough.setUniform("uTex", 0);
		passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
		passThrough.setUniform("alpha", (-helicopter.timer / 5.f) + 1.f);

		black.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		passThrough.setUniform("alpha", 1.f);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Level1::Release()
{
	backgroundAudio->release();
	player.engineLoop->release();
	helicopter.Unload();
}