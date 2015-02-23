#pragma once

#include "Camera.h"

namespace flopse
{
	class FirstPersonCamera : public Camera
	{
	public:
		FirstPersonCamera();
		virtual ~FirstPersonCamera();

		void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}