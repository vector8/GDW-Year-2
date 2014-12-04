#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include "Utilities.h"

namespace flopse
{
	Mesh::Mesh(GLfloat *vertexData, int numVertices, Shader *s, const std::string &textureFilename, bool useUVs, bool useNormals, bool useColour) : vertexData(vertexData), numVertices(numVertices), shader(s),
		texture(NULL), useUVs(useUVs), useNormals(useNormals), useColour(useColour)
	{
		this->initArrays(vertexData, numVertices, useUVs, useNormals);

		calculateDimensions(vertexData, numVertices);

		if (textureFilename.size() > 0)
		{
			setTexture(textureFilename);
		}
	}

	Mesh::Mesh(const std::string &objFileName, Shader *s, const std::string &textureFilename) : shader(s), texture(NULL), width(0.f), height(0.f), depth(0.f), useUVs(true), useNormals(true), useColour(false)
	{
		this->loadOBJ(objFileName);

		this->initArrays(&objData[0], objData.size() / 8);

		calculateDimensions(&objData[0], objData.size() / 8);

		if (textureFilename.size() > 0)
		{
			setTexture(textureFilename);
		}
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

	void Mesh::loadOBJ(const std::string &fileName)
	{
		std::ifstream in(fileName, std::ios::in);
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;

		if (!in)
		{
			std::cout << "Cannot open " << fileName << std::endl;
			assert(false);
		}

		std::string line;
		while (std::getline(in, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				std::istringstream s(line.substr(2));
				GLfloat v;
				while (s >> v)
				{
					vertices.push_back(v);
				}
			}
			else if (line.substr(0, 3) == "vt ")
			{
				std::istringstream s(line.substr(3));
				GLfloat uv;
				while (s >> uv)
				{
					uvs.push_back(uv);
				}
			}
			else if (line.substr(0, 3) == "vn ")
			{
				std::istringstream s(line.substr(3));
				GLfloat n;
				while (s >> n)
				{
					normals.push_back(n);
				}
			}
			else if (line.substr(0, 2) == "f ")
			{
				std::vector<std::string> tokens = split(line.substr(2), " /");

				assert(tokens.size() == 9);

				if (tokens.size() == 9)
				{
					std::istringstream s;
					GLuint temp;
					for (int i = 0; i < 9; i += 3)
					{
						if (tokens[i].length() > 0)
						{
							s.str(tokens[i]);
							s.clear();
							assert(s >> temp);
							temp--;
							objData.push_back(vertices[temp * 3]);
							objData.push_back(vertices[(temp * 3) + 1]);
							objData.push_back(vertices[(temp * 3) + 2]);
						}

						if (tokens[i + 1].length() > 0)
						{
							s.str(tokens[i + 1]);
							s.clear();
							assert(s >> temp);
							temp--;
							objData.push_back(uvs[temp * 2]);
							objData.push_back(uvs[(temp * 2) + 1]);
						}

						if (tokens[i + 2].length() > 0)
						{
							s.str(tokens[i + 2]);
							s.clear();
							assert(s >> temp);
							temp--;
							objData.push_back(normals[temp * 3]);
							objData.push_back(normals[(temp * 3) + 1]);
							objData.push_back(normals[(temp * 3) + 2]);
						}
					}
				}/*
				else if (tokens.size() == 6)
				{
					std::istringstream s;
					GLuint temp;
					for (int i = 0; i < 6; i += 2)
					{
						if (tokens[i].length() > 0)
						{
							s.str(tokens[i]);
							s.clear();
							assert(s >> temp);
							temp--;
							objData.push_back(vertices[temp * 3]);
							objData.push_back(vertices[(temp * 3) + 1]);
							objData.push_back(vertices[(temp * 3) + 2]);
						}

						objData.push_back(0.f);
						objData.push_back(0.f);

						if (tokens[i + 1].length() > 0)
						{
							s.str(tokens[i + 1]);
							s.clear();
							assert(s >> temp);
							temp--;
							objData.push_back(normals[temp * 3]);
							objData.push_back(normals[(temp * 3) + 1]);
							objData.push_back(normals[(temp * 3) + 2]);
						}
					}
				}*/
			}
			else
			{
				// ignore the line
			}
		}
	}

	void Mesh::setTexture(const std::string &filename)
	{
		this->texture = new sf::Texture();
		if (!this->texture->loadFromFile(filename))
		{
			std::cout << "!!!!ERROR LOADING TEXTURE!!!!" << std::endl;
		}
	}

	void Mesh::setTexture(sf::Texture* t)
	{
		this->texture = t;
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

	sf::Texture* Mesh::getTexture() const
	{
		return this->texture;
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
}