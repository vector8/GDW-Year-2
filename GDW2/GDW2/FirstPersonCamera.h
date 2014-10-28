#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera();
	FirstPersonCamera(const glm::vec3 &pos, const glm::vec3 &target);

	void update(const sf::RenderWindow &window, const sf::Time &dt);
};