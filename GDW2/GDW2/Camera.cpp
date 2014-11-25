#include "Camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

namespace flopse
{
	Camera::Camera()
	{
		recalculateView();
	}

	glm::vec3 Camera::getLocalPosition() const
	{
		return localTransform.getPosition();
	}

	glm::vec3 Camera::getGlobalPosition() const
	{
		glm::vec4 tpos4(0.f, 0.f, 0.f, 1.f);

		tpos4 = globalTransform * tpos4;

		return glm::vec3(tpos4[0], tpos4[1], tpos4[2]);
	}

	void Camera::setPosition(const glm::vec3 &pos)
	{
		localTransform.setPosition(pos);
	}

	void Camera::lookAt(const glm::vec3 &target)
	{
		//glm::vec3 newFront = glm::normalize(target - globalTransform.getPosition());
		// TODO: figure out how to calculate rotation based on target..
	}

	void Camera::recalculateView()
	{
		glm::vec4 tpos4(0.f, 0.f, 0.f, 1.f), tfront4(0.f, 0.f, -1.f, 0.f);

		tpos4 = globalTransform * tpos4;
		tfront4 = globalTransform * tfront4;

		glm::vec3 tpos3(tpos4[0], tpos4[1], tpos4[2]);
		glm::vec3 tfront3(tfront4[0], tfront4[1], tfront4[2]);

		view = glm::lookAt(tpos3, tpos3 + tfront3, glm::vec3(0.f, 1.f, 0.f));
	}
}