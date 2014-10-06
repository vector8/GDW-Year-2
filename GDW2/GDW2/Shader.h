#pragma once

#include <GL/glew.h>

class Shader
{
public:
	// Our program ID
	GLuint program;

	// Constructor reads and builds our shader
	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);

	// Use our program
	void use();
};