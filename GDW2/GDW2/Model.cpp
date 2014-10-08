#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include "Utilities.h"

Model::Model(GLfloat *vertexData, int numVertices, Shader *s) : vertexData(vertexData), numVertices(numVertices), shader(s), translationMatrix(), rotationMatrix(), scaleMatrix()
{
	this->initArrays(vertexData, numVertices);
}

Model::Model(std::string objFileName, Shader *s) : shader(s), translationMatrix(), rotationMatrix(), scaleMatrix()
{
	this->loadOBJ(objFileName);

	this->initArrays(&objData[0], objData.size() / 8);
}

void Model::initArrays(GLfloat *vertexData, int numVertices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * numVertices * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	// position
	GLuint loc = glGetAttribLocation(shader->program, "position");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(loc);

	// uv
	/*loc = glGetAttribLocation(shader->program, "uv");
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(loc);*/

	// normal
	loc = glGetAttribLocation(shader->program, "normal");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(loc);

	glBindVertexArray(0);
}

void Model::setPosition(glm::vec3 pos)
{
	glm::mat4 temp;
	temp = glm::translate(temp, pos);
	translationMatrix = temp;
}

void Model::translate(glm::vec3 t)
{
	glm::mat4 temp;
	temp = glm::translate(temp, t);
	translationMatrix = temp * translationMatrix;
}

void Model::rotate(float degrees, glm::vec3 axis)
{
	glm::mat4 temp;
	temp = glm::rotate(temp, degrees, axis);
	rotationMatrix = temp * rotationMatrix;
}

void Model::scale(glm::vec3 s)
{
	glm::mat4 temp;
	temp = glm::scale(temp, s);
	scaleMatrix = temp * scaleMatrix;
}

void Model::loadOBJ(std::string fileName)
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
			while(s >> v)
				vertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vt ")
		{
			std::istringstream s(line.substr(3));
			GLfloat uv;
			while (s >> uv)
				uvs.push_back(uv);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream s(line.substr(3));
			GLfloat n;
			while (s >> n)
				normals.push_back(n);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::vector<std::string> tokens = split(line.substr(2), " /");

			assert(tokens.size() == 9);

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
		}
		else
		{
			// ignore the line
		}
	}
}

int Model::getNumberOfVertices() const
{
	if (objData.size() > 0)
		return objData.size() / 8;
	else
		return numVertices;
}

glm::mat4 Model::getTransform() const
{
	return (translationMatrix * rotationMatrix) * scaleMatrix;
}

glm::mat4 Model::getRotationMatrix() const
{
	return rotationMatrix;
}

glm::mat4 Model::getTranslationMatrix() const
{
	return translationMatrix;
}

glm::mat4 Model::getScaleMatrix() const
{
	return scaleMatrix;
}

glm::vec3 Model::getFront() const
{
	glm::vec4 front(1.f, 0.f, 0.f, 1.f);

	front = rotationMatrix * front;

	return glm::vec3(front.x, front.y, front.z);
}

glm::vec3 Model::getUp() const
{
	/*glm::vec4 up(0.f, 1.f, 0.f, 1.f);

	up = rotationMatrix * up;

	return glm::vec3(up.x, up.y, up.z);*/

	return glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Model::getPosition() const
{
	return glm::vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
}