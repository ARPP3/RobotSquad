#pragma once 

#include "stdafx.h"

class cFrameBuffer
{
public:
	cFrameBuffer() { }
	~cFrameBuffer();

	void InitBuffer(unsigned numColorAttachments);

	void InitDepthTexture(unsigned width, unsigned height, bool msaa = false);
	void InitColorTexture(unsigned index, unsigned width, unsigned height,
		GLint internalFormat, GLint filter, GLint wrap, bool msaa = false);
	bool CheckFBO();

	// Clears all openGL memory
	void Unload();

	// Clears all attached textures
	void Clear();

	void Bind();
	void UnBind();

	void MoveToBackBuffer(int windowWidth, int windowHeight);
	void BlitToBuffer(int windowWidth, int windowHeight, cFrameBuffer *buf, bool isDepth = false);

	GLuint GetDepthHandle() const;
	GLuint GetColorHandle(unsigned index) const;

private:
	GLuint _FBO = GL_NONE;
	GLuint _DepthAttachment = GL_NONE;
	GLuint *_ColorAttachment = nullptr;
	GLenum *_Bufs = nullptr;

	bool _msaa = false;

	unsigned _NumColorAttachments = 0;
};