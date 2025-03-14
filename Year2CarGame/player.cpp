#include "stdafx.h"

float Lerp(float a, float b, float u)
{
	return  a*(1 - u) + b*u;
}

glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float u)
{
	glm::vec3 lerped;
	lerped.x = a.x*(1 - u) + b.x*u;
	lerped.y = a.y*(1 - u) + b.y*u;
	lerped.z = a.z*(1 - u) + b.z*u;
	return lerped;
}

void SmoothFollow(const float yRot, const float rotDamp, const float heightDamp, const float height, const float targetY, const float distanceFromTarget)
{
	glm::vec3 wanted = glm::vec3(SceneCamera.GetTar().x - sin((-yRot * PI) / 180) * distanceFromTarget, SceneCamera.GetTar().y + height,
		SceneCamera.GetTar().z - cos((-yRot * PI) / 180) * distanceFromTarget);

	glm::vec3 pos;

	// Damp the rotation around the y-axis
	pos.x = Lerp(SceneCamera.GetPos().x, wanted.x, rotDamp * 0.017f);
	pos.y = Lerp(SceneCamera.GetPos().y, wanted.y - targetY, heightDamp * 0.017f);
	pos.z = Lerp(SceneCamera.GetPos().z, wanted.z, rotDamp * 0.017f);

	glm::vec3 dir = glm::normalize(pos - SceneCamera.GetTar()) * 1.95f;

	btVector3 start = btVector3(SceneCamera.GetTar().x + dir.x, SceneCamera.GetTar().y + dir.y, SceneCamera.GetTar().z + dir.z);
	btVector3 end = btVector3(pos.x, pos.y, pos.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

	// Perform raycast
	currentLevel->physics.world->rayTest(start, end, RayCallback);
	if (RayCallback.hasHit()) {

		end = RayCallback.m_hitPointWorld;
	}

	//SceneCamera.SetPos(pos);

	SceneCamera.SetPos(glm::vec3(end.x(), end.y(), end.z()));
}

float QuatToEulerY(glm::mat4x4 m)
{
	const double RD_TO_DEG = 180 / PI;
	double h;

	/* Quaternion Y to Euler Y */
	{
		double sinP = -m[2][1];

		// extract heading
		if (sinP < -0.9999 || sinP > 0.9999) { // account for small angle errors
			h = atan2(-m[0][2], m[0][0]) * RD_TO_DEG;
		}
		else {
			h = atan2(m[0][2], m[2][2]) * RD_TO_DEG;
		}
	}
	return (float) h;
}

void Player::Init()
{
	robotMesh.LoadFromFile(".\\Assets\\Models\\Body.mesh");
	wheelMesh[0].LoadFromFile(".\\Assets\\Models\\FL.mesh");
	wheelMesh[1].LoadFromFile(".\\Assets\\Models\\FR.mesh");
	wheelMesh[2].LoadFromFile(".\\Assets\\Models\\BL.mesh");
	wheelMesh[3].LoadFromFile(".\\Assets\\Models\\BR.mesh");

	robotTexture.LoadTexture(".\\Assets\\Textures\\robotTexture.jpg", true);
	robotTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	//HUD textures

	hud.LoadTexture(".\\Assets\\Textures\\hud.png", false);
	hud.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	spaceMessage.LoadTexture(".\\Assets\\Textures\\msg_pickUpBomb.png", false);
	spaceMessage.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	moveMessage.LoadTexture(".\\Assets\\Textures\\msg_tryToMove.png", false);
	moveMessage.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	tutorialMessage.LoadTexture(".\\Assets\\Textures\\msg_tutorial.png", false);
	tutorialMessage.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	levelMessage.LoadTexture(".\\Assets\\Textures\\msg_bombDefused.png", false);
	levelMessage.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);

	mouse.LoadTexture(".\\Assets\\Textures\\mouseArrow.png", false);
	mouse.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR);

	tutorialReturn.LoadTexture(".\\Assets\\Textures\\msg_tutorialReturn.png", false);
	tutorialReturn.SetFiltering(TEXTURE_FILTER_MAG_NEAREST,
		TEXTURE_FILTER_MIN_NEAREST);


	digitalNumbers[0].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\0.png", false);
	digitalNumbers[1].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\1.png", false);
	digitalNumbers[2].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\2.png", false);
	digitalNumbers[3].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\3.png", false);
	digitalNumbers[4].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\4.png", false);
	digitalNumbers[5].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\5.png", false);
	digitalNumbers[6].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\6.png", false);
	digitalNumbers[7].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\7.png", false);
	digitalNumbers[8].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\8.png", false);
	digitalNumbers[9].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\9.png", false);
	digitalNumbers[10].LoadTexture(".\\Assets\\Textures\\DigitalNumbers\\colon.png", false);
	FOR(i, 11)	digitalNumbers[i].SetFiltering(TEXTURE_FILTER_MAG_NEAREST, TEXTURE_FILTER_MIN_NEAREST);

	cartoonNumbers[0].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\0.png", false);
	cartoonNumbers[1].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\1.png", false);
	cartoonNumbers[2].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\2.png", false);
	cartoonNumbers[3].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\3.png", false);
	cartoonNumbers[4].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\4.png", false);
	cartoonNumbers[5].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\5.png", false);
	cartoonNumbers[6].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\6.png", false);
	cartoonNumbers[7].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\7.png", false);
	cartoonNumbers[8].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\8.png", false);
	cartoonNumbers[9].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\9.png", false);
	cartoonNumbers[10].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\10.png", false);
	cartoonNumbers[11].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\11.png", false);
	cartoonNumbers[12].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\12.png", false);
	cartoonNumbers[13].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\13.png", false);
	cartoonNumbers[14].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\14.png", false);
	cartoonNumbers[15].LoadTexture(".\\Assets\\Textures\\CartoonNumbers\\15.png", false);
	FOR(i, 16)	cartoonNumbers[i].SetFiltering(TEXTURE_FILTER_MAG_NEAREST, TEXTURE_FILTER_MIN_NEAREST);

	/* For the physics of the Robot */
	glm::vec3 Inertia = glm::vec3{ 0.0f, 0.0f, 0.0f };
	glm::vec3 HalfExtents = glm::vec3{ 1.5f, 0.5f, 1.91f };
	glm::vec3 Rotation = glm::vec3{ 0.f, 0.f, 0.f };
	glm::vec3 Position = glm::vec3{ 0.f, 1.f, 0.f };
	glm::vec4 FRPosition = glm::vec4{ -1.275f, 0.f, 1.719f, 0.37f };
	glm::vec4 FLPosition = glm::vec4{  1.275f, 0.f, 1.719f, 0.37f };
	glm::vec4 BRPosition = glm::vec4{ -1.275f, 0.f, -1.221f, 0.47f };
	glm::vec4 BLPosition = glm::vec4{  1.275f, 0.f, -1.221f, 0.47f };

	playerRobot.SetupBulletCar(currentLevel->physics, 1000.f, Inertia, HalfExtents,
		Rotation, Position, FRPosition, FLPosition, BRPosition, BLPosition);

	playerRobot.car.chassisRigidBody->setCollisionFlags(playerRobot.car.chassisRigidBody->getCollisionFlags() |
		btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\pickUpSound.mp3", FMOD_3D, 0, &pickUpSound));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\dropDownSound.mp3", FMOD_3D, 0, &dropDownSound));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\engineLoop.wav", FMOD_3D, 0, &engineLoop));
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\crash.wav", FMOD_3D, 0, &crash));
	engineLoop->setMode(FMOD_LOOP_NORMAL);
	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, engineLoop, false, &engineChannel));
	engineChannel->setChannelGroup(currentLevel->Channels);
	clawChannel->setChannelGroup(currentLevel->Channels);
	engineChannel->setVolume(0.f);	

	if (!smokeEffect.Init(".\\Assets\\Textures\\smokeParticle.png", 250, 1))
	{
		MessageBox(appMain.hWnd, L"Particle Failed to Initialize", L"PARTICLE ERROR", MB_ICONERROR);
	}

	smokeEffect.LerpAlpha = glm::vec2(0.1f, 0.0f);
	smokeEffect.LerpSize = glm::vec2(0.3f, 1.0f);
	smokeEffect.RangeLifetime = glm::vec2(1.0f, 3.0f);

	smokeEffect.RangeVelocityX = glm::vec2(-0.1, 0.1);
	smokeEffect.RangeVelocityY = glm::vec2(0.0, 0.1);
	smokeEffect.RangeVelocityZ = glm::vec2(-0.1, 0.1);

	smokeEffect.RangeX = glm::vec2(-0.5, 0.5);
	smokeEffect.RangeY = glm::vec2(-0.161, 0.161);
	smokeEffect.RangeZ = glm::vec2(-0.5, 0.5);
}

