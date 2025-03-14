#pragma once

#include "stdafx.h"

using namespace glm;

class cHelicopter
{
public:
	//cHelicopter();
	cHelicopter(){}
	~cHelicopter();

	void Init();
	void Unload();

	void Draw();
	void Update();

	vec3 GetPos() { return position; }
	vec3 GetRot() { return rotation; }
	vec3 GetScl() { return scale; }

	void SetPos(vec3 p) { position = p; }
	void SetRot(vec3 r) { rotation = r; }
	void SetScl(vec3 s) { scale = s; }
	void UpdateModelMatrix();

	void SetShaderProgram(cShaderProgram* sp)
	{
		shaderProgram = sp;
	}

	mat4x4 matrix;
	cMesh bodyModel, bladeModel, magnetModel;
	cTexture texture;

	bool hasCar = false;
	float timer = 0.0f;

private:
	vec3 position;
	vec3 positionAdd; //This is for the hover effect
	vec3 rotation;
	vec3 scale;

	cShaderProgram* shaderProgram;
	float dt = 0.f;

	vec3 direction = glm::vec3(0.0, 0.0, 1.0);
	vec3 lerpedDirection;
	float speed = 0.0;

	int iterator = 0; //temp
	glm::vec3 pickupPos = glm::vec3(-9999.f, 0, 0);

	AStar pathA;
	AStar pathB;
	std::vector<glm::vec3> pathToFollow;

	FMOD::Sound *bladeChopping;
	FMOD::Channel *bladeChannel;
};

class cBoxDynamic
{
public:
	void Init(const std::string file);
	void Draw();

	vec3 GetPos() { return position; }
	vec3 GetRot() { return rotation; }
	vec3 GetScl() { return scale; }

	void SetPos(vec3 p) { position = p; }
	void SetRot(vec3 r) { rotation = r; }
	void SetScl(vec3 s) { scale = s; }
	void UpdateModelMatrix();

	void SetShaderProgram(cShaderProgram* sp)
	{
		shaderProgram = sp;
	}

	mat4x4 matrix;
	cMesh model;
	cTexture texture;

	btMotionState *_motionState;
	btRigidBody *body;
	cShaderProgram* shaderProgram;

	std::vector<btRigidBody*> boxes;
	std::vector<int> boxTextureID;
	std::vector<glm::vec3> boxScale;

	cTexture boxTexture;
	cMesh box[2];

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;
};