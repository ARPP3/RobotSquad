#pragma once

#include "stdafx.h"

class fMod
{
public:
	fMod(){}
	~fMod(){}

	int Startup();
	void CheckError(FMOD_RESULT err);
	void CreateSound(char* fileName);
	void ListenerPosition(FMOD_VECTOR listenerPos);

	FMOD::System *system;
	FMOD::Sound *audio;
	FMOD::Channel *channel;

protected:
private:
};