void Player::HandleEvents()
{
	if (currentLevel->timer > 0.0f || isTutorial)
	{
		BulletCar* car = &playerRobot.car;

		btTransform t;
		car->chassisMotionState->getWorldTransform(t);

		engineChannel->setFrequency(44100 * ((glm::abs(car->vehicle->getCurrentSpeedKmHour()) + 65.f) / 145.f));

		// S key : Move Backward
		if ((Keys::Key('S') || Keys::Key(VK_DOWN)) || (((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f) > 0.03))
		{
			if (car->vehicle->getCurrentSpeedKmHour() <= 0.1f)
			{
				if (((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f) > 0.03)
				{
					car->gEngineForce = (-car->maxEngineForce / 2.f) * ((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f);
				}
				else
				{
					car->gEngineForce = -car->maxEngineForce / 2.f;
				}
				float f;		engineChannel->getVolume(&f);
				engineChannel->setVolume(Lerp(f, 1.0, 1.f / 60.f));
			}
			else
			{
				if (((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f) > 0.03)
				{
					car->gBreakingForce = car->maxBreakingForce * ((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f);
				}
				else
				{
					car->gBreakingForce = car->maxBreakingForce;
				}
			}
		}

		// W key : Move Forward
		if ((Keys::Key('W') || Keys::Key(VK_UP) || (((float)Controller->GetState().Gamepad.bRightTrigger / 255.f) > 0.03)) && car->vehicle->getCurrentSpeedKmHour() < 80.f)
		{
			if (((float)Controller->GetState().Gamepad.bRightTrigger / 255.f) > 0.03)
			{
				car->gEngineForce = car->maxEngineForce * ((float)Controller->GetState().Gamepad.bRightTrigger / 255.f);
			}
			else
			{
				car->gEngineForce = car->maxEngineForce;
			}

			float f;		engineChannel->getVolume(&f);
			engineChannel->setVolume(Lerp(f, 1.0, 1.f / 60.f));
		}
		if (!(Keys::Key('W') || Keys::Key(VK_UP)) && !(Keys::Key('S') || Keys::Key(VK_DOWN))
			&& (((float)Controller->GetState().Gamepad.bRightTrigger / 255.f) < 0.03)
			&& (((float)Controller->GetState().Gamepad.bLeftTrigger / 255.f) < 0.03))
		{
			float f;		engineChannel->getVolume(&f);
			engineChannel->setVolume(Lerp(f, glm::max(0.0f, car->vehicle->getCurrentSpeedKmHour() - 30.f) / 50.f, 1.f / (60.f / 20.f)));
		}

		float speedFactor = car->vehicle->getCurrentSpeedKmHour() / 85.f;
		speedFactor = (speedFactor * -1.f) + 1.7f;

		car->gVehicleSteering = (Controller->GetStick(Gamepad::LEFT_STICK_X) != 0.0) ? -car->steeringClamp * Controller->GetStick(Gamepad::LEFT_STICK_X) : 0.0f;
		car->gVehicleSteering = ((Keys::Key('A') || Keys::Key(VK_LEFT))) ? car->steeringClamp : ((Keys::Key('D') || Keys::Key(VK_RIGHT))) ? -car->steeringClamp : car->gVehicleSteering;

		car->gVehicleSteering = car->gVehicleSteering * speedFactor;

		if (showTutorialMessage && isTutorial)
		{
			if (Keys::Onekey(VK_LBUTTON))
			{
				if (Keys::MousePosition(false).x > X_PERCENT(22.0625f) && Keys::MousePosition(false).x < X_PERCENT(44.8125f) &&
					Keys::MousePosition(false).y > Y_PERCENT(36.777f) && Keys::MousePosition(false).y < Y_PERCENT(48.666f))
				{
					gameState = 1;
				}
				if (Keys::MousePosition(false).x > X_PERCENT(55.f) && Keys::MousePosition(false).x < X_PERCENT(78.f) &&
					Keys::MousePosition(false).y > Y_PERCENT(36.777f) && Keys::MousePosition(false).y < Y_PERCENT(48.666f))
				{
					gameState = 3;
				}
			}
		}

		if (showLevelMessage && !isTutorial)
		{
			if (Keys::Onekey(VK_LBUTTON))
			{
				if (Keys::MousePosition(false).x > X_PERCENT(22.0625f) && Keys::MousePosition(false).x < X_PERCENT(44.8125f) &&
					Keys::MousePosition(false).y > Y_PERCENT(60.f) && Keys::MousePosition(false).y < Y_PERCENT(73.f))
				{
					gameState = 1;
				}
				if (Keys::MousePosition(false).x > X_PERCENT(55.f) && Keys::MousePosition(false).x < X_PERCENT(78.f) &&
					Keys::MousePosition(false).y > Y_PERCENT(60.f) && Keys::MousePosition(false).y < Y_PERCENT(73.f))
				{
					gameState = 1;
				}
			}
		}

		bool breakout = false;

		if (haveBomb == -1)
		{
			for (int i = 0; i < numBombs; i++)
			{
				if (breakout) break;
				cBomb temp = currentLevel->tnt[i];
				btTransform t;	temp._motionState->getWorldTransform(t);
				btTransform t2;	playerRobot.car.chassisMotionState->getWorldTransform(t2);
				glm::vec3 a = glm::vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());
				glm::vec3 b = glm::vec3(t2.getOrigin().x(), t2.getOrigin().y(), t2.getOrigin().z());

				float distance = glm::sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));

				if (distance < 5.f)
				{
					showSpaceMessage = true;

					if (Keys::Onekey(32))
					{
						breakout = true;
						haveBomb = i;
						cBomb tnt;
						tnt.Init(glm::vec3(-90000.f, 0.f, 0.f));
						tnt.model = currentLevel->tnt[i].model;
						tnt.texture = currentLevel->tnt[i].texture;
						tnt.SetShaderProgram(currentLevel->tnt[i].shaderProgram);

						FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, pickUpSound, false, &clawChannel));

						currentLevel->tnt[i].body->~btRigidBody();
						currentLevel->tnt[i].body->setWorldTransform(btTransform(btQuaternion(0.f, 0.f, 0.f, 1.f), btVector3(-9000.f, 0.f, 0.f)));
						//currentLevel->tnt[i].body = nullptr;
						currentLevel->tnt[i] = tnt;
					}
				}
			}
		}
		else
		{
			showTutorialReturn = false;
			btTransform t2;	playerRobot.car.chassisMotionState->getWorldTransform(t2);
			glm::vec3 a = glm::vec3(30.0, 2.0, 30.0);
			glm::vec3 b = glm::vec3(t2.getOrigin().x(), t2.getOrigin().y(), t2.getOrigin().z());

			float distance = glm::sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));

			if (distance < 10.f)
			{
				showTutorialReturn = true;
			}

			if (Keys::Onekey(32))
			{
				glm::mat4x4 matrix;
				float m[16];			btTransform t;

				playerRobot.car.chassisMotionState->getWorldTransform(t);
				t.getOpenGLMatrix(m);

				matrix = glm::mat4x4
					(
					m[0], m[1], m[2], m[3],
					m[4], m[5], m[6], m[7],
					m[8], m[9], m[10], m[11],
					m[12], m[13], m[14], m[15]
					);

				matrix = glm::translate(matrix, glm::vec3(0.0, 0.0, 3.0));

				glm::vec3 pos = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

				cBomb tnt;
				tnt.Init(pos);
				tnt.model = currentLevel->tnt[haveBomb].model;
				tnt.texture = currentLevel->tnt[haveBomb].texture;
				tnt.SetShaderProgram(currentLevel->tnt[haveBomb].shaderProgram);

				FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, dropDownSound, false, &clawChannel));

				currentLevel->tnt[haveBomb] = tnt;
				currentLevel->tnt[haveBomb].body->~btRigidBody();

				//currentLevel->tnt[haveBomb].body = nullptr;
				haveBomb = -1;
			}
		}

		playerRobot.car.chassisMotionState->getWorldTransform(t);
		glm::vec3 a = glm::vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z());
		glm::vec3 b = glm::vec3(0.0, 0.0, 0.0);

		float distance = glm::sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));

		if (distance < 10.f)
		{
			showMoveHelp = true;
		}

		if (isPickedUp && haveBomb != -1)
		{
			glm::mat4x4 matrix;
			float m[16];			btTransform t;

			playerRobot.car.chassisMotionState->getWorldTransform(t);
			t.getOpenGLMatrix(m);

			matrix = glm::mat4x4
				(
				m[0], m[1], m[2], m[3],
				m[4], m[5], m[6], m[7],
				m[8], m[9], m[10], m[11],
				m[12], m[13], m[14], m[15]
				);

			matrix = glm::translate(matrix, glm::vec3(0.0, 0.0, 3.0));

			glm::vec3 pos = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

			cBomb tnt;
			tnt.Init(pos);
			tnt.model = currentLevel->tnt[haveBomb].model;
			tnt.texture = currentLevel->tnt[haveBomb].texture;
			tnt.SetShaderProgram(currentLevel->tnt[haveBomb].shaderProgram);

			FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, dropDownSound, false, &clawChannel));

			currentLevel->tnt[haveBomb] = tnt;
			currentLevel->tnt[haveBomb].body->~btRigidBody();

			//currentLevel->tnt[haveBomb].body = nullptr;
			haveBomb = -1;
		}

		if ((Keys::Onekey('R') && isPickedUp == false))
		{
			btTransform t;	playerRobot.car.chassisMotionState->getWorldTransform(t);
			playerRobot.car.gVehicleSteering = 0.f;
			playerRobot.car.chassisRigidBody->setCenterOfMassTransform(btTransform
				(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(t.getOrigin().x(), t.getOrigin().y() + 1.f, t.getOrigin().z())));
			playerRobot.car.chassisRigidBody->setLinearVelocity(btVector3(0, 0, 0));
			playerRobot.car.chassisRigidBody->setAngularVelocity(btVector3(0, 0, 0));
			currentLevel->physics.world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(playerRobot.car.chassisRigidBody->getBroadphaseHandle(),
				currentLevel->physics.world->getDispatcher());
			if (playerRobot.car.vehicle)
			{
				playerRobot.car.vehicle->resetSuspension();
				for (int i = 0; i < playerRobot.car.vehicle->getNumWheels(); i++)
				{
					//synchronize the wheels with the (interpolated) chassis world transform
					playerRobot.car.vehicle->updateWheelTransform(i, true);
				}
			}
		}
	}
	else
	{
		int wheelIndex = 2;
		playerRobot.car.vehicle->setBrake(9999.f, wheelIndex);
		wheelIndex = 3;
		playerRobot.car.vehicle->setBrake(9999.f, wheelIndex);
	}

	btTransform t = playerRobot.car.chassisRigidBody->getWorldTransform();

	float intensity = (t.getBasis().getColumn(1).y() * -0.5f) + 0.5f;

	float Sideways = (Keys::Key('D') || Keys::Key(VK_RIGHT));
	Sideways = (Sideways == 0 ? -1.f * (Keys::Key('A') || Keys::Key(VK_LEFT)) : Sideways);

	float Forward = (Keys::Key('W') || Keys::Key(VK_UP));
	Forward =  (Forward == 0 ? -1.f * (Keys::Key('S') || Keys::Key(VK_DOWN)) : Forward);

	Sideways = (Controller->GetStick(Gamepad::LEFT_STICK_X) != 0.f ? Controller->GetStick(Gamepad::LEFT_STICK_X) : Sideways);
	Forward = (Controller->GetStick(Gamepad::LEFT_STICK_Y) != 0.f ? Controller->GetStick(Gamepad::LEFT_STICK_Y) : Forward);
	
	btTransform t2;

	//Slo Mo
	playerRobot.car.chassisMotionState->getWorldTransform(t2);

	btVector3 start = btVector3(t2.getOrigin().x(), t2.getOrigin().y(), t2.getOrigin().z());
	btVector3 end = btVector3(t2.getOrigin().x(), -10.f, t2.getOrigin().z());
	btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

	// Perform raycast
	currentLevel->physics.world->rayTest(start, end, RayCallback);
	if (RayCallback.hasHit())
	{
		end = RayCallback.m_hitPointWorld;
	}

	float rayDist = glm::distance(glm::vec3(end.x(), end.y(), end.z()), glm::vec3(start.x(), start.y(), start.z()));

	if (rayDist > 3.f && currentLevel->player.playerRobot.car.vehicle->getCurrentSpeedKmHour() > 60.f)
	{
		//float height = std::max(, 1.0f);
		newGameSpeed = 0.4f + 0.6 * ((std::min(rayDist - 3.f, 1.0f) * -1.f) + 1.f);
		intensity *= 0.4f;
	}

	playerRobot.car.chassisRigidBody->applyTorque(t.getBasis().getColumn(0) * (((17500.f * intensity) + 2500.f) * Forward));	//Forward
	playerRobot.car.chassisRigidBody->applyTorque(t.getBasis().getColumn(2) * (((17500.f * intensity) + 2500.f) * Sideways));	//Sideways

	currentLevel->Channels->setPitch(gameSpeed);
}

