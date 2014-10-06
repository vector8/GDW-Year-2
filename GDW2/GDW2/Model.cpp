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

#ifndef M_PI
#define M_PI 3.14159265359
#endif

Model::Model(GLfloat *vertexData, int numVertices, Shader *s) : vertexData(vertexData), numVertices(numVertices), shader(s), useEBO(false), translationMatrix(), rotationMatrix(), scaleMatrix()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * numVertices * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * numVertices * sizeof(GLfloat), vertexData);

	// position
	GLuint loc = glGetAttribLocation(shader->program, "position");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(loc);

	// colour
	loc = glGetAttribLocation(shader->program, "color");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(loc);

	glBindVertexArray(0);
}

Model::Model(std::string objFileName, Shader *s) : shader(s), useEBO(true), translationMatrix(), rotationMatrix(), scaleMatrix()
{
	this->loadOBJ(objFileName);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * vertices.size() * sizeof(GLfloat), &vertices[0]);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(GLuint), &vertexIndices[0], GL_STATIC_DRAW);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, vertexIndices.size() * sizeof(GLuint), &vertexIndices[0]);

	// position
	GLuint loc = glGetAttribLocation(shader->program, "position");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
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
					vertexIndices.push_back(temp);
				}
				//else
					//vertexIndices.push_back(-1);

				if (tokens[i + 1].length() > 0)
				{
					s.str(tokens[i + 1]);
					s.clear();
					assert(s >> temp);
					temp--;
					uvIndices.push_back(temp);
				}
				//else
					//uvIndices.push_back(-1);

				if (tokens[i + 2].length() > 0)
				{
					s.str(tokens[i + 2]);
					s.clear();
					assert(s >> temp);
					temp--;
					normalIndices.push_back(temp);
				}
				//else
					//normalIndices.push_back(-1);
			}
		}
		else
		{
			// ignore the line
		}
	}
}

bool Model::usingEBO() const
{
	return useEBO;
}

int Model::getNumberOfVertices() const
{
	if (useEBO)
		return vertexIndices.size();
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