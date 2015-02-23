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
	
	Camera::~Camera()
	{

	}

	glm::vec3 Camera::getLocalPosition() const
	{
		return localTransform.getPosition();
	}

	glm::vec3 Camera::getGlobalPosition() const
	{
		glm::vec4 tpos4(0.f, 0.f, 0.f, 1.f);

		tpos4 = globalTransform * tpos4;

		return glm::vec3(tpos4.x, tpos4.y, tpos4.z);
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
		glm::vec4 tpos4(0.f, 0.f, 0.f, 1.f), tfront4(0.f, 0.f, 1.f, 0.f);

		tpos4 = globalTransform * tpos4;
		tfront4 = globalTransform * tfront4;

		glm::vec3 tpos3(tpos4.x, tpos4.y, tpos4.z);
		glm::vec3 tfront3(tfront4.x, tfront4.y, tfront4.z);

		view = glm::lookAt(tpos3, tpos3 + tfront3, glm::vec3(0.f, 1.f, 0.f));
	}
}