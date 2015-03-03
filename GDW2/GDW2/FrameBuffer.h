#pragma once
#include <GL\glew.h>

namespace flopse
{
	class FrameBuffer
	{
	private:
		GLuint fbo = GL_NONE;
		GLuint depthAttachment = GL_NONE;
		GLuint *colorAttachments = nullptr;
		GLenum *buffs = nullptr;

		unsigned int numColorAttachments = 0;

	public:
		FrameBuffer() = delete;
		FrameBuffer(unsigned int numColorAttachments);
		~FrameBuffer();

		void initDepthTexture(unsigned int width, unsigned int height);
		void initColorTexture(unsigned int index, unsigned int width, unsigned int height, GLint internalFormat, GLint filter, GLint wrap);
		bool checkFBO();

		// Clears all OpenGL memory
		void unload();

		// Clears all attached textures
		void clear();
		
		void bind();
		void unbind();

		void moveToBackBuffer(int windowWidth, int windowHeight);
		void moveToBackBuffer(int sourceWidth, int sourceHeight, int destWidth, int destHeight);
		void moveToBackBuffer(int sourceX0, int sourceY0, int sourceX1, int sourceY1, int destX0, int destY0, int destX1, int destY1);

		GLuint getDepthHandle() const;
		GLuint getColorHandle(unsigned int index) const;
	};
}