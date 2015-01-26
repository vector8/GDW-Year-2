#pragma once

#include "Camera.h"

namespace flopse
{
	class ThirdPersonCamera : public Camera
	{
	private:
		//ThirdPersonCamera();
		//ThirdPersonCamera(const glm::vec3 &pos, const glm::vec3 &target);

	public:
		ThirdPersonCamera();
		//ThirdPersonCamera(Mesh* target, float distance);

		void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}