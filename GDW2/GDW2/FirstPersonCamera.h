#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera();
	FirstPersonCamera(glm::vec3 pos, glm::vec3 target);

	void update(const sf::RenderWindow &window);
};