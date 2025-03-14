#include "stdafx.h"

GLDebugDrawer::GLDebugDrawer()
	:m_debugMode(0)
{
}

GLDebugDrawer::~GLDebugDrawer()
{
}

void GLDebugDrawer::Init()
{
	
}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	GLuint VAO = GL_NONE;
	GLuint VBO = GL_NONE;

	float VBO_DATA[] =
	{
		from.x(), from.y(), from.z(),
		to.x(), to.y(), to.z(),

		0.0f, 0.0f,
		0.0f, 0.0f,

		fromColor.x(), fromColor.y(), fromColor.z(),
		toColor.x(), toColor.y(), toColor.z()
	};

	int vertexSize = 2 * 3 * sizeof(float);
	int uvSize = 2 * 2 * sizeof(float);
	int normalSize = 2 * 3 * sizeof(float);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); //vertices
	glEnableVertexAttribArray(1); //UV coordinates
	glEnableVertexAttribArray(2); //normal coordinates

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize + uvSize + normalSize, VBO_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize));
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexSize + uvSize));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	sp->UseProgram();

	sp->setUniform("viewMatrix", SceneCamera.mView);
	sp->setUniform("projectionMatrix", SceneCamera.mProjection);
	sp->setUniform("modelMatrix", glm::mat4x4(1.0));
	sp->setUniform("normalMatrix", glm::mat4x4(1.0));

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(GL_NONE);
}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	drawLine(from, to, color, color);
}

void GLDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
{
}



void	GLDebugDrawer::drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
{
}

void	GLDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void	GLDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void	GLDebugDrawer::reportErrorWarning(const char* warningString)
{
	printf("%s\n", warningString);
}

void	GLDebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

	{
		btVector3 to = pointOnB + normalOnB * 1;//distance;
		const btVector3&from = pointOnB;

		drawLine(from, to, color);
	}
}