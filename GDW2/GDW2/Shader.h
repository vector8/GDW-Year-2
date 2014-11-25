#pragma once

#include <GL/glew.h>

namespace flopse
{
	class Shader
	{
	public:
		// Our program ID
		GLuint program;

		// Various uniform locations
		GLint modelLoc;
		GLint normalMatrixLoc;
		GLint viewLoc;
		GLint projectionLoc;
		GLint objectColorLoc;
		GLint lightPosLoc;
		GLint viewPosLoc;

		// Constructor reads and builds our shader
		Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);

		// Use our program
		void use() const;
	};
}