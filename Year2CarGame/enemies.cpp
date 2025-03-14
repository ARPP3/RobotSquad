#include "stdafx.h"

cHelicopter::~cHelicopter()
{
	bodyModel.Unload();
	bladeModel.Unload();
	magnetModel.Unload();
}

void cHelicopter::Init()
{
	//Model shit
	scale = glm::vec3(1.0, 1.0, 1.0);
	//PathFinding garbage
	pathA.ReadNavMesh(".\\Assets\\PhysicsLevels\\navMeshWarehouse.graph");
	pathA.SetStartAndGoal(glm::vec3(75.0, 0.0, 67.0), glm::vec3(164.0, 0.0, 67.0));
	pathA.FindPath();
	pathB = pathA;
	pathB.SetStartAndGoal(glm::vec3(164.0, 0.0, 116.0), glm::vec3(75.0, 0.0, 116.0));
	pathB.FindPath();
	//Movement crap
	pathToFollow.reserve(pathA.pathBack.size() + pathB.pathBack.size());
	pathToFollow.insert(pathToFollow.end(), pathB.pathBack.begin(), pathB.pathBack.end()); //These are inserted in REVERSE order
	pathToFollow.insert(pathToFollow.end(), pathA.pathBack.begin(), pathA.pathBack.end());
	direction = pathToFollow[pathToFollow.size() - 1];
	position = glm::vec3(75.0, 0.0, 67.0);
	direction = glm::normalize(direction - position);
	speed = 5.f/60.f;
	//sound stuff
	FMod.CheckError(FMod.system->createSound(".\\Assets\\Sounds\\helicopterSound.wav", FMOD_3D, 0, &bladeChopping));
	bladeChopping->setMode(FMOD_LOOP_NORMAL);
	FMod.CheckError(FMod.system->playSound(FMOD_CHANNEL_FREE, bladeChopping, false, &bladeChannel));
	bladeChannel->setChannelGroup(currentLevel->Channels);
}

void cHelicopter::Unload()
{
	direction = glm::vec3(0.0, 0.0, 0.0);
	position = glm::vec3(0.0, 0.0, 0.0);
	pathToFollow.clear();
	bladeChopping->release();
}

void cHelicopter::Update()
{
	FMOD_VECTOR bladePos = { position.x + positionAdd.x, position.y + positionAdd.y + 5.f, position.z + positionAdd.z };
	FMOD_VECTOR bladeVel = { 0.0, 0.0, 0.0 };
	bladeChannel->set3DAttributes(&bladePos, &bladeVel);

	dt += 1.f;

	if (glm::distance(position, pathToFollow[(int)pathToFollow.size() - 1 - (iterator % (pathToFollow.size()))]) < 2.f)
	{
		iterator++;
		direction = pathToFollow[(int)pathToFollow.size() - 1 - (iterator % (pathToFollow.size()))];
		direction = glm::normalize(direction - position);
	}
	position += direction * speed; //Lerp(position, direction, appMain.sof(6.f));
	lerpedDirection = Lerp(lerpedDirection, direction, appMain.sof(1.0f));


	positionAdd = glm::vec3(sin(dt* 0.53f * 3.14f / 180.f)/4.f, sin(dt * 3.14f / 180.f)/2.f, sin(dt* 0.45f * 3.14f / 180.f)/4.f);
	UpdateModelMatrix();

	btTransform t;	currentLevel->player.playerRobot.car.chassisMotionState->getWorldTransform(t);
	glm::vec3 a = position;
	glm::vec3 b = glm::vec3(t.getOrigin().x(), t.getOrigin().y() + 1.0f, t.getOrigin().z());

	float distance = glm::sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));

	if (distance < 20)
	{
		SceneCamera.SetTar(Lerp(b, a + positionAdd + glm::vec3(0.0, 5.0, 0.0), cos(((20.f - distance) / 20.f) * 3.1415) * -0.5 + 0.5));

		SceneCamera.mView = glm::lookAt(SceneCamera.GetPos(), SceneCamera.GetTar(), glm::vec3(0.f, 1.f, 0.f));
		//SceneCamera.mProjection = glm::perspective(Camera.FOV, Camera.Ratio, Camera.NearClip, Camera.FarClip);
		glm::vec3 forwardV = glm::normalize(SceneCamera.GetTar() - SceneCamera.GetPos());

		FMOD_VECTOR pos = { SceneCamera.GetPos().x, SceneCamera.GetPos().y, SceneCamera.GetPos().z };
		FMOD_VECTOR vel = { 0.0, 0.0, 0.0 };
		FMOD_VECTOR forward = { forwardV.x, forwardV.y, forwardV.z };
		FMOD_VECTOR up = { 0.0, 1.0, 0.0 };
		FMod.CheckError(FMod.system->set3DListenerAttributes(0, &pos, &vel, &forward, &up));
	}

	if (distance < 5.f)
	{
		if (hasCar == false)
		{
			timer = 5.0f;
			pickupPos = SceneCamera.GetPos();
		}
		hasCar = true;
	}
	if (hasCar)
	{
		timer -= (float)fmod(appMain.sof(0.4f), 0.1);		//0.4 is the closest to 1 second
	}

	if (timer <= 0.0 && hasCar == true)
	{
		btTransform t;	currentLevel->player.playerRobot.car.chassisMotionState->getWorldTransform(t);
		currentLevel->player.playerRobot.car.gVehicleSteering = 0.f;
		currentLevel->player.playerRobot.car.chassisMotionState->setWorldTransform(btTransform
			(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 1.0, 0.0)));
		currentLevel->player.playerRobot.car.chassisRigidBody->setCenterOfMassTransform(btTransform
			(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 1.0, 0.0)));
		currentLevel->player.playerRobot.car.chassisRigidBody->setLinearVelocity(btVector3(0, 0, 0));
		currentLevel->player.playerRobot.car.chassisRigidBody->setAngularVelocity(btVector3(0, 0, 0));
		currentLevel->physics.world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs
			(currentLevel->player.playerRobot.car.chassisRigidBody->getBroadphaseHandle(),
			currentLevel->physics.world->getDispatcher());
		if (currentLevel->player.playerRobot.car.vehicle)
		{
			currentLevel->player.playerRobot.car.vehicle->resetSuspension();
			for (int i = 0; i<currentLevel->player.playerRobot.car.vehicle->getNumWheels(); i++)
			{
				//synchronize the wheels with the (interpolated) chassis world transform
				currentLevel->player.playerRobot.car.vehicle->updateWheelTransform(i, true);
			}
		}
		hasCar = false;
	}

	currentLevel->player.isPickedUp = hasCar;
}