void Player::Update()
{
	float m[16];	btTransform t;
	playerRobot.car.chassisMotionState->getWorldTransform(t);
	t.getOpenGLMatrix(m);

	glm::mat4x4 matrix;

	matrix = glm::mat4x4
		(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]
		);

	float yRot = QuatToEulerY(matrix);
	float targetY = 1.0f;

	//float mouseX = Keys::MousePosition().x;
	//accumRot += mouseX;
	//mouseRot = accumRot;
	//if ((prevYRot > 0 && yRot < 0) || (prevYRot < 0 && yRot > 0)) //Rollover
	//{
	//	accumRot = fMod(accumRot + 540, 360.f) - 180.f;
	//}

	//if (mouseX == 0)	accumRot = Lerp(accumRot, yRot, 0.017);
	//prevYRot = yRot;

	SceneCamera.SetTar(glm::vec3(t.getOrigin().x(), t.getOrigin().y() + 1.0f, t.getOrigin().z()));
	SmoothFollow(yRot, 15.f, 2.0f, 2.f, 1.0f, 10.f);
	SceneCamera.FOV = Lerp(SceneCamera.FOV, 22.5 + (std::max(45.f, playerRobot.car.vehicle->getCurrentSpeedKmHour()) / 2.f), appMain.sof(1.0));

	SceneCamera.mView = glm::lookAt(SceneCamera.GetPos(), SceneCamera.GetTar(), glm::vec3(0.f, 1.f, 0.f));
	//SceneCamera.mProjection = glm::perspective(Camera.FOV, Camera.Ratio, Camera.NearClip, Camera.FarClip);
	glm::vec3 forwardV = glm::normalize(SceneCamera.GetPos() - SceneCamera.GetTar());

	FMOD_VECTOR pos = { SceneCamera.GetPos().x, SceneCamera.GetPos().y, SceneCamera.GetPos().z };
	FMOD_VECTOR vel = {	0.0, 0.0, 0.0 };
	FMOD_VECTOR forward = { forwardV.x, forwardV.y, forwardV.z };
	FMOD_VECTOR up = { 0.0, 1.0, 0.0 };
	FMod.CheckError(FMod.system->set3DListenerAttributes(0, &pos, &vel, &forward, &up));

	FMOD_VECTOR carPos = { t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z() };
	FMOD_VECTOR carVel = { 0.0, 0.0, 0.0 };
	engineChannel->set3DAttributes(&carPos, &carVel);

	int wheelIndex = 2;
	playerRobot.car.vehicle->applyEngineForce(playerRobot.car.gEngineForce, wheelIndex);
	playerRobot.car.vehicle->setBrake(playerRobot.car.gBreakingForce + 35, wheelIndex);
	wheelIndex = 3;
	playerRobot.car.vehicle->applyEngineForce(playerRobot.car.gEngineForce, wheelIndex);
	playerRobot.car.vehicle->setBrake(playerRobot.car.gBreakingForce + 35, wheelIndex);


	wheelIndex = 0;
	playerRobot.car.vehicle->setSteeringValue(playerRobot.car.gVehicleSteering, wheelIndex);
	wheelIndex = 1;
	playerRobot.car.vehicle->setSteeringValue(playerRobot.car.gVehicleSteering, wheelIndex);

	playerRobot.car.gEngineForce = 0.0f;
	playerRobot.car.gBreakingForce = 0.0f;
	playerRobot.car.gVehicleSteering = Lerp(playerRobot.car.gVehicleSteering, 0.0f, appMain.sof(1.0) * 4.f);
	
	if (!showLevelMessage)
	{
		currentLevel->timer -= (float)fmod(appMain.sof(0.4f), 0.1) * gameSpeed;		//0.4 is the closest to 1 second
		winTime = currentLevel->timer;
	}

	playerRobot.car.chassisMotionState->getWorldTransform(t);
	t.getOpenGLMatrix(m);
	smokeEffect.Transform = glm::translate(glm::mat4x4(1.0), glm::vec3(m[12] - (m[8] * 2.f),	m[13] - (m[9] * 2.f),	m[14] - (m[10] * 2.f)));
	smokeEffect.Transform = glm::translate(smokeEffect.Transform, glm::vec3(-(m[4] * 0.08f), -(m[5] * 0.08f), -(m[6] * 0.08f)));
	smokeEffect.Update(appMain.sof(1.0));
}

