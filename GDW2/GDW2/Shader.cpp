#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace flopse
{
	// Constructor reads and builds our shader
	Shader::Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode, fragmentCode;

		try
		{
			// Open files
			std::ifstream vShaderFile(vertexSourcePath), fShaderFile(fragmentSourcePath);

			std::stringstream vShaderStream, fShaderStream;

			// Read files' buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();

			// Convert stream into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::exception e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Repeat for fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Shader program
		this->program = glCreateProgram();
		glAttachShader(this->program, vertex);
		glAttachShader(this->program, fragment);
		glLinkProgram(this->program);

		// Print linking errors if any
		glGetProgramiv(this->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		// Cache uniform locs
		modelLoc = glGetUniformLocation(program, "model");
		normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");
		viewLoc = glGetUniformLocation(program, "view");
		projectionLoc = glGetUniformLocation(program, "projection");
		objectColorLoc = glGetUniformLocation(program, "objectColor");
		lightPosLoc = glGetUniformLocation(program, "lightPos");
		viewPosLoc = glGetUniformLocation(program, "viewPos");
	}

	// Use our program
	void Shader::use() const
	{
		glUseProgram(this->program);
	}
}