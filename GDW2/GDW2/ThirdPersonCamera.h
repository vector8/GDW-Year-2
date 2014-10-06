#pragma once

#include "Camera.h"
#include "Model.h"

class ThirdPersonCamera : public Camera
{
private:
	ThirdPersonCamera();
	ThirdPersonCamera(glm::vec3 pos, glm::vec3 target);

	Model* target;
	float distance;

public:
	ThirdPersonCamera(Model* target, float distance);

	void update(const sf::RenderWindow &window);
};