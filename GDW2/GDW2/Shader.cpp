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
		GLint success;
		GLchar infoLog[512];

		// Vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		// Print compile errors if any
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Repeat for fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		// Print compile errors if any
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Shader program
		this->program = glCreateProgram();
		glAttachShader(this->program, vertexShader);
		glAttachShader(this->program, fragmentShader);
		glLinkProgram(this->program);

		// Print linking errors if any
		glGetProgramiv(this->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Cache uniform locs
		modelLoc = glGetUniformLocation(program, "model");
		viewLoc = glGetUniformLocation(program, "view");
		projectionLoc = glGetUniformLocation(program, "projection");
		//objectColorLoc = glGetUniformLocation(program, "objectColor");
		lightPosLoc = glGetUniformLocation(program, "lightPos");
		viewPosLoc = glGetUniformLocation(program, "viewPos");
		lightColourLoc = glGetUniformLocation(program, "lightColour");
		ambientLoc = glGetUniformLocation(program, "ambient");
		diffuseLoc = glGetUniformLocation(program, "diffuse");
		specularLoc = glGetUniformLocation(program, "specular");
		specularExponentLoc = glGetUniformLocation(program, "specularExponent");
		constantAttenuationLoc = glGetUniformLocation(program, "constantAttenuation");
		linearAttenuationLoc = glGetUniformLocation(program, "linearAttenuation");
		quadraticAttenuationLoc = glGetUniformLocation(program, "quadraticAttenuation");
		thresholdLoc = glGetUniformLocation(program, "uThreshold");
		pixelSizeLoc = glGetUniformLocation(program, "uPixelSize");
		sceneLoc = glGetUniformLocation(program, "uScene");
		bloomLoc = glGetUniformLocation(program, "uBloom");
	}

	Shader::~Shader()
	{
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertexShader = 0;
		fragmentShader = 0;

		glDeleteProgram(program);
		program = 0;
	}

	// Use our program
	void Shader::bind() const
	{
		glUseProgram(this->program);
	}

	// Unbind the program
	void Shader::unbind() const
	{
		glUseProgram(GL_NONE);
	}
}