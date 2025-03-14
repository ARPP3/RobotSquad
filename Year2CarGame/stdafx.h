// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define WIN32_EXTRA_LEAN
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning( disable : 4390; disable : 4244 )
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Useful Things
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>

using namespace std;

#include <gl/glew.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletMultiThreaded/btParallelConstraintSolver.h>
#include <BulletMultiThreaded/SequentialThreadSupport.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/PlatformDefinitions.h>
#include <btRaycastVehicle.h>
#include <LinearMath\btIDebugDraw.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <Xinput.h>

// Some useful defines
#define FOR(q,n) for(unsigned int q=0;q<n;q++)
#define SFOR(q,s,e) for(unsigned int q=s;q<=e;q++)
#define RFOR(q,n) for(unsigned int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(unsigned int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

#define PI 3.141592

extern int WIDTH;
extern int HEIGHT;
extern int MAX_WIDTH;
extern int MAX_HEIGHT;
extern bool WIREFRAME;
extern bool BLOOM_ENABLED;
extern float BLOOM_THRESHOLD;
extern int BLOOM_BLUR_PASSES;
extern bool PHYSICS_DEBUG;
extern bool isFullScreen;
extern int gameState;
extern float framerate_limit;
extern float gameSpeed;
extern float newGameSpeed;

//Things that can't be changed (easily)

#define NSIGHT				TRUE
#define BLOOM_DOWNSCALE		2.0f
#define MSAA_SAMPLES		2
#define SHADOW_RESOLUTION	1024

#define EDGE_OFFSET			64  //Set to 0 for better performance

#define BUFFER_OFFSET(i) ((char *)0 + (i))

#define X_PERCENT(percent) ((percent / 100.f) * (float) MAX_WIDTH)
#define Y_PERCENT(percent) ((percent / 100.f) * (float) MAX_HEIGHT)

#include "opengl_control.h"
#include "opengl_app.h"
#include "media.h"
#include "shaders.h"
#include "debug.h"
#include "framebuffer.h"
#include "cfmod.h"
#include "object.h"
#include "bulletcar.h"
#include "ParticleEffect.h"
#include "Random.h"
#include "mainmenu.h"
#include "pathfinding.h"
#include "player.h"
#include "enemies.h"
#include "levels.h"

extern void InitFullScreenQuad();
extern void DrawFullScreenQuad();
extern glm::mat4x4 ScreenRectangle(int x, int y, int w, int h);
extern float Lerp(float a, float b, float u);
extern glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float u);
extern void SmoothFollow(const float yRot, const float rotDamp, const float heightDamp, const float height, const float targetY, const float distanceFromTarget);
extern float QuatToEulerY(glm::mat4x4 m);
//extern bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2);

extern COpenGLWinApp appMain;
extern cCamera SceneCamera;
extern cLevel *currentLevel;
extern fMod FMod;
extern CXBOXController *Controller;

// TODO: reference additional headers your program requires here