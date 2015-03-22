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
		//ThirdPersonCamera(std::shared_ptr<Mesh> target, float distance);
		virtual ~ThirdPersonCamera();
	};
}