void Player::Draw()
{
	glViewport(0, 0, MAX_WIDTH + EDGE_OFFSET, MAX_HEIGHT + EDGE_OFFSET);

	robotTexture.BindTexture(0);

	float m[16];			btTransform t;

	// Body
	{
		playerRobot.car.chassisMotionState->getWorldTransform(t);
		t.getOpenGLMatrix(m);

		matrix = glm::mat4x4
			(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
			);

		sp->setUniform("modelMatrix", matrix);
		sp->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));

		glBindVertexArray(robotMesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, robotMesh.GetNumVerticies());
		glBindVertexArray(0);
	}

	// Wheels
	for (int i = 0; i < 4; i++)
	{
		playerRobot.car.vehicle->updateWheelTransform(i, true);
		t = playerRobot.car.vehicle->getWheelInfo(i).m_worldTransform;
		t.getOpenGLMatrix(m);

		glm::mat4x4 w_matrix;

		w_matrix = glm::mat4x4
			(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
			);

		sp->setUniform("modelMatrix", w_matrix);
		sp->setUniform("normalMatrix", glm::transpose(glm::inverse(w_matrix)));

		glBindVertexArray(wheelMesh[i].VAO);
		glDrawArrays(GL_TRIANGLES, 0, wheelMesh[i].GetNumVerticies());
		glBindVertexArray(0);
	}

	// Bomb
	if (haveBomb != -1)
	{
		playerRobot.car.chassisMotionState->getWorldTransform(t);
		t.getOpenGLMatrix(m);

		matrix = glm::mat4x4
			(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
			);

		bombMat = glm::mat4x4(1.0);
		bombMat = glm::translate(bombMat, glm::vec3(0.0, 0.7, 2.0));
		bombMat = glm::rotate(bombMat, 180.f, glm::vec3(0.0, 0.0, 1.0));
		bombMat = glm::rotate(bombMat, 30.f, glm::vec3(1.0, 0.0, 0.0));
		bombMat = glm::rotate(bombMat, 180.f, glm::vec3(1.0, 0.0, 0.0));
		matrix * bombMat;

		sp->setUniform("modelMatrix", matrix * bombMat);
		sp->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix * bombMat)));

		currentLevel->tnt[haveBomb].texture.BindTexture(0);
		glBindVertexArray(currentLevel->tnt[haveBomb].model.VAO);
		glDrawArrays(GL_TRIANGLES, 0, currentLevel->tnt[haveBomb].model.GetNumVerticies());
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		sp->UseProgram();
	}
}

