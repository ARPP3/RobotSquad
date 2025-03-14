#include "stdafx.h"

cObject::cObject()
{
	scale = glm::vec3(1.0, 1.0, 1.0);
}

cObject::~cObject()
{
	model.Unload();
}

void cObject::Draw()
{
	shaderProgram->setUniform("modelMatrix", matrix);
	shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));

	texture.BindTexture(0);

	glBindVertexArray(model.VAO);
	glDrawArrays(GL_TRIANGLES, 0, model.GetNumVerticies());
	glBindVertexArray(0);
}

void cObject::UpdateModelMatrix()
{
	//Translate
	matrix = glm::translate(glm::mat4(1.0), position);
	//Rotate
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0, 0.0, 1.0));
	//Scale
	matrix = glm::scale(matrix, scale);
}

void cCamera::UpdateViewMatrix()
{
	mView = glm::lookAt(position, target, glm::vec3(0.0, 1.0, 0.0));
}

void cSkybox::RenderSkybox()
{
	glm::mat4x4 matrix = glm::translate(glm::mat4(1.0), SceneCamera.GetPos());

	sp->setUniform("modelMatrix", matrix);
	sp->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));
	sp->setUniform("ambient", 1.f);

	skyTexture.BindTexture(0);

	glDepthMask(0);
	glBindVertexArray(skyMesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, skyMesh.GetNumVerticies());
	glBindVertexArray(0);
	glDepthMask(1);
}

void cBomb::Init(glm::vec3 position)
{
	btScalar _mass = 200.0;
	btBoxShape *shape = new btBoxShape(btVector3(btScalar(0.3), btScalar(1.1), btScalar(0.3)));
	btVector3 _inertia = btVector3(0.0, 0.0, 0.0);

	_motionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(1.0, 0.0, 0.0), btScalar(90 * (PI / 180.f))), btVector3(position.x, position.y, position.z)));
	shape->calculateLocalInertia(_mass, _inertia);

	body = new btRigidBody(_mass, _motionState, shape, _inertia);
	body->setFriction(5.0f);
	// Be sure to add the chassis of the vehicle into the world as a rigid body
	currentLevel->physics.world->addRigidBody(body);
	currentLevel->physics.bodies.push_back(new bulletObject(0, body));
	body->setUserPointer(currentLevel->physics.bodies[currentLevel->physics.bodies.size() - 1]);
}

void cBomb::Draw()
{
	shaderProgram->setUniform("modelMatrix", matrix);
	shaderProgram->setUniform("normalMatrix", glm::transpose(glm::inverse(matrix)));

	texture.BindTexture(0);

	glBindVertexArray(model.VAO);
	glDrawArrays(GL_TRIANGLES, 0, model.GetNumVerticies());
	glBindVertexArray(0);
}

void cBomb::UpdateModelMatrix()
{
	float m[16];	btTransform t;
	_motionState->getWorldTransform(t);
	t.getOpenGLMatrix(m);

	matrix = glm::mat4x4
		(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]
		);
}