#pragma once

#include <GL/glew.h>

namespace flopse
{
	class Shader
	{
	public:
		// Our program ID
		GLuint program;

		// Shader IDs
		GLuint vertexShader, fragmentShader;

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
		~Shader();

		// Use our program
		void use() const;
	};
}