void Player::DrawHUD()
{
	ShowCursor(FALSE);

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	currentLevel->passThrough.UseProgram();
	currentLevel->passThrough.setUniform("uTex", 0);
	currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));

	if (!showTutorialMessage && !showLevelMessage)	//Only draw these if there is no end screen
	{
		hud.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		if (showMoveHelp && !showSpaceMessage && haveBomb == -1 && isTutorial)
		{
			glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(50.f), Y_PERCENT(10.55f), X_PERCENT(25.f), Y_PERCENT(22.2f));
			currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);

			moveMessage.BindTexture(0);
			DrawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
			showMoveHelp = false;
		}

		if (showSpaceMessage)
		{
			glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(50.f), Y_PERCENT(10.5f), X_PERCENT(23.125f), Y_PERCENT(15.555f));
			currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);

			spaceMessage.BindTexture(0);
			DrawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
			showSpaceMessage = false;
		}

		int time = (int)ceil(currentLevel->timer);
		if (time < 0 ? time = 0 : 0);
		int seconds = time % 60;
		int minutes = (int) ((float) time - (float) seconds) / 60.f;

		int clock[5] =
		{
			(minutes - (minutes % 10)) / 10,
			(minutes % 10),
			10,
			(seconds - (seconds % 10)) / 10,
			(seconds % 10)
		};

		FOR(i, 5)
		{
			glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(((float)(1267.f + (i * 70.f)) / 1600.f) * 100.f), Y_PERCENT(9.666f), X_PERCENT(4.375f), Y_PERCENT(11.11f));
			currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
			currentLevel->passThrough.setUniform("uTex", 0);

			digitalNumbers[clock[i]].BindTexture(0);
			DrawFullScreenQuad();

			currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
			showSpaceMessage = false;
		}

		{	//Draw bomb counter
			glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(9.687f), Y_PERCENT(16.f), X_PERCENT(9.375f), Y_PERCENT(16.666f));
			currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);

			cartoonNumbers[numBombsLeft].BindTexture(0);
			DrawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
		}
	}
	if (showTutorialMessage && isTutorial)
	{
		currentLevel->passThrough.setUniform("modelMatrix", glm::mat4(1.0));
		tutorialMessage.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		Keys::locked = false;
		glm::mat4x4 modelMatrix = ScreenRectangle((int)Keys::MousePosition(false).x, (int)Keys::MousePosition(false).y, X_PERCENT(5.25f), Y_PERCENT(8.666f));
		currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
		mouse.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
	if (showLevelMessage && !isTutorial)
	{
		currentLevel->passThrough.setUniform("modelMatrix", glm::mat4(1.0));
		levelMessage.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		Keys::locked = false;
		glm::mat4x4 modelMatrix = ScreenRectangle((int)Keys::MousePosition(false).x, (int)Keys::MousePosition(false).y, X_PERCENT(5.25f), Y_PERCENT(8.666f));
		currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
		mouse.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		int time = (int)ceil(winTime);
		if (time < 0 ? time = 0 : 0);
		int seconds = time % 60;
		int minutes = (int)((float)time - (float)seconds) / 60.f;

		int clock[5] =
		{
			(minutes - (minutes % 10)) / 10,
			(minutes % 10),
			10,
			(seconds - (seconds % 10)) / 10,
			(seconds % 10)
		};

		FOR(i, 5)
		{
			glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(((float)(966.f + (i * 70.f)) / 1600.f) * 100.f), Y_PERCENT(53.f), X_PERCENT(4.375f), Y_PERCENT(11.11f));
			currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
			currentLevel->passThrough.setUniform("uTex", 0);

			digitalNumbers[clock[i]].BindTexture(0);
			DrawFullScreenQuad();

			currentLevel->passThrough.setUniform("modelMatrix", glm::mat4x4(1.0));
		}
	}
	if (showTutorialReturn && !showTutorialMessage && isTutorial)
	{
		glm::mat4x4 modelMatrix = ScreenRectangle(X_PERCENT(50.f), Y_PERCENT(13.333f), X_PERCENT(46.875f), Y_PERCENT(22.222f));
		currentLevel->passThrough.setUniform("modelMatrix", modelMatrix);
		tutorialReturn.BindTexture(0);
		DrawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}