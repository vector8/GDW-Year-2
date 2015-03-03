#include "FrameBuffer.h"

namespace flopse
{
	FrameBuffer::FrameBuffer(unsigned int numColorAttachments) : numColorAttachments(numColorAttachments)
	{
		glGenFramebuffers(1, &fbo);

		colorAttachments = new GLuint[numColorAttachments];

		// buffs is required as a parameter for glDrawBuffers()
		buffs = new GLenum[numColorAttachments];

		for (int i = 0; i < numColorAttachments; i++)
		{
			buffs[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		unload();
	}

	void FrameBuffer::initDepthTexture(unsigned int width, unsigned int height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create depth texture
		glGenTextures(1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void FrameBuffer::initColorTexture(unsigned int index, unsigned int width, unsigned int height, GLint internalFormat, GLint filter, GLint wrap)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create color texture
		glGenTextures(1, &colorAttachments[index]);
		glBindTexture(GL_TEXTURE_2D, colorAttachments[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		// bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, colorAttachments[index], 0);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	bool FrameBuffer::checkFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			unload();
			return false;
		}

		return true;
	}

	// Clears all OpenGL memory
	void FrameBuffer::unload()
	{
		if (buffs != nullptr)
		{
			delete[] buffs;
			buffs = nullptr;
		}

		if (colorAttachments != nullptr)
		{
			for (int i = 0; i < numColorAttachments; i++)
			{
				glDeleteTextures(1, &colorAttachments[i]);
			}

			delete[] colorAttachments;
			colorAttachments = nullptr;
		}

		if (depthAttachment != GL_NONE)
		{
			glDeleteTextures(1, &depthAttachment);
			depthAttachment = GL_NONE;
		}

		numColorAttachments = 0;

		glDeleteFramebuffers(1, &fbo);
		fbo = GL_NONE;
	}

	// Clears all attached textures
	void FrameBuffer::clear()
	{
		GLbitfield temp = 0;

		if (depthAttachment != GL_NONE)
		{
			temp = temp | GL_DEPTH_BUFFER_BIT;
		}

		if (colorAttachments != nullptr)
		{
			temp = temp | GL_COLOR_BUFFER_BIT;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(temp);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glDrawBuffers(numColorAttachments, buffs);
	}

	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void FrameBuffer::moveToBackBuffer(int windowWidth, int windowHeight)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

		glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void FrameBuffer::moveToBackBuffer(int sourceWidth, int sourceHeight, int destWidth, int destHeight)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

		glBlitFramebuffer(0, 0, sourceWidth, sourceHeight, 0, 0, destWidth, destHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void FrameBuffer::moveToBackBuffer(int sourceX0, int sourceY0, int sourceX1, int sourceY1, int destX0, int destY0, int destX1, int destY1)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

		glBlitFramebuffer(sourceX0, sourceY0, sourceX1, sourceY1, destX0, destY0, destX1, destY1, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	GLuint FrameBuffer::getDepthHandle() const
	{
		return depthAttachment;
	}

	GLuint FrameBuffer::getColorHandle(unsigned int index) const
	{
		return colorAttachments[index];
	}
}