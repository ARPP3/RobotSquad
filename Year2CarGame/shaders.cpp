#include "stdafx.h"

cShader::cShader()
{
	bLoaded = false;
}

bool cShader::LoadShader(std::string sFile, int a_iType)
{
	FILE* fp = fopen(sFile.c_str(), "rt");
	if (!fp)return false;

	// Get all lines from a file

	std::vector<std::string> sLines;
	char sLine[255];
	while (fgets(sLine, 255, fp))sLines.push_back(sLine);
	fclose(fp);

	const char** sProgram = new const char*[sLines.size() + 1];
	for (unsigned int i = 0; i < sLines.size(); i++)
		sProgram[i] = sLines[i].c_str();

	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, sLines.size(), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);
	if (iCompilationStatus == GL_FALSE)
	{
		std::vector<char> infoLog;
		infoLog.resize(512);

		GLint infoLen;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &infoLen);

		glGetShaderInfoLog(uiShader, sizeof(char) * 512, &infoLen, &infoLog[0]);

		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
		return false;
	}

	iType = a_iType;
	bLoaded = true;

	return 1;
}

bool cShader::IsLoaded()
{
	return bLoaded;
}

GLuint cShader::GetShaderID()
{
	return uiShader;
}

void cShader::DeleteShader()
{
	if (!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}

cShaderProgram::cShaderProgram()
{
	bLinked = false;
}

void cShaderProgram::CreateProgram()
{
	uiProgram = glCreateProgram();
}

bool cShaderProgram::AddShaderToProgram(cShader* shShader)
{
	if (!shShader->IsLoaded())return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}

bool cShaderProgram::LinkProgram()
{
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

void cShaderProgram::DeleteProgram()
{
	if (!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

void cShaderProgram::UseProgram()
{
	if (bLinked)
		glUseProgram(uiProgram);
}

GLuint cShaderProgram::GetProgramID()
{
	return uiProgram;
}

// Setting floats

void cShaderProgram::setUniform(std::string sName, float* fValues, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

void cShaderProgram::setUniform(std::string sName, const float fValue)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void cShaderProgram::setUniform(std::string sName, glm::vec2* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

void cShaderProgram::setUniform(std::string sName, const glm::vec2 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void cShaderProgram::setUniform(std::string sName, glm::vec3* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void cShaderProgram::setUniform(std::string sName, const glm::vec3 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void cShaderProgram::setUniform(std::string sName, glm::vec4* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void cShaderProgram::setUniform(std::string sName, const glm::vec4 vVector)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 4x4 matrices

void cShaderProgram::setUniform(std::string sName, glm::mat4* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

void cShaderProgram::setUniform(std::string sName, const glm::mat4 mMatrix)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting integers

void cShaderProgram::setUniform(std::string sName, int* iValues, int iCount)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

void cShaderProgram::setUniform(std::string sName, const int iValue)
{
	int iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}