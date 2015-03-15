#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include "Utilities.h"

namespace flopse
{
	Mesh::Mesh(GLfloat *vertexData, int numVertices, std::shared_ptr<Shader> s, const std::string &diffuseMapFilename, const std::string &specularMapFilename, bool useUVs, bool useNormals, bool useColour) :
		vertexData(vertexData), numVertices(numVertices), shader(s),
		useUVs(useUVs), useNormals(useNormals), useColour(useColour)
	{
		this->initArrays(vertexData, numVertices, useUVs, useNormals);

		calculateDimensions(vertexData, numVertices);

		if (diffuseMapFilename.size() > 0)
		{
			setDiffuseMap(diffuseMapFilename);
		}

		if (specularMapFilename.size() > 0)
		{
			setSpecularMap(specularMapFilename);
		}
	}

	Mesh::Mesh(const std::string &fileName, std::shared_ptr<Shader> s, const std::string &diffuseMapFilename, const std::string &specularMapFilename) :
		shader(s), width(0.f), height(0.f), depth(0.f), useUVs(true), useNormals(true), useColour(false)
	{
		if (this->loadFromFile(fileName))
		{
			this->initArrays(&objData[0], objData.size() / 8);

			calculateDimensions(&objData[0], objData.size() / 8);

			if (diffuseMapFilename.size() > 0)
			{
				setDiffuseMap(diffuseMapFilename);
			}

			if (specularMapFilename.size() > 0)
			{
				setSpecularMap(specularMapFilename);
			}
		}
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::refreshArrays()
	{
		this->initArrays(&objData[0], objData.size() / 8);

		calculateDimensions(&objData[0], objData.size() / 8);
	}

	void Mesh::initArrays(GLfloat *vertexData, int numVertices, bool useUVs, bool useNormals, bool useColour)
	{
		int numberElements = 3;
		int position = 3;

		if (useUVs)
		{
			numberElements += 2;
		}
		if (useNormals)
		{
			numberElements += 3;
		}
		if (useColour)
		{
			numberElements += 4;
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numberElements * numVertices * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

		// position
		GLuint loc = glGetAttribLocation(shader->program, "position");
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, numberElements * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(loc);

		if (useUVs)
		{
			// uv
			loc = glGetAttribLocation(shader->program, "uv");
			glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, numberElements * sizeof(GLfloat), (GLvoid*)(position * sizeof(GLfloat)));
			glEnableVertexAttribArray(loc);
			position += 2;
		}

		if (useNormals)
		{
			// normal
			loc = glGetAttribLocation(shader->program, "normal");
			glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, numberElements * sizeof(GLfloat), (GLvoid*)(position * sizeof(GLfloat)));
			glEnableVertexAttribArray(loc);
			position += 3;
		}

		if (useColour)
		{
			// colour
			loc = glGetAttribLocation(shader->program, "color");
			glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, numberElements * sizeof(GLfloat), (GLvoid*)(position * sizeof(GLfloat)));
			glEnableVertexAttribArray(loc);
			position += 4;
		}

		glBindVertexArray(0);
	}

	void Mesh::calculateDimensions(GLfloat *vertexData, int numVertices)
	{
		float maxY = 0.f, minY = 0.f, maxX = 0.f, minX = 0.f, maxZ = 0.f, minZ = 0.f;

		for (int i = 0; i < numVertices; i++)
		{
			int index = i * 8;

			if (vertexData[index] > maxX)
			{
				maxX = vertexData[index];
			}
			else if (vertexData[index] < minX)
			{
				minX = vertexData[index];
			}

			if (vertexData[index + 1] > maxY)
			{
				maxY = vertexData[index + 1];
			}
			else if (vertexData[index + 1] < minY)
			{
				minY = vertexData[index + 1];
			}

			if (vertexData[index + 2] > maxZ)
			{
				maxZ = vertexData[index + 2];
			}
			else if (vertexData[index + 2] < minZ)
			{
				minZ = vertexData[index + 2];
			}
		}

		width = maxX - minX;
		height = maxY - minY;
		depth = maxZ - minZ;
	}

	void Mesh::setDiffuseMap(const std::string &filename)
	{
		this->diffuseMap = std::make_shared<sf::Texture>();
		if (!this->diffuseMap->loadFromFile(filename))
		{
			std::cout << "!!!!ERROR LOADING DIFFUSE MAP!!!!" << std::endl;
		}
	}

	void Mesh::setDiffuseMap(std::shared_ptr<sf::Texture> t)
	{
		this->diffuseMap = t;
	}

	void Mesh::setSpecularMap(const std::string &filename)
	{
		this->specularMap = std::make_shared<sf::Texture>();
		if (!this->specularMap->loadFromFile(filename))
		{
			std::cout << "!!!!ERROR LOADING SPECULAR MAP!!!!" << std::endl;
		}
	}

	void Mesh::setSpecularMap(std::shared_ptr<sf::Texture> t)
	{
		this->specularMap = t;
	}

	void Mesh::setPointColour(const Colour &c)
	{
		if (useColour)
		{
			int position = 3;
			int numElements = 7;

			if (useUVs)
			{
				position += 2;
				numElements += 2;
			}
			if (useNormals)
			{
				position += 3;
				numElements += 7;
			}

			for (int i = position; i < numVertices; i += numElements)
			{
				vertexData[position] = c.getR();
				vertexData[position + 1] = c.getG();
				vertexData[position + 2] = c.getB();
				vertexData[position + 3] = c.getA();
			}
		}
	}

	int Mesh::getNumberOfVertices() const
	{
		if (objData.size() > 0)
		{
			return objData.size() / 8;
		}
		else
		{
			return numVertices;
		}
	}

	std::shared_ptr<sf::Texture> Mesh::getDiffuseMap() const
	{
		return this->diffuseMap;
	}

	std::shared_ptr<sf::Texture> Mesh::getSpecularMap() const
	{
		return this->specularMap;
	}

	float Mesh::getWidth() const
	{
		return width;
	}

	float Mesh::getHeight() const
	{
		return height;
	}

	float Mesh::getDepth() const
	{
		return depth;
	}

	bool Mesh::loadFromFile(const std::string &fileName)
	{
		FILE *binaryFile = nullptr;
		fopen_s(&binaryFile, fileName.c_str(), "rb");

		if (binaryFile == nullptr)
		{
			std::cout << "Could not open file for reading: " << fileName << std::endl;

			return false;
		}

		unsigned int size = 0;
		fread(&size, sizeof(unsigned int), 1, binaryFile);

		objData.resize(size);

		fread(&objData[0], sizeof(GLfloat), size, binaryFile);

		return true;
	}
}