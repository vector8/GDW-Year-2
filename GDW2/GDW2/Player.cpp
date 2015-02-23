#include "Player.h"
#include "Game.h"
#include <iostream>
#include "SoundManager.h"
#include <vector>
#include <sstream>

namespace flopse
{
	Player::Player(std::shared_ptr<Mesh> m) : Entity(m), jumping(false), dy(0.f)
	{
		idleMesh = m;
		std::vector<Keyframe> runFrames;
		Shader* s = new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag");

		for (int i = 1; i < 9; i++)
		{
			std::stringstream ss;
			ss << "meshes/GoblinRun" << i << ".bmf";
			std::string filename = ss.str();
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setTexture("textures/GoblinTexture.png");

			if (i == 8)
			{
				frame.duration = sf::seconds(0.033);
			}
			else
			{
				frame.duration = sf::seconds(0.1);
			}

			runFrames.push_back(frame);
		}

		runAnimation = new Animation(runFrames);

		footsteps = new Sound("sounds/footstep.wav", true);
	}

	Player::~Player()
	{
		delete runAnimation;
		delete footsteps;
	}

	void Player::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		dy -= 600.f * dt.asSeconds();

		if (dy < -400.f)
		{
			dy = -400.f;
		}

		GLfloat speed = 400.f;

		glm::vec3 position = localTransform.getPosition();
		glm::vec3 newPos(position);

		// update position
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getUp(), glm::cross(localTransform.getFront(), localTransform.getUp())));
			footsteps->play();

			runAnimation->update(dt);
			mesh = runAnimation->getCurrentMesh();
		}
		else
		{
			footsteps->setPaused(true);
			mesh = idleMesh;
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

		if (abs(dy) > 0.1f)
		{
			newPos.y += dy * dt.asSeconds() - 15.f * dt.asSeconds() * dt.asSeconds();
		}

		Game* g = Game::getGame();
		// check for collision
		bool collidedX = false, collidedY = false, collidedZ = false;
		std::vector<BoundingBox> colliders = g->getColliders();
		for (std::vector<BoundingBox>::iterator it = colliders.begin(); it != colliders.end(); it++)
		{
			boundingBox.position = glm::vec3(newPos.x, position.y, position.z);

			if (boundingBox.hasCollided(*it))
			{
				collidedX = true;
			}

			boundingBox.position = glm::vec3(position.x, newPos.y, position.z);

			if (boundingBox.hasCollided(*it))
			{
				collidedY = true;
				jumping = false;
			}

			boundingBox.position = glm::vec3(position.x, position.y, newPos.z);

			if (boundingBox.hasCollided(*it))
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

		boundingBox.position = newPos;
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

		footsteps->setPosition(this->getGlobalPosition());

		//std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
	}

	void Player::jump()
	{
		if (!jumping)
		{
			dy = 600.f;
			jumping = true;
		}
	}
}