#pragma once

#include "stdafx.h"

struct BulletCar
{
	btRigidBody* chassisRigidBody;
	btCollisionShape* chassisShape;
	btCompoundShape* compound;
	btDefaultMotionState* chassisMotionState;
	btRaycastVehicle::btVehicleTuning tuning;
	btVehicleRaycaster* raycaster;
	btRaycastVehicle* vehicle;

	btScalar chassisMass;
	btVector3 chassisInertia;	

	float gEngineForce = 0.f;
	float gBreakingForce = 0.f;

	float maxEngineForce = 6000.f;
	float maxBreakingForce = 100.f;

	float gVehicleSteering = 0.f;
	float steeringClamp = 0.35f;
};

class cPhysics;

class Car
{
public:
	void SetupBulletCar(cPhysics p, float _mass, glm::vec3 _chassisInertia, glm::vec3 _carDimensions, glm::vec3 _orientation, glm::vec3 _position,
		glm::vec4 _FRWheel, glm::vec4 _FLWheel, glm::vec4 _BRWheel, glm::vec4 _BLWheel);
	
	BulletCar car;

	glm::mat4 GetMatrix()
	{
		float m[16];	btTransform t;
		car.chassisMotionState->getWorldTransform(t);
		t.getOpenGLMatrix(m);

		glm::mat4 returnMat;

		for (int i = 0; i < 16; i++)
		{
			returnMat = glm::mat4
				(
				m[0], m[1], m[2], m[3],
				m[4], m[5], m[6], m[7],
				m[8], m[9], m[10], m[11],
				m[12], m[13], m[14], m[15]
				);
		}
		return returnMat;
	}
};