void cHelicopter::UpdateModelMatrix()
{
	//lerpedDirection = Lerp(lerpedDirection, (position + positionAdd) + ((glm::normalize(direction - position) * 5.f)), appMain.sof(1.0));

	//Rotate
	matrix =	glm::inverse(glm::lookAt(position + positionAdd + glm::vec3(0.0, 5.0, 0.0),
				position + lerpedDirection + positionAdd + glm::vec3(0.0, 4.9, 0.0),
				glm::vec3(0.0, 1.0, 0.0)));
	//Scale
	matrix = glm::scale(matrix, scale);

	if (hasCar)
	{
		btTransform t;
		glm::mat4x4 mag = glm::translate(matrix, glm::vec3(0.0, -2.0, 0.0));
		mag = glm::rotate(mag, 180.f, glm::vec3(0.0, 1.0, 0.0));
	
		btScalar m[16] =
		{
			mag[0][0], mag[0][1], mag[0][2], mag[0][3],
			mag[1][0], mag[1][1], mag[1][2], mag[1][3],
			mag[2][0], mag[2][1], mag[2][2], mag[2][3],
			mag[3][0], mag[3][1], mag[3][2], mag[3][3]
		};

		t.setFromOpenGLMatrix(m);
		currentLevel->player.playerRobot.car.chassisRigidBody->setWorldTransform(t);
		currentLevel->player.playerRobot.car.chassisMotionState->setWorldTransform(t);

		SceneCamera.SetTar(Lerp(SceneCamera.GetTar(), glm::vec3(t.getOrigin().x(), t.getOrigin().y() + 1.0f, t.getOrigin().z()), appMain.sof(0.5f)));
		SceneCamera.SetPos(pickupPos);

		SceneCamera.mView = glm::lookAt(SceneCamera.GetPos(), SceneCamera.GetTar(), glm::vec3(0.f, 1.f, 0.f));
		//SceneCamera.mProjection = glm::perspective(Camera.FOV, Camera.Ratio, Camera.NearClip, Camera.FarClip);
		glm::vec3 forwardV = glm::normalize(SceneCamera.GetTar() - SceneCamera.GetPos());

		FMOD_VECTOR pos = { SceneCamera.GetPos().x, SceneCamera.GetPos().y, SceneCamera.GetPos().z };
		FMOD_VECTOR vel = { 0.0, 0.0, 0.0 };
		FMOD_VECTOR forward = { forwardV.x, forwardV.y, forwardV.z };
		FMOD_VECTOR up = { 0.0, 1.0, 0.0 };
		FMod.CheckError(FMod.system->set3DListenerAttributes(0, &pos, &vel, &forward, &up));
	}
}

