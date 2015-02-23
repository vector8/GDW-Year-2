#include "ThirdPersonCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace flopse
{
	ThirdPersonCamera::ThirdPersonCamera() : Camera()
	{
	}

	ThirdPersonCamera::~ThirdPersonCamera()
	{

	}

	void ThirdPersonCamera::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		recalculateView();
	}
}