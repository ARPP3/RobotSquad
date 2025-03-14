#include "stdafx.h"

#define CHAR_BUFFER_SIZE 128

bool cMesh::LoadFromFile(const string &file)
{

	FILE *binaryFile = fopen(file.c_str(), "rb");

	if (binaryFile == nullptr)
	{
		HWND* hWnd = appMain.oglControl.get_hWnd();
		wchar_t errorMessage;
		wsprintf(&errorMessage, L"Cannot open file for reading: %s", file);
		MessageBox(*hWnd, &errorMessage, L"File Error", MB_ICONERROR);
		return false;
	}

	fread(&_NumFaces, sizeof(unsigned int), 1, binaryFile);
	fread(&_NumVerticies, sizeof(unsigned int), 1, binaryFile);

	float *verticies = new float[_NumVerticies * 3];
	float *UVs = new float[_NumVerticies * 2];
	float *normals = new float[_NumVerticies * 3];

	fread(verticies, sizeof(float), _NumVerticies * 3, binaryFile);
	fread(UVs, sizeof(float), _NumVerticies * 2, binaryFile);
	fread(normals, sizeof(float), _NumVerticies * 3, binaryFile);

	fclose(binaryFile);

	//Send data to OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Verticies);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0); //Vertex
	glEnableVertexAttribArray(1); //UVs
	glEnableVertexAttribArray(2); //Normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _NumVerticies * 3, verticies, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _NumVerticies * 2, UVs, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _NumVerticies * 3, normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete[] verticies;
	delete[] UVs;
	delete[] normals;

	return true;
}

unsigned int cMesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int cMesh::GetNumVerticies() const
{
	return _NumVerticies;
}

//- Release data from OpenGL (VRAM)
void cMesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Verticies);
	glDeleteVertexArrays(1, &VAO);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Verticies = 0;
	VAO = 0;

	_NumFaces = 0;
	_NumVerticies = 0;
}


cTexture::cTexture()
{
	mipMapsGenerated = false;
}

bool cTexture::LoadTexture(string _path, bool _generateMipMaps)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(_path.c_str(), 0); // Check the file signature and deduce its format

	if (fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(_path.c_str());

	if (fif == FIF_UNKNOWN) // If still unkown, return failure
		return false;

	if (FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, _path.c_str());
	if (!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	width = FreeImage_GetWidth(dib); // Get the image width and height
	height = FreeImage_GetHeight(dib);
	bpp = FreeImage_GetBPP(dib);

	// If somehow one of these failed (they shouldn't), return failure
	if (bDataPointer == NULL || width == 0 || height == 0)
		return false;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	int iFormat = bpp == 32 ? GL_BGRA : bpp == 24 ? GL_BGR : bpp == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = bpp == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(GL_TEXTURE_2D, 0, bpp == 32 ? GL_RGBA8 : bpp == 24 ? GL_RGB8 : 0,
		width, height, 0, iFormat, GL_UNSIGNED_BYTE, bDataPointer);

	if (_generateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(dib);

	glBindTexture(GL_TEXTURE_2D, GL_NONE);

#if NSIGHT
	glGenSamplers(1, &uiSampler);
#endif

	path = _path;
	mipMapsGenerated = _generateMipMaps;

	return true; // Success
}

#if NSIGHT
void cTexture::SetFiltering(int _magnification, int _minification)
{
	// Set magnification filter
	if (_magnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (_magnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (_minification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (_minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	minification = _minification;
	magnification = _magnification;

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if (mipMapsGenerated)
	{
		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
#else
void cTexture::SetFiltering(int _magnification, int _minification)
{
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	// Set magnification filter
	if (_magnification == TEXTURE_FILTER_MAG_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if (_magnification == TEXTURE_FILTER_MAG_BILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if (_minification == TEXTURE_FILTER_MIN_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_BILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if (_minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if (_minification == TEXTURE_FILTER_MIN_TRILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	minification = _minification;
	magnification = _magnification;

	if (mipMapsGenerated)
	{
		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}

	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}
#endif

void cTexture::BindTexture(int i_TextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + i_TextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

#if NSIGHT
	glBindSampler(i_TextureUnit, uiSampler);
#endif
}

void cTexture::ReleaseTexture()
{
#if NSIGHT
	glDeleteSamplers(1, &uiSampler);
#endif

	glDeleteTextures(1, &uiTexture);
}

int cTexture::GetMinificationFilter()
{
	return minification;
}

int cTexture::GetMagnificationFilter()
{
	return magnification;
}

void cTexture::SetSamplerParameter(GLenum parameter, GLenum value)
{
#if NSIGHT
	glSamplerParameteri(uiSampler, parameter, value);
#else
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexParameteri(GL_TEXTURE_2D, parameter, value);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
#endif
}