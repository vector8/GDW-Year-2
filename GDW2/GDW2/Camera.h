#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

class Camera
{
protected:
	glm::vec3 cameraPos, cameraFront;
	GLfloat yaw, pitch;

public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 target);

	glm::mat4 view;

	void setPosition(glm::vec3 pos);
	void lookAt(glm::vec3 target);

	virtual void update(const sf::RenderWindow &window);
};