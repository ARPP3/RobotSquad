#pragma once

#include "stdafx.h"

using namespace glm;

class cObject
{
public:
	cObject();
	cObject(string _model, string _texture, cShaderProgram* sp)
	{
		model.LoadFromFile(".\\Assets\\Models\\" + _model);
		texture.LoadTexture(".\\Assets\\Textures\\" + _texture);
		texture.SetFiltering(TEXTURE_FILTER_MAG_BILINEAR,
			TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
		shaderProgram = sp;
	}
	~cObject();

	virtual void Draw();

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

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;

	cShaderProgram* shaderProgram;
};

class cPlanet : public cObject
{
	void Update()
	{

	}
};

class cCamera
{
public:
	cCamera()
	{
		position = vec3(0.0, 1.0, -5.0);
		target = vec3(0.0, 0.0, 0.0);

		FOV = 45.f;
		AR = 1.0f;

		mView = glm::lookAt(position, target, glm::vec3(0.0, 1.0, 0.0));
		mProjection = glm::perspective(FOV, 1.0f, 0.01f, 1000.f);
		mDecalProjection = glm::perspective(FOV, 1.0f, 0.0101f, 1010.f);
		mDecalProjection2 = glm::perspective(FOV, 1.0f, 0.0102f, 1020.f);

		mDecalProjection2 = glm::perspective(FOV, 1.0f, 0.01f, 250.f);
	}
	~cCamera(){}

	void UpdateProjMatrix()
	{
		mProjection = glm::perspective(FOV * (1.f + (1.f - gameSpeed)), AR, 0.01f, 1000.f);
		mDecalProjection = glm::perspective(FOV * (1.f + (1.f - gameSpeed)), AR, 0.0101f, 1010.f);
		mDecalProjection2 = glm::perspective(FOV * (1.f + (1.f - gameSpeed)), AR, 0.0102f, 1020.f);

		mBombProj = glm::perspective(FOV * (1.f + (1.f - gameSpeed)), AR, 0.01f, 250.f);
	}

	vec3 GetPos() { return position; }
	vec3 GetTar() { return target; }

	void SetPos(vec3 p) { position = p; }
	void SetTar(vec3 t) { target = t; }
	void UpdateViewMatrix();

	mat4x4 mView;
	mat4x4 mProjection;
	mat4x4 mDecalProjection;
	mat4x4 mDecalProjection2;

	mat4x4 mBombProj;

	float FOV;
	float AR;

private:
	vec3 position;
	vec3 target;
};

class cSkybox
{
public:
	void RenderSkybox();

	void SetShaderProgram(cShaderProgram* shaderProgram)
	{
		sp = shaderProgram;
	}

	cMesh skyMesh;
	cTexture skyTexture;

private:
	cShaderProgram* sp;
};

class cBomb
{
public:
	void Init(glm::vec3 position);
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

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;
};