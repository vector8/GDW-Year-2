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
		vertexShader = createShader(vertexSourcePath, GL_VERTEX_SHADER);
		fragmentShader = createShader(fragmentSourcePath, GL_FRAGMENT_SHADER);

		// Shader program
		this->program = glCreateProgram();
		glAttachShader(this->program, vertexShader);
		glAttachShader(this->program, fragmentShader);
		glLinkProgram(this->program);

		GLint success;
		GLchar infoLog[512];

		// Print linking errors if any
		glGetProgramiv(this->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Cache uniform locations
		cacheUniformLocations();
	}

	Shader::Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath, const GLchar* geometrySourcePath)
	{
		vertexShader = createShader(vertexSourcePath, GL_VERTEX_SHADER);
		fragmentShader = createShader(fragmentSourcePath, GL_FRAGMENT_SHADER);
		geometryShader = createShader(geometrySourcePath, GL_GEOMETRY_SHADER);

		// Shader program
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glAttachShader(program, geometryShader);
		glLinkProgram(program);

		GLint success;
		GLchar infoLog[512];

		// Print linking errors if any
		glGetProgramiv(this->program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(this->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Cache uniform locations
		cacheUniformLocations();
	}

	Shader::~Shader()
	{
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDetachShader(program, geometryShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

		vertexShader = 0;
		fragmentShader = 0;
		geometryShader = 0;

		glDeleteProgram(program);
		program = 0;
	}

	GLuint Shader::createShader(const GLchar* sourcePath, GLenum shaderType)
	{
		std::string code;
		std::string typeString, typeStringCaps;

		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			typeString = "Vertex";
			typeStringCaps = "VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			typeString = "Fragment";
			typeStringCaps = "FRAGMENT";
			break;
		case GL_GEOMETRY_SHADER:
			typeString = "Geometry";
			typeStringCaps = "GEOMETRY";
			break;
		default:
			break;
		}

		// 1. Retrieve the source code from sourcePath
		try
		{
			// Open files
			std::ifstream shaderFile(sourcePath);

			if (!shaderFile.good())
			{
				std::cout << typeString << " shader file not found!" << std::endl;
			}

			std::stringstream shaderStream;

			// Read files' buffer contents into streams
			shaderStream << shaderFile.rdbuf();

			// Close file handlers
			shaderFile.close();

			// Convert stream into GLchar array
			code = shaderStream.str();
		}
		catch (std::exception e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* shaderCode = code.c_str();

		// 2. Compile shader
		GLint success;
		GLchar infoLog[512];

		// Vertex shader
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		// Print compile errors if any
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << typeStringCaps << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	void Shader::cacheUniformLocations()
	{
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

		thresholdLoc = glGetUniformLocation(program, "threshold");
		pixelSizeLoc = glGetUniformLocation(program, "uPixelSize");

		sceneLoc = glGetUniformLocation(program, "scene");
		sceneNormalsLoc = glGetUniformLocation(program, "sceneNormals");
		sceneDepthLoc = glGetUniformLocation(program, "sceneDepth");

		bloomLoc = glGetUniformLocation(program, "bloom");

		fogFactorLoc = glGetUniformLocation(program, "fogFactor");

		shadowLocs.worldToShadowMap = glGetUniformLocation(program, "worldToShadowMap");
		shadowLocs.shadowMapDepth = glGetUniformLocation(program, "shadowMapDepth");
		shadowLocs.drawShadow = glGetUniformLocation(program, "drawShadow");
		shadowLocs.shadows = glGetUniformLocation(program, "shadows");

		lightPosLoc = glGetUniformLocation(program, "lightPos");

		blendLoc = glGetUniformLocation(program, "blend");

		validPlacementLoc = glGetUniformLocation(program, "validPlacement");

		texLoc = glGetUniformLocation(program, "tex");
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