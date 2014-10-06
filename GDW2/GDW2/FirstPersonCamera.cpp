#include "FirstPersonCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

FirstPersonCamera::FirstPersonCamera() : Camera()
{
}

FirstPersonCamera::FirstPersonCamera(glm::vec3 pos, glm::vec3 target) : Camera(pos, target)
{
}

void FirstPersonCamera::update(const sf::RenderWindow &window)
{
	GLfloat camSpeed = 0.005f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		cameraPos += camSpeed * glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		cameraPos -= camSpeed * glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		cameraPos -= glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)) * camSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		cameraPos += glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)) * camSpeed;

	sf::Vector2i mouse = sf::Mouse::getPosition(window);
	GLfloat xoffset = mouse.x - 400;
	GLfloat yoffset = 300 - mouse.y;

	GLfloat sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
}