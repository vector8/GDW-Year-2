#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

namespace flopse
{
	class Camera
	{
	protected:
		glm::vec3 cameraPos, cameraFront;
		GLfloat yaw, pitch;

	public:
		Camera();
		Camera(const glm::vec3 &pos, const glm::vec3 &target);

		glm::mat4 view;

		glm::vec3 getPosition() const;

		void setPosition(const glm::vec3 &pos);
		void lookAt(const glm::vec3 &target);

		virtual void update(const sf::RenderWindow &window, const sf::Time &dt);
	};
}