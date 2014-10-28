#include "Camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

Camera::Camera() : yaw(0.f), pitch(0.f)
{
	cameraPos = glm::vec3(0.f, 0.f, 2.f);
	cameraFront = glm::vec3(0.f, 0.f, -1.f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
}

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &target) : cameraPos(pos)
{
	cameraFront = glm::normalize(target - cameraPos);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));

	glm::vec3 oldFront(0.f, 0.f, -1.f), newFront(cameraFront.x, 0.f, cameraFront.z);
	yaw = glm::degrees(acosf(glm::dot(oldFront, newFront) / (glm::length(oldFront) * glm::length(newFront))));
	pitch = glm::degrees(acosf(glm::dot(newFront, cameraFront) / (glm::length(newFront) * glm::length(cameraFront))));
}

glm::vec3 Camera::getPosition() const
{
	return this->cameraPos;
}

void Camera::setPosition(const glm::vec3 &pos)
{
	cameraPos = glm::vec3(pos);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::lookAt(const glm::vec3 &target)
{
	cameraFront = glm::normalize(target - cameraPos);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::update(const sf::RenderWindow &window, const sf::Time &dt)
{

}