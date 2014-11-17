#pragma once

#include "Camera.h"
#include "Entity.h"

namespace flopse
{
	class ThirdPersonCamera : public Camera
	{
	private:
		ThirdPersonCamera();
		ThirdPersonCamera(const glm::vec3 &pos, const glm::vec3 &target);

		Entity* target;
		float distance;

	public:
		ThirdPersonCamera(Entity* target, float distance);

		void update(const sf::RenderWindow &window, const sf::Time &dt);
	};
}