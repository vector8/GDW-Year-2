#include "Player.h"
#include "Game.h"

Player::Player(const glm::vec3 &pos, const Model &model) : Entity(pos, model), jumping(false), dy(0.f)
{

}

void Player::update(const sf::Window &window, const sf::Time &dt)
{
	dy -= 30.f * dt.asSeconds();

	if (dy < -20.f)
		dy = -20.f;

	GLfloat speed = 10.f;

	glm::vec3 newPos(pos);

	// update position
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(model->getUp(), glm::cross(model->getFront(), model->getUp())));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(model->getUp(), glm::cross(model->getFront(), model->getUp())));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(model->getFront(), model->getUp()));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(model->getFront(), model->getUp()));

	newPos.y += dy * dt.asSeconds() - 15.f * dt.asSeconds() * dt.asSeconds();

	Game* g = Game::getGame();
	// check for collision
	bool collidedX = false, collidedY = false, collidedZ = false;
	for (std::vector<BoundingBox*>::iterator it = g->colliders.begin(); it != g->colliders.end(); it++)
	{
		box->setPosition(glm::vec3(newPos.x, pos.y, pos.z));

		if (box->hasCollided(*(*it)))
		{
			collidedX = true;
		}

		box->setPosition(glm::vec3(pos.x, newPos.y, pos.z));

		if (box->hasCollided(*(*it)))
		{
			collidedY = true;
			jumping = false;
		}

		box->setPosition(glm::vec3(pos.x, pos.y, newPos.z));

		if (box->hasCollided(*(*it)))
		{
			collidedZ = true;
		}
	}

	if (collidedX)
		newPos.x = pos.x;
	if (collidedY)
		newPos.y = pos.y;
	if (collidedZ)
		newPos.z = pos.z;
	
	pos = glm::vec3(newPos);
	box->setPosition(newPos);
	model->setPosition(newPos);

	sf::Vector2i mouse = sf::Mouse::getPosition(window);
	GLfloat xoffset = mouse.x - 400;
	GLfloat yoffset = 300 - mouse.y;

	GLfloat sensitivity = 0.1;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	model->yaw += xoffset;

	if (std::abs(model->pitch + yoffset) <= 89.f)
	{
		model->pitch += yoffset;
	}
	else if (yoffset > 0)
	{
		yoffset = 89.f - model->pitch;
		model->pitch = 89.f;
	}
	else
	{
		yoffset = -89.f - model->pitch;
		model->pitch = -89.f;
	}

	model->rotate(-xoffset, model->getUp());
	model->rotate(yoffset, glm::cross(model->getFront(), model->getUp()));
}

void Player::jump()
{
	if (!jumping)
	{
		dy = 15.f;
		jumping = true;
	}
}