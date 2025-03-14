#include "stdafx.h"

struct bulletObject
{
	bulletObject(int i, btRigidBody* b) : id(i), body(b), hit(false) {}
	int id;
	bool hit;
	btRigidBody* body;
};

class cPhysics
{
public:
	void InitWorld()
	{
		//collisionConfig = new btDefaultCollisionConfiguration(); // Default is good
		//dispatcher = new btCollisionDispatcher(collisionConfig);
		//broadphase = new btDbvtBroadphase(); // Split into a grid search
		//solver = new btSequentialImpulseConstraintSolver();
		//world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

		collisionConfig = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		btVector3 worldMin(-1000, -50, -1000);
		btVector3 worldMax(1000, 50, 1000);
		broadphase = new btAxisSweep3(worldMin, worldMax);
		//broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
		world->getDispatchInfo().m_enableSPU = true;

		btContactSolverInfo& info = world->getSolverInfo();
		//info.m_numIterations = 10;

		world->setGravity(btVector3(0, -10, 0));

		world->setDebugDrawer(&gDebugDrawer);

		gDebugDrawer.setDebugMode(gDebugDrawer.getDebugMode()/* | btIDebugDraw::DBG_DrawNormals*/ | btIDebugDraw::DBG_DrawWireframe);

		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, -1, 0, 1), btVector3(0, -1, 0)));

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

		world->addRigidBody(groundRigidBody);
		bodies.push_back(new bulletObject(0, groundRigidBody));
		groundRigidBody->setUserPointer(bodies[bodies.size() - 1]);
	}

	bool LoadFromFile(const string &file)
	{

		FILE *physicsFile = nullptr;
		physicsFile = fopen(file.c_str(), "rb");

		if (physicsFile == nullptr)
		{
			HWND* hWnd = appMain.oglControl.get_hWnd();
			wchar_t errorMessage;
			wsprintf(&errorMessage, L"Cannot open file for reading: %s", file);
			MessageBox(*hWnd, &errorMessage, L"File Error", MB_ICONERROR);
			return false;
		}

		glm::vec3 p;
		glm::vec3 r;
		glm::vec3 s;

		while (fscanf(physicsFile, "%f %f %f %f %f %f %f %f %f", &p.x, &p.y, &p.z, &r.x, &r.y, &r.z, &s.x, &s.y, &s.z) != EOF)
		{
			btBoxShape *shape = new btBoxShape(btVector3(s.x / 2.f, s.y / 2.f, s.z / 2.f));
			btRigidBody *body;

			btTransform t;			
			t.setOrigin(btVector3(btScalar(-1.f * p.x), btScalar(p.y), btScalar(p.z)));

			t.setRotation(btQuaternion(btVector3(1.0, 0.0, 0.0), btScalar(r.x * (PI / 180.f))));
			t.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), btScalar(r.y * (PI / 180.f))) * t.getRotation());
			t.setRotation(btQuaternion(btVector3(0.0, 0.0, 1.0), btScalar(r.z * (PI / 180.f))) * t.getRotation());
			
			body = new btRigidBody(0, 0, shape);
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
			body->setWorldTransform(t);
			// Be sure to add the chassis of the vehicle into the world as a rigid body
			world->addRigidBody(body);
			bodies.push_back(new bulletObject(1, body));
			body->setUserPointer(bodies[bodies.size() - 1]);
		}

		fclose(physicsFile);

		return true;
	}

	bool Unload()
	{
		//cleanup in the reverse order of creation/initialization

		//delete dynamics world
		delete world;

		//delete solver
		delete solver;

		//delete broadphase
		delete broadphase;

		//delete dispatcher
		delete dispatcher;

		//delete collision algorithms creation functions
		delete collisionConfig;

		return true;
	}

	btDynamicsWorld* world; // The world (move into Game Class later)
	std::vector<bulletObject*> bodies; // A list of rigidbodies

	//Bullet Physics
	btDispatcher* dispatcher; // Collision algorithm between things (see page 21 of manual)
	btBroadphaseInterface* broadphase; // This is how physics is calculated I think
	btConstraintSolver* solver; // How the world is solved
	btCollisionConfiguration* collisionConfig;

	GLDebugDrawer gDebugDrawer;
};

class cLevel
{
public:
	cLevel()
	{
	}
	~cLevel()
	{
	}

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	cShaderProgram basicShaderProgram;
	cShaderProgram toonProgram;
	cShaderProgram passThrough;

	cShaderProgram billboard;
	cShaderProgram bloomHighPass;
	cShaderProgram blurHorizontal;
	cShaderProgram blurVertical;
	cShaderProgram bloomComposite;

	ParticleEffect sparks;

	Player player;
	cPhysics physics;

	float timer;

	cBomb tnt[20];
	int levelSize;

	FMOD::ChannelGroup *Channels;
private:
};

class BlowUp : public cLevel
{
public:
	BlowUp()
	{
	}
	~BlowUp()
	{
	}

	virtual void Init();
	virtual void Release();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Draw();

	cSkybox sky;
	cPlanet	planet[10];
	
	FMOD::Sound *backgroundAudio;
	FMOD::Channel *backgroundChannel;

	cFrameBuffer mainBuffer;
	cFrameBuffer workBuffer1, workBuffer2;
	cFrameBuffer msaaBuffer;

	ParticleEffect explosion;
	glm::vec3 centers[10];

	cTexture endMessage;
	cTexture mouse;
private:
};

class Tutorial1 : public cLevel
{
public:
	Tutorial1()
	{
		for (int i = 0; i < 12; i++)
		{
			voicePlayed[i] = 0;
		}
	}
	~Tutorial1()
	{
	}

	virtual void Init();
	virtual void Release();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Draw();

	cSkybox sky;
	cObject walls;

	cFrameBuffer mainBuffer;
	cFrameBuffer workBuffer1, workBuffer2;
	cFrameBuffer msaaBuffer;
	cFrameBuffer shadowMap;

	cTexture toonRamp;

	glm::mat4x4 shadowTransform;
	glm::mat4x4 shadowProjection;
	glm::mat4x4 viewToShadowMap;

	FMOD::Sound *backgroundAudio;
	FMOD::Channel *backgroundChannel;

	FMOD::Sound *voice[12];
	FMOD::Channel *voiceChannel;

	ParticleEffect ringEffect;

	int voicePlayed[12];

	int numBombs = 2;
private:
};


class Level1 : public cLevel
{
public:
	Level1()
	{
	}
	~Level1()
	{
	}

	virtual void Init();
	virtual void Release();

	virtual void HandleEvents();
	virtual void Update();
	virtual void Draw();

	cSkybox sky;
	cObject brick;			//brick.obj
	cObject door;			//doors.obj
	cObject walls;			//walls.obj
	cObject floor;			//concrete.obj
	cObject moss;			//grime.obj
	cObject vent;			//crates.obj
	cObject puddle;			//puddles.obj
	cObject fire;			//fire.obj
	cObject oil;			//oil.obj
	cObject orangefence;	//orangefence.obj
	cObject forklift;		//forklift.obj
	cObject ring;
	cObject tree[2];
	cHelicopter helicopter;

	cFrameBuffer mainBuffer;
	cFrameBuffer workBuffer1, workBuffer2;
	cFrameBuffer msaaBuffer;
	cFrameBuffer shadowMap;

	cTexture toonRamp;

	glm::mat4x4 shadowTransform;
	glm::mat4x4 shadowProjection;
	glm::mat4x4 viewToShadowMap;

	FMOD::Sound *backgroundAudio;
	FMOD::Channel *backgroundChannel;

	cTexture black;
	ParticleEffect ringEffect;

	cBoxDynamic boxes;

	int numBombs = 3;
private:
};