#include "ThirdPersonCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

ThirdPersonCamera::ThirdPersonCamera() : Camera()
{
}

ThirdPersonCamera::ThirdPersonCamera(const glm::vec3 &pos, const glm::vec3 &target) : Camera(pos, target)
{
}

ThirdPersonCamera::ThirdPersonCamera(Model* target, float distance) : target(target), distance(distance)
{

}

void ThirdPersonCamera::update(const sf::RenderWindow &window, const sf::Time &dt)
{
	setPosition(target->getPosition() + glm::vec3(-1.f * distance * target->getFront()));
	lookAt(target->getPosition());
}