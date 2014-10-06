#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>

class Model
{
private:
	GLuint VBO, EBO;

	//glm::vec3 position, direction, scale, defaultDirection;

	glm::mat4 rotationMatrix, translationMatrix, scaleMatrix;

	GLfloat *vertexData;
	int numVertices;

	bool useEBO;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;
	std::vector<GLuint> vertexIndices, uvIndices, normalIndices;

	void loadOBJ(std::string fileName);

public:
	Model(GLfloat *vertexData, int numVertices, Shader *s);
	Model(std::string objFileName, Shader *s);

	Shader *shader;
	GLuint VAO;

	void setPosition(glm::vec3 pos);
	void translate(glm::vec3 t);
	void rotate(float degrees, glm::vec3 axis);
	/*void setDirection(glm::vec3 axis);
	void setDefaultDirection(glm::vec3 axis);*/
	void scale(glm::vec3 s);

	bool usingEBO() const;
	int getNumberOfVertices() const;

	glm::mat4 getTransform() const;

	glm::mat4 getRotationMatrix() const;
	glm::mat4 getTranslationMatrix() const;
	glm::mat4 getScaleMatrix() const;

	glm::vec3 getFront() const;
	glm::vec3 getUp() const;
	glm::vec3 getPosition() const;
	
	/*glm::vec3 getDirection() const;
	glm::vec3 getScale() const;*/
};