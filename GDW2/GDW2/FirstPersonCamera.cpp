#include "FirstPersonCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>

namespace flopse
{
	FirstPersonCamera::FirstPersonCamera() : Camera()
	{
	}

	FirstPersonCamera::~FirstPersonCamera()
	{

	}

	void FirstPersonCamera::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		GLfloat camSpeed = 10.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			localTransform.translate(dt.asSeconds() * camSpeed * glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(localTransform.getFront(), glm::vec3(0.f, 1.f, 0.f)))));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			localTransform.translate(-dt.asSeconds() * camSpeed * glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), glm::cross(localTransform.getFront(), glm::vec3(0.f, 1.f, 0.f)))));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			localTransform.translate(-dt.asSeconds() * camSpeed * glm::normalize(glm::cross(localTransform.getFront(), glm::vec3(0.f, 1.f, 0.f))));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			localTransform.translate(dt.asSeconds() * camSpeed * glm::normalize(glm::cross(localTransform.getFront(), glm::vec3(0.f, 1.f, 0.f))));
		}

		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		GLfloat xoffset = mouse.x - (int)(window.getSize().x) / 2;
		GLfloat yoffset = (int)(window.getSize().y) / 2 - mouse.y;

		GLfloat sensitivity = 0.1;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		localTransform.yaw += xoffset;

		if (std::abs(localTransform.pitch + yoffset) <= 89.f)
		{
			localTransform.pitch += yoffset;
		}
		else if (yoffset > 0)
		{
			yoffset = 89.f - localTransform.pitch;
			localTransform.pitch = 89.f;
		}
		else
		{
			yoffset = -89.f - localTransform.pitch;
			localTransform.pitch = -89.f;
		}

		localTransform.rotate(-xoffset, localTransform.getUp());
		localTransform.rotate(yoffset, glm::cross(localTransform.getFront(), localTransform.getUp()));

		recalculateView();
	}
}