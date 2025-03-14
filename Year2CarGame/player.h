#pragma once

#include "stdafx.h"

class Player
{
public:
	Player(){}
	~Player(){}

	void Init();
	void HandleEvents();
	void Update();
	void Draw();

	void DrawHUD();

	void SetShaderProgram(cShaderProgram* shaderProgram)
	{
		sp = shaderProgram;
	}

	cPhysics* physics;

	Car playerRobot;
	cMesh robotMesh;
	cMesh wheelMesh[4];
	cTexture robotTexture;

	//GamePlay Stuff
	int numBombs;
	int numBombsLeft;
	int haveBomb = -1;

	//HUD Stuff
	cTexture spaceMessage;
	cTexture moveMessage;
	cTexture tutorialMessage;
	cTexture levelMessage;
	cTexture tutorialReturn;
	cTexture winMessage;
	cTexture loseMessage;
	cTexture mouse;
	cTexture hud;
	cTexture digitalNumbers[11];
	cTexture cartoonNumbers[16];

	bool showSpaceMessage = false;
	bool showTutorialMessage = false;
	bool showLevelMessage = false;
	bool showTutorialReturn = false;
	bool showMoveHelp = false;
	bool isTutorial = false;
	
	bool isPickedUp = false;

	float winTime;

	cShaderProgram* sp;
	ParticleEffect smokeEffect;

	glm::mat4x4 bombMat;
	glm::mat4x4 matrix;

	//Orbital Cam
	float accumRot, mouseRot;

	//Sounds
	FMOD::Sound *pickUpSound, *dropDownSound, *engineLoop, *crash;
	FMOD::Channel *clawChannel, *engineChannel;
};