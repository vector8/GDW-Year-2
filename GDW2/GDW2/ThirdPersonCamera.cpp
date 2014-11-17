#include "ThirdPersonCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

namespace flopse
{
	ThirdPersonCamera::ThirdPersonCamera() : Camera()
	{
	}

	ThirdPersonCamera::ThirdPersonCamera(Entity* target, float distance) : target(target), distance(distance)
	{

	}

	void ThirdPersonCamera::update(const sf::RenderWindow &window, const sf::Time &dt)
	{
		setPosition(target->getGlobalPosition() + glm::vec3(-1.f * distance * target->getGlobalFront()));
		lookAt(target->getGlobalPosition());
	}
}