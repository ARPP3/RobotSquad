#pragma once

#include "stdafx.h"

class mainMenu
{
public:
	mainMenu(){}
	~mainMenu(){}

	void Init();
	void Update();
	void Draw();
	
	cTexture background;
	cTexture levelSelectTex[2];
	cTexture controlsTex[2];
	cTexture creditTex;
	cTexture mouse;

	cObject robot;

	cShaderProgram passThrough;
	cShaderProgram basicShaderProgram;

	FMOD::Sound *backgroundAudio;
	
	int levelSelect = -1;
	int controlsSelect = -1;
	int menuState = 0;

	int positionY = 0;
private:

};