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

			if (!vShaderFile.good())
			{
				std::cout << "Vertex shader file not found!" << std::endl;
			}

			if (!fShaderFile.good())
			{
				std::cout << "Fragment shader file not found!" << std::endl;
			}

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

		// Cache uniform locations
		modelLoc = glGetUniformLocation(program, "model");
		viewLoc = glGetUniformLocation(program, "view");
		projectionLoc = glGetUniformLocation(program, "projection");
		objectColorLoc = glGetUniformLocation(program, "objectColor");
		viewPosLoc = glGetUniformLocation(program, "viewPos");

		materialLocs.diffuse = glGetUniformLocation(program, "material.diffuse");
		materialLocs.specular = glGetUniformLocation(program, "material.specular");
		materialLocs.specularExponent = glGetUniformLocation(program, "material.specularExponent");

		for (int i = 0; i < NUM_POINT_LIGHTS; i++)
		{
			std::string prefix = "pointLights[" + std::to_string(i) + "].";
			pointLightLocs[i].position = glGetUniformLocation(program, std::string(prefix + "position").c_str());
			pointLightLocs[i].ambient = glGetUniformLocation(program, std::string(prefix + "ambient").c_str());
			pointLightLocs[i].diffuse = glGetUniformLocation(program, std::string(prefix + "diffuse").c_str());
			pointLightLocs[i].specular = glGetUniformLocation(program, std::string(prefix + "specular").c_str());
			pointLightLocs[i].constantAttenuation = glGetUniformLocation(program, std::string(prefix + "constantAttenuation").c_str());
			pointLightLocs[i].linearAttenuation = glGetUniformLocation(program, std::string(prefix + "linearAttenuation").c_str());
			pointLightLocs[i].quadraticAttenuation = glGetUniformLocation(program, std::string(prefix + "quadraticAttenuation").c_str());
		}

		directionalLightLocs.direction = glGetUniformLocation(program, "dirLight.direction");
		directionalLightLocs.ambient = glGetUniformLocation(program, "dirLight.ambient");
		directionalLightLocs.diffuse = glGetUniformLocation(program, "dirLight.diffuse");
		directionalLightLocs.specular = glGetUniformLocation(program, "dirLight.specular");

		thresholdLoc = glGetUniformLocation(program, "uThreshold");
		pixelSizeLoc = glGetUniformLocation(program, "uPixelSize");
		sceneLoc = glGetUniformLocation(program, "scene");
		bloomLoc = glGetUniformLocation(program, "bloom");
		
		fogFactorLoc = glGetUniformLocation(program, "fogFactor");

		shadowLocs.worldToShadowMap = glGetUniformLocation(program, "worldToShadowMap");
		shadowLocs.shadowMapDepth = glGetUniformLocation(program, "shadowMapDepth");
		shadowLocs.drawShadow = glGetUniformLocation(program, "drawShadow");
		shadowLocs.shadows = glGetUniformLocation(program, "shadows");

		lightPosLoc = glGetUniformLocation(program, "lightPos");

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