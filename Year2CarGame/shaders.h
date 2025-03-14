#pragma once
#include "stdafx.h"

class cShader
{
public:
	bool LoadShader(std::string sFile, int a_iType);
	void DeleteShader();

	bool IsLoaded();
	GLuint GetShaderID();

	cShader();

private:
	GLuint uiShader; // ID of shader
	int iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded; // Whether shader was loaded and compiled
};

class cShaderProgram
{
public:
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(cShader* shShader);
	bool LinkProgram();

	void UseProgram();

	GLuint GetProgramID();

	// Setting vectors
	void setUniform(std::string sName, glm::vec2* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec2 vVector);
	void setUniform(std::string sName, glm::vec3* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec3 vVector);
	void setUniform(std::string sName, glm::vec4* vVectors, int iCount = 1);
	void setUniform(std::string sName, const glm::vec4 vVector);

	// Setting floats
	void setUniform(std::string sName, float* fValues, int iCount = 1);
	void setUniform(std::string sName, const float fValue);

	// Setting 4x4 matrices
	void setUniform(std::string sName, glm::mat4* mMatrices, int iCount = 1);
	void setUniform(std::string sName, const glm::mat4 mMatrix);

	// Setting integers
	void setUniform(std::string sName, int* iValues, int iCount = 1);
	void setUniform(std::string sName, const int iValue);

	cShaderProgram();

	cShader vert, frag, geom;

private:
	GLuint uiProgram; // ID of program
	bool bLinked; // Whether program was linked and is ready to use
};