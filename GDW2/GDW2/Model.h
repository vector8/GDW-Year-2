#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class Model
{
private:
	GLuint VBO;

	//glm::vec3 position, direction, scale, defaultDirection;

	glm::mat4 rotationMatrix, translationMatrix, scaleMatrix;

	GLfloat *vertexData;
	int numVertices;

	float width, height, depth;

	std::vector<GLfloat> objData;

	void loadOBJ(std::string fileName);

	sf::Texture* texture;

public:
	Model(GLfloat *vertexData, int numVertices, Shader *s);
	Model(std::string objFileName, Shader *s);

	void initArrays(GLfloat *vertexData, int numVertices);
	void calculateDimensions(GLfloat *vertexData, int numVertices);

	Shader *shader;
	GLuint VAO;

	void setPosition(glm::vec3 pos);
	void translate(glm::vec3 t);
	void rotate(float degrees, glm::vec3 axis);
	/*void setDirection(glm::vec3 axis);
	void setDefaultDirection(glm::vec3 axis);*/
	void scale(glm::vec3 s);

	void setTexture(std::string filename);
	void setTexture(sf::Texture *t);

	int getNumberOfVertices() const;

	glm::mat4 getTransform() const;

	glm::mat4 getRotationMatrix() const;
	glm::mat4 getTranslationMatrix() const;
	glm::mat4 getScaleMatrix() const;

	glm::vec3 getFront() const;
	glm::vec3 getUp() const;
	glm::vec3 getPosition() const;

	sf::Texture* getTexture() const;

	float getWidth() const;
	float getHeight() const;
	float getDepth() const;
	
	/*glm::vec3 getDirection() const;
	glm::vec3 getScale() const;*/
};