#include "stdafx.h"

void Car::SetupBulletCar(cPhysics p, float _mass, glm::vec3 _chassisInertia, glm::vec3 _carDimensions, glm::vec3 _orientation, glm::vec3 _position,
	glm::vec4 _FRWheel, glm::vec4 _FLWheel, glm::vec4 _BRWheel, glm::vec4 _BLWheel)
{
	car.chassisMass = btScalar(_mass);
	car.chassisInertia = btVector3(_chassisInertia.x, _chassisInertia.y, _chassisInertia.z);
	car.chassisShape = new btBoxShape(btVector3(_carDimensions.x, _carDimensions.y - 0.05f, _carDimensions.z));// -_carDimensions.y));
	//btCylinderShape *chassisShape2 = new btCylinderShape(btVector3(_carDimensions.y - 0.1, _carDimensions.x, _carDimensions.y - 0.1));
	//btCylinderShape *chassisShape3 = new btCylinderShape(btVector3(_carDimensions.y - 0.1, _carDimensions.x, _carDimensions.y - 0.1));
	
	car.compound = new btCompoundShape();
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0, 0.025f, 0));
	car.compound->addChildShape(localTrans, car.chassisShape);

	/*localTrans.setRotation(btQuaternion(btVector3(0, 0, 1), 90 * PI / 180.f));

	localTrans.setOrigin(btVector3(0, 0, _carDimensions.z - (_carDimensions.y / 2.f)));
	car.compound->addChildShape(localTrans, chassisShape2);
	localTrans.setOrigin(btVector3(0, 0, 0 - _carDimensions.z + (_carDimensions.y / 2.f)));
	car.compound->addChildShape(localTrans, chassisShape3);*/

	car.chassisMotionState = new btDefaultMotionState(btTransform(btQuaternion
		(_orientation.x, _orientation.y, _orientation.z, 1.f), btVector3(_position.x, _position.y, _position.z)));
	car.compound->calculateLocalInertia(car.chassisMass, car.chassisInertia);

	btRigidBody::btRigidBodyConstructionInfo chassisRigidBodyCI(car.chassisMass, car.chassisMotionState, car.compound, car.chassisInertia);
	car.chassisRigidBody = new btRigidBody(chassisRigidBodyCI);
	car.chassisRigidBody->setCcdMotionThreshold(_carDimensions.y);
	car.chassisRigidBody->setCcdSweptSphereRadius(0.8f * _carDimensions.y);
	car.chassisRigidBody->setActivationState(DISABLE_DEACTIVATION);
	// Be sure to add the chassis of the vehicle into the world as a rigid body
	p.world->addRigidBody(car.chassisRigidBody);
	p.bodies.push_back(new bulletObject(2, car.chassisRigidBody));
	car.chassisRigidBody->setUserPointer(p.bodies[p.bodies.size() - 1]);
	car.chassisRigidBody->setDamping(0.1f, 0.3f);

	car.raycaster = new btDefaultVehicleRaycaster(p.world);
	car.vehicle = new btRaycastVehicle(car.tuning, car.chassisRigidBody, car.raycaster);
	car.vehicle->setCoordinateSystem(0, 1, 2);

	btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
	btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
	btScalar suspensionRestLength(0.6f);
	// Be sure to attach the wheels not higher than the upper bounds of the hull of the vehicle chassis
	car.vehicle->addWheel(btVector3(_FRWheel.x, _FRWheel.y, _FRWheel.z), wheelDirection, wheelAxis, suspensionRestLength, _FRWheel.w, car.tuning, true);
	car.vehicle->addWheel(btVector3(_FLWheel.x, _FLWheel.y, _FLWheel.z), wheelDirection, wheelAxis, suspensionRestLength, _FLWheel.w, car.tuning, true);
	car.vehicle->addWheel(btVector3(_BRWheel.x, _BRWheel.y, _BRWheel.z), wheelDirection, wheelAxis, suspensionRestLength, _BRWheel.w, car.tuning, false);
	car.vehicle->addWheel(btVector3(_BLWheel.x, _BLWheel.y, _BLWheel.z), wheelDirection, wheelAxis, suspensionRestLength, _BLWheel.w, car.tuning, false);

	p.world->addAction(car.vehicle);
}