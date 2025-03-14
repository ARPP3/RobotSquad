#pragma once

#include "stdafx.h"

enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class cMesh
{
public:
	cMesh()	{}
	~cMesh(){}

	bool LoadFromFile(const string &file);

	//- Release data from OpenGL (VRAM)
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVerticies() const;

	//OpenGL buffers and objects
	GLuint VBO_Verticies = 0;
	GLuint VBO_UVs = 0;
	GLuint VBO_Normals = 0;
	GLuint VAO = 0;

	unsigned int _NumFaces = 0;
	unsigned int _NumVerticies = 0;

private:
};

class cTexture
{
public:
	bool LoadTexture(string _path, bool _generateMipMaps = false);
	void BindTexture(int i_TextureUnit = 0);

	void SetFiltering(int _magnification, int _minification);

	int GetMinificationFilter();
	int GetMagnificationFilter();
	void SetSamplerParameter(GLenum parameter, GLenum value);

	void ReleaseTexture();

	cTexture();
private:
	int width, height, bpp;
	unsigned int uiTexture;
	unsigned int uiSampler;
	bool mipMapsGenerated;

	int minification, magnification;

	string path;
};