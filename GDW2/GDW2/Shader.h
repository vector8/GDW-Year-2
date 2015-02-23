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
		GLint viewLoc;
		GLint projectionLoc;
		// GLint objectColorLoc;
		GLint lightPosLoc;
		GLint viewPosLoc;
		GLint lightColourLoc;
		GLint ambientLoc;
		GLint diffuseLoc;
		GLint specularLoc;
		GLint specularExponentLoc;
		GLint constantAttenuationLoc;
		GLint linearAttenuationLoc;
		GLint quadraticAttenuationLoc;
		GLint thresholdLoc;
		GLint pixelSizeLoc;
		GLint sceneLoc;
		GLint bloomLoc;

		// Constructor reads and builds our shader
		Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
		virtual ~Shader();

		// Use our program
		void bind() const;

		void unbind() const;
	};
}