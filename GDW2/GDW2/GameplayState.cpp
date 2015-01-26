#include "GameplayState.h"
#include <glm\gtc\matrix_transform.hpp>

#include "Tower.h"
#include "Projectile.h"
#include "FSoundManager.h"
#include "Game.h"

namespace flopse
{
	GameplayState::GameplayState(sf::RenderWindow* window) : window(window), hud(window)
	{
		Mesh *playerMesh = new Mesh("meshes/Goblin.bmf", new Shader("shaders/texShader.vert", "shaders/texShader.frag"));
		playerMesh->setTexture("textures/GoblinTexture.png");
		player = new Player(playerMesh);

		currentLevel = new Level(player);
		root = currentLevel;

		projection = glm::perspective(Game::getGame()->getFieldOfView(), 1024.0f / 768.0f, 0.1f, 100000.0f);

		//ParticleSystem *s = particleManager->createParticleSystem(ParticleSystemBehaviour::Swarm, 4, 1000, glm::vec3(5.f, 5.f, 5.f));
	}

	void GameplayState::update(const sf::Time &dt)
	{
		root->update(*window, dt, glm::mat4());

		hud.update(dt);

		window->setMouseCursorVisible(false);
		// Reset mouse to middle of screen
		sf::Mouse::setPosition(sf::Vector2i((int)(window->getSize().x) / 2, (int)(window->getSize().y) / 2), *window);
	}

	void GameplayState::draw()
	{
		// Clear the color / depth buffer
		glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		root->draw(currentLevel->cam->getGlobalPosition(), currentLevel->cam->view, projection, currentLevel->lightPos);

		hud.draw();
	}

	void GameplayState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		case sf::Keyboard::Space:
			player->jump();
			break;
		case sf::Keyboard::E:
		{
			if (!player->jumping && player->gold >= 50)
			{
				player->gold -= 50;

				glm::vec3 front = player->getGlobalFront();
				Tower* t = Tower::createTower(TowerType::Arrow, player->getGlobalPosition() + glm::vec3(front.x, 0.f, front.z)  * -30.f);
				this->currentLevel->attach(t);
				FSoundManager::getSoundManager()->clank();
			}
			else
			{
				// TODO play error sound, flash gold on ui
			}
		}
			break;
		default:
			break;
		}

		hud.keyPressed(e);
	}

	void GameplayState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
		{
			glm::vec3 pos = player->getGlobalPosition() + glm::vec3(0.f, 3.f * player->mesh->getHeight() / 4.f, 0.f);
			Projectile* p = new Projectile(pos, pos + player->getGlobalFront() * -1500.f, 5);
			this->currentLevel->attach(p);
			FSoundManager::getSoundManager()->pew();
		}
			break;
		default:
			break;
		}

		hud.mouseButtonPressed(e);
	}

	void GameplayState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		default:
			break;
		}
	}

	void GameplayState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
	}

	void GameplayState::setFieldOfView(float degrees)
	{
		projection = glm::perspective(degrees, 1024.f / 768.f, 0.1f, 100000.0f);
	}
}