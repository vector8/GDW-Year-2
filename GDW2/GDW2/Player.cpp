#include "Player.h"
#include "Game.h"
#include <iostream>
#include "SoundManager.h"
#include <vector>
#include <sstream>

namespace flopse
{
	Player::Player() : jumping(false), dy(0.f)
	{
		initAnimations();

		footsteps = new Sound("sounds/footstep.wav", true);

		camJoint = std::make_shared<SceneNode>();
		camJoint->parent = this;
		children.add(camJoint);
	}

	Player::~Player()
	{
		delete footsteps;
	}

	void Player::initAnimations()
	{
		std::vector<Keyframe> frames;
		std::shared_ptr<Shader> s = Shader::getStandardShader(StandardShaders::Phong);

		// Run animation
		for (int i = 1; i <= 5; i++)
		{
			std::string filename = "meshes/PlayerRun" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");
			/*frame.mesh->setDiffuseMap("textures/GoblinTexture.png");
			frame.mesh->setSpecularMap("textures/GoblinSpecularMap.png");*/

			frame.duration = sf::seconds(0.22f);

			/*if (i == 8)
			{
				frame.duration = sf::seconds(0.033);
			}
			else
			{
				frame.duration = sf::seconds(0.1);
			}*/

			frames.push_back(frame);
		}
		runAnimation = std::make_shared<Animation>(frames);
		mesh = runAnimation->getCurrentMesh();

		// Idle animation
		for (int i = 1; i <= 5; i++)
		{
			std::string filename = "meshes/idle" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");

			frame.duration = sf::seconds(0.5f);

			frames.push_back(frame);
		}
		idleAnimation = std::make_shared<Animation>(frames);
		mesh = idleAnimation->getCurrentMesh(); frames.clear();

		//strafe left
		for (int i = 1; i <= 5; i++)
		{
			std::string filename = "meshes/strafeleft" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");

			frame.duration = sf::seconds(0.5f);

			frames.push_back(frame);
		}
		strafeLeftAnimation = std::make_shared<Animation>(frames);
		mesh = strafeLeftAnimation->getCurrentMesh();

		//strafe right
		for (int i = 1; i <= 5; i++)
		{
			std::string filename = "meshes/straferight" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");

			frame.duration = sf::seconds(0.5f);

			frames.push_back(frame);
		}
		strafeRightAnimation = std::make_shared<Animation>(frames);
		mesh = strafeRightAnimation->getCurrentMesh();

		//backwards
		/*for (int i = 5; i >= 1; i--)
		{
			std::string filename = "meshes/PlayerRun" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");

			frame.duration = sf::seconds(0.22f);

			frames.push_back(frame);
		}
		backAnimation = std::make_shared<Animation>(frames);
		frames.clear();*/

		//cast
		for (int i = 1; i <= 5; i++)
		{
			std::string filename = "meshes/cast" + std::to_string(i) + ".bmf";
			Keyframe frame;
			frame.mesh = std::make_shared<Mesh>(filename, s);
			frame.mesh->setDiffuseMap("textures/PlayerDiffuse.png");
			frame.mesh->setSpecularMap("textures/BlankSpecular.png");

			frame.duration = sf::seconds(0.5f);

			frames.push_back(frame);
		}
		castAnimation = std::make_shared<Animation>(frames);
		mesh = castAnimation->getCurrentMesh();
	}

	void Player::attachCam(const std::shared_ptr<SceneNode> &n)
	{
		camJoint->attach(n);
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

			if (!jumping)
			{
				footsteps->play();
				runAnimation->update(dt);
			}
			else
			{
				footsteps->setPaused(true);
			}
			mesh = runAnimation->getCurrentMesh();
		}
		else
		{
			footsteps->setPaused(true);
			idleAnimation->update(dt);
			mesh = idleAnimation->getCurrentMesh();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getUp(), glm::cross(localTransform.getFront(), localTransform.getUp())));
			//backAnimation->update(dt);
			//mesh = backAnimation->getCurrentMesh();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			newPos -= speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getFront(), localTransform.getUp()));
			strafeLeftAnimation->update(dt);
			mesh = strafeLeftAnimation->getCurrentMesh();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			newPos += speed * dt.asSeconds() * glm::normalize(glm::cross(localTransform.getFront(), localTransform.getUp()));
			strafeRightAnimation->update(dt);
			mesh = strafeRightAnimation->getCurrentMesh();
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
				
				if (dy <= 0)
				{
					jumping = false;
				}
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
		// Reset mouse to middle of screen
		sf::Mouse::setPosition(sf::Vector2i((int)(window.getSize().x) / 2, (int)(window.getSize().y) / 2), window);
		float xoffset = mouse.x - (int)(window.getSize().x) / 2;
		float yoffset = (int)(window.getSize().y) / 2 - mouse.y;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		localTransform.yaw += xoffset;

		// limit pitch to between 23 and -60 degrees
		if (camJoint->localTransform.pitch + yoffset > 23.f)
		{
			yoffset = 23.f - camJoint->localTransform.pitch;
		}
		else if (camJoint->localTransform.pitch + yoffset < -60.f)
		{
			yoffset = -60.f - camJoint->localTransform.pitch;
		}
		camJoint->localTransform.pitch += yoffset;

		localTransform.rotate(-xoffset, localTransform.getUp());
		camJoint->localTransform.rotate(yoffset, glm::cross(camJoint->localTransform.getFront(), camJoint->localTransform.getUp()));
	}

	void Player::postUpdate(const sf::RenderWindow &window, const sf::Time &dt)
	{
		footsteps->setPosition(this->getGlobalPosition());
		//std::cout << this->getGlobalPosition().x << ", " << this->getGlobalPosition().y << ", " << this->getGlobalPosition().z << std::endl;
	}

	void Player::jump()
	{
		if (!jumping)
		{
			dy = 375.f;
			jumping = true;
		}
	}

	glm::vec3 Player::getAimDirection()
	{
		return camJoint->getGlobalFront();
	}
}