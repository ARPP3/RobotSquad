#include "stdafx.h"

void cFrameBuffer::InitBuffer(unsigned numColorAttachments)
{
	_NumColorAttachments = numColorAttachments;

	glGenFramebuffers(1, &_FBO);

	_ColorAttachment = new GLuint[_NumColorAttachments];

	// _Bufs is required as a parameter for glDrawBuffers()
	_Bufs = new GLenum[_NumColorAttachments];
	FOR(i, _NumColorAttachments)
	{
		_Bufs[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

cFrameBuffer::~cFrameBuffer()
{
	Unload();
}

void cFrameBuffer::InitDepthTexture(unsigned width, unsigned height, bool msaa)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (!msaa)
	{
		//create depth texture
		glGenTextures(1, &_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);
	}
	else
	{
		_msaa = msaa;
		glGenTextures(1, &_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _DepthAttachment);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_DEPTH_COMPONENT24, width, height, false);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, _DepthAttachment, 0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_NONE);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void cFrameBuffer::InitColorTexture(unsigned index, unsigned width, unsigned height,
	GLint internalFormat, GLint filter, GLint wrap, bool msaa)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	if (!msaa)
	{
		//create color texture
		glGenTextures(1, &_ColorAttachment[index]);
		glBindTexture(GL_TEXTURE_2D, _ColorAttachment[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		//bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, _ColorAttachment[index], 0);
	}
	else
	{
		_msaa = msaa;
		glGenTextures(1, &_ColorAttachment[0]);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _ColorAttachment[0]);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_RGBA8, width, height, false);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _ColorAttachment[0], 0);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

bool cFrameBuffer::CheckFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Unload();
		return false;
	}

	return true;
}

// Clears all openGL memory
void cFrameBuffer::Unload()
{
	if (_Bufs != nullptr)
	{
		delete[] _Bufs;
		_Bufs = nullptr;
	}

	if (_ColorAttachment != nullptr)
	{
		FOR(i, _NumColorAttachments)
		{
			glDeleteTextures(1, &_ColorAttachment[i]);
		}

		delete[] _ColorAttachment;
		_ColorAttachment = nullptr;
	}

	if (_DepthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &_DepthAttachment);
		_DepthAttachment = GL_NONE;
	}

	_NumColorAttachments = 0;
}

// Clears all attached textures
void cFrameBuffer::Clear()
{
	GLbitfield temp = 0;

	if (_DepthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;
	}

	if (_ColorAttachment != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void cFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDrawBuffers(_NumColorAttachments, _Bufs);
}

void cFrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void cFrameBuffer::MoveToBackBuffer(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, WIDTH,
			HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void cFrameBuffer::BlitToBuffer(int windowWidth, int windowHeight, cFrameBuffer *buf, bool isDepth)
{
	if (!isDepth) //Blit color buffer
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buf->_FBO);

		glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth,
			windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}
	else if (isDepth) //Blit depth buffer
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buf->_FBO);

		glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth,
			windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}
}

GLuint cFrameBuffer::GetDepthHandle() const
{
	return _DepthAttachment;
}
GLuint cFrameBuffer::GetColorHandle(unsigned index) const
{
	return _ColorAttachment[index];
}