void cHelicopter::Draw()
{
	//Body
	shaderProgram->setUniform("modelMatrix", matrix);
	shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));

	texture.BindTexture(0);

	glBindVertexArray(bodyModel.VAO);
	glDrawArrays(GL_TRIANGLES, 0, bodyModel.GetNumVerticies());
	glBindVertexArray(0);

	//Blades
	glm::mat4x4 blades = glm::rotate(matrix, (float)(GetTickCount() % 360), glm::vec3(0.0, 1.0, 0.0));

	shaderProgram->setUniform("modelMatrix", blades);
	shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(blades)));

	glBindVertexArray(bladeModel.VAO);
	glDrawArrays(GL_TRIANGLES, 0, bladeModel.GetNumVerticies());
	glBindVertexArray(0);

	//Magnet
	glm::mat4x4 magnet = glm::translate(matrix, glm::vec3(0.0, -1.0, 0.0));

	shaderProgram->setUniform("modelMatrix", magnet);
	shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(magnet)));

	glBindVertexArray(magnetModel.VAO);
	glDrawArrays(GL_TRIANGLES, 0, magnetModel.GetNumVerticies());
	glBindVertexArray(0);
}

void cBoxDynamic::Init(const std::string file)
{
	FILE *physicsFile = nullptr;
	physicsFile = fopen(file.c_str(), "rb");

	if (physicsFile == nullptr)
	{
		HWND* hWnd = appMain.oglControl.get_hWnd();
		wchar_t errorMessage;
		wsprintf(&errorMessage, L"Cannot open file for reading: %s", file);
		MessageBox(*hWnd, &errorMessage, L"File Error", MB_ICONERROR);
		return;
	}

	glm::vec3 p;
	glm::vec3 r;
	glm::vec3 s;

	while (fscanf(physicsFile, "%f %f %f %f %f %f %f %f %f", &p.x, &p.y, &p.z, &r.x, &r.y, &r.z, &s.x, &s.y, &s.z) != EOF)
	{
		btBoxShape *shape = new btBoxShape(btVector3(s.x / 2.f, s.y / 2.f, s.z / 2.f));
		btRigidBody *body;
		btScalar _mass = 20.0;
		_motionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(1.0, 0.0, 0.0), btScalar(90 * (PI / 180.f))), btVector3(position.x, position.y, position.z)));
		btVector3 boxInertia;

		shape->calculateLocalInertia(_mass, boxInertia);

		btTransform t;
		t.setOrigin(btVector3(btScalar(-1.f * p.x), btScalar(p.y), btScalar(p.z)));

		t.setRotation(btQuaternion(btVector3(1.0, 0.0, 0.0), btScalar(r.x * (PI / 180.f))));
		t.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), btScalar(r.y * (PI / 180.f))) * t.getRotation());
		t.setRotation(btQuaternion(btVector3(0.0, 0.0, 1.0), btScalar(r.z * (PI / 180.f))) * t.getRotation());

		body = new btRigidBody(_mass, _motionState, shape, boxInertia);
		body->setWorldTransform(t);
		body->setFriction(10.f);
		// Be sure to add the chassis of the vehicle into the world as a rigid body
		currentLevel->physics.world->addRigidBody(body);
		currentLevel->physics.bodies.push_back(new bulletObject(5, body));
		body->setUserPointer(currentLevel->physics.bodies[currentLevel->physics.bodies.size() - 1]);

		boxes.push_back(body);
		boxTextureID.push_back(RandomRangei(0, 1));
		boxScale.push_back(s);
	}

	fclose(physicsFile);

	boxTexture.LoadTexture(".\\Assets\\Textures\\fire.jpg", true);
	boxTexture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	box[0].LoadFromFile(".\\Assets\\Models\\box1.mesh");
	box[1].LoadFromFile(".\\Assets\\Models\\box2.mesh");
}

void cBoxDynamic::Draw()
{
	btTransform t;
	float m[16];
	glm::mat4x4 matrix;

	for (int i = 0; i < boxes.size() - 1; i++)
	{
		t = boxes[i]->getWorldTransform();
		t.getOpenGLMatrix(m);

		matrix = glm::mat4x4 (
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

		matrix = glm::scale(matrix, boxScale[i]);

		shaderProgram->setUniform("modelMatrix", matrix);
		shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));

		boxTexture.BindTexture(0);

		glBindVertexArray(box[boxTextureID[i]].VAO);
		glDrawArrays(GL_TRIANGLES, 0, box[boxTextureID[i]].GetNumVerticies());
		glBindVertexArray(0);
	}
}

void cBoxDynamic::UpdateModelMatrix()
{
	
}