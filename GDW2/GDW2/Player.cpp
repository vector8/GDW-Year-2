#include "Player.h"
#include "Game.h"

namespace flopse
{
	Player::Player(Model* model) : Entity(model), jumping(false), dy(0.f)
	{

	}

	void Player::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		dy -= 30.f * dt.asSeconds();

		if (dy < -20.f)
		{
			dy = -20.f;
		}

		GLfloat speed = 10.f;

		glm::vec3 position = localTransform.getPosition();
		glm::vec3 newPos(position);

		// update position
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getUp(), glm::cross(localTransform.getFront(), localTransform.getUp())));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getUp(), glm::cross(localTransform.getFront(), localTransform.getUp())));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getFront(), localTransform.getUp()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getFront(), localTransform.getUp()));
		}

		newPos.y += dy * dt.asSeconds() - 15.f * dt.asSeconds() * dt.asSeconds();

		Game* g = Game::getGame();
		// check for collision
		bool collidedX = false, collidedY = false, collidedZ = false;
		for (std::vector<BoundingBox*>::iterator it = g->colliders.begin(); it != g->colliders.end(); it++)
		{
			boundingBox->position = glm::vec3(newPos.x, position.y, position.z);

			if (boundingBox->hasCollided(*(*it)))
			{
				collidedX = true;
			}

			boundingBox->position = glm::vec3(position.x, newPos.y, position.z);

			if (boundingBox->hasCollided(*(*it)))
			{
				collidedY = true;
				jumping = false;
			}

			boundingBox->position = glm::vec3(position.x, position.y, newPos.z);

			if (boundingBox->hasCollided(*(*it)))
			{
				collidedZ = true;
			}
		}

		if (collidedX)
		{
			newPos.x = position.x;
		}
		if (collidedY)
		{
			newPos.y = position.y;
		}
		if (collidedZ)
		{
			newPos.z = position.z;
		}

		boundingBox->position = newPos;
		localTransform.setPosition(newPos);

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
	}

	void Player::jump()
	{
		if (!jumping)
		{
			dy = 15.f;
			jumping = true;
		}
	}
}