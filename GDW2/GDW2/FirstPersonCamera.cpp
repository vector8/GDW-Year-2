#include "FirstPersonCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

namespace flopse
{
	FirstPersonCamera::FirstPersonCamera() : Camera()
	{
	}

	FirstPersonCamera::FirstPersonCamera(const glm::vec3 &pos, const glm::vec3 &target) : Camera(pos, target)
	{
	}

	void FirstPersonCamera::update(const sf::RenderWindow &window, const sf::Time &dt)
	{
		GLfloat camSpeed = 10.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			cameraPos += dt.asSeconds() * camSpeed * glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f))));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			cameraPos -= dt.asSeconds() * camSpeed * glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f))));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			cameraPos -= dt.asSeconds() * camSpeed * glm::normalize(glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			cameraPos += dt.asSeconds() * camSpeed * glm::normalize(glm::cross(cameraFront, glm::vec3(0.f, 1.f, 0.f)));
		}

		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		GLfloat xoffset = mouse.x - window.getSize().x / 2;
		GLfloat yoffset = window.getSize().y / 2 - mouse.y;

		GLfloat sensitivity = 0.1;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		while (yaw > 360.f)
		{
			yaw -= 360.f;
		}
		while (yaw < -360.f)
		{
			yaw += 360.f;
		}

		glm::vec3 front;
		// yaw - 90.f to account for the fact that we use -z as front instead of +x
		front.x = cos(glm::radians(yaw - 90.f)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw - 90.f)) * cos(glm::radians(pitch));

		// inverted camera:
		/*front.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));*/
		cameraFront = glm::normalize(front);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.f, 1.f, 0.f));
	}
}