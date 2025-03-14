#include "stdafx.h"

int fMod::Startup()
{
	FMOD_RESULT result;
	unsigned int version;
	int numDrivers;
	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS caps;

	/* Create the system */
	CheckError(FMOD::System_Create(&system));

	/* Check version */
	CheckError(system->getVersion(&version));

	if (version < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %i. This program requires %i\n", version, FMOD_VERSION);
		MessageBox(appMain.hWnd, L"You are using an old version of FMOD", L"FMOD ERROR", MB_ICONERROR);
	}

	CheckError(system->getNumDrivers(&numDrivers));

	/* No sound cards (disable sound) */
	if (numDrivers == 0)
	{
		CheckError(system->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
	}

	/* If there are any sound cards */
	else
	{
		CheckError(system->getDriverCaps(0, &caps, 0, &speakerMode));
		CheckError(system->setSpeakerMode(speakerMode));

		/* Increase buffer size if Acceleration slider set to off */
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			CheckError(system->setDSPBufferSize(1024, 10));
		}
	}

	/* Initialize */
	result = system->init(100, FMOD_INIT_NORMAL, 0);

	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		CheckError(system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));
		CheckError(system->init(100, FMOD_INIT_NORMAL, 0));
	}

	CheckError(system->set3DSettings(1.0, 1.0, 1.0f));

	return 0;
}

void fMod::CheckError(FMOD_RESULT err)
{
	if (err != FMOD_OK)
	{
		printf("FMOD error! (%i) %s\n", err, FMOD_ErrorString(err));
		return;
	}
}

void fMod::CreateSound(char* fileName)
{
	CheckError(system->createSound(fileName, FMOD_3D, 0, &audio));
	CheckError(audio->set3DMinMaxDistance(0.5f, 75.f));
	audio->setMode(FMOD_LOOP_NORMAL);
	//channel->setLoopCount(-1);
	CheckError(system->playSound(FMOD_CHANNEL_FREE, audio, false, &channel));
}

void fMod::ListenerPosition(FMOD_VECTOR listenerPos)
{
	FMOD_VECTOR forward = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.f, 0.f, 0.f };

	CheckError(system->set3DListenerAttributes(0, &listenerPos, &vel, &forward, &up));
}