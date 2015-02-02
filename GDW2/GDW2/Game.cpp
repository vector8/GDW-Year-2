#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include "Game.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "Tower.h"
#include "Projectile.h"
#include "FSoundManager.h"

namespace flopse
{
	Game::Game() : window(new sf::RenderWindow(sf::VideoMode(1024, 768), "Garrison", sf::Style::Close)), running(true), frames(0), fullscreen(false)
	{
		window->setVerticalSyncEnabled(true);
		window->setFramerateLimit(60);

		glewExperimental = GL_TRUE;
		glewInit();
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		glEnable(GL_DEPTH_TEST);

		gameplayState = nullptr;
		mainMenuState = new MainMenuState(window);
		optionsMenuState = nullptr;
		gameOverState = nullptr;
		currentState = mainMenuState;
	}

	void Game::run()
	{
		clock.restart();

		while (window->isOpen())
		{
			frames++;
			sf::Event event;
			while (window->pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window->close();
					running = false;
					break;
				case sf::Event::MouseMoved:
					currentState->mouseMoved(event.mouseMove);
					break;
				case sf::Event::MouseButtonPressed:
					currentState->mouseButtonPressed(event.mouseButton);
					break;
				case sf::Event::MouseButtonReleased:
					currentState->mouseButtonReleased(event.mouseButton);
					break;
				case sf::Event::KeyPressed:
					currentState->keyPressed(event.key);

					switch (event.key.code)
					{
					case sf::Keyboard::Escape:
						if (currentState == gameplayState)
						{
							setMainMenuState();
						}
						else if (currentState == mainMenuState)
						{
							setGameplayState();
						}
						else if (currentState == optionsMenuState)
						{
							setMainMenuState();
						}
						break;
					case sf::Keyboard::F4:
						if (event.key.alt)
						{
							window->close();
							running = false;
						}
						break;
					case sf::Keyboard::Return:
						if (event.key.alt)
						{
							if (fullscreen)
							{
								window->create(sf::VideoMode(1024, 768), "Garrison");
								fullscreen = false;
							}
							else
							{
								window->create(sf::VideoMode::getDesktopMode(), "Garrison", sf::Style::Fullscreen);
								fullscreen = true;
							}
						}
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

			if (running)
			{
				elapsed = clock.restart();

				fpsTimer += elapsed;

				// Update
				currentState->update(elapsed);

				FSoundManager::getSoundManager()->update();

				// Draw
				currentState->draw();

				// calculate current fps
				if (fpsTimer.asSeconds() > 1.f)
				{
					fpsTimer -= sf::seconds(1.0f);
					int fps = frames;
					frames = 0;

					std::cout << fps << std::endl;
				}

				// Swap the buffers
				window->display();

				// Sleep if less than 1/60 second
				/*if (elapsed.asSeconds() < (1.f / 60.f))
				{
				std::this_thread::sleep_for(std::chrono::milliseconds((long)(((1.f/60.f) - elapsed.asSeconds()) * 1000.f)));
				}*/
			}
		}
	}

	Player* Game::getPlayer() const
	{
		return gameplayState->player;
	}

	Level* Game::getCurrentLevel() const
	{
		return gameplayState->currentLevel;
	}

	Camera* Game::getCamera() const
	{
		return gameplayState->currentLevel->cam;
	}

	std::vector<Entity*> Game::getEntities() const
	{
		return gameplayState->currentLevel->entities;
	}

	std::vector<BoundingBox*> Game::getColliders() const
	{
		return gameplayState->currentLevel->colliders;
	}

	std::vector<Enemy*> Game::getEnemies() const
	{
		return gameplayState->currentLevel->enemies;
	}

	void Game::newGame()
	{
		window->setMouseCursorVisible(false);
		this->gameplayState = new GameplayState(window);
		this->currentState = this->gameplayState;
		clock.restart();
	}

	void Game::setGameplayState()
	{
		if (this->gameplayState == nullptr)
		{
			this->gameplayState = new GameplayState(window);
		}

		this->currentState = this->gameplayState;
		clock.restart();
	}

	void Game::setMainMenuState()
	{
		if (this->mainMenuState == nullptr)
		{
			this->mainMenuState = new MainMenuState(window);
		}

		this->currentState = this->mainMenuState;
	}

	void Game::setOptionsMenuState()
	{
		if (this->optionsMenuState == nullptr)
		{
			this->optionsMenuState = new OptionsMenuState(window);
		}

		this->currentState = this->optionsMenuState;
	}

	void Game::setGameOverState()
	{
		if (this->gameOverState == nullptr)
		{
			this->gameOverState = new GameOverState(window);
		}

		this->currentState = this->gameOverState;
	}

	void Game::exit()
	{
		window->close();
		running = false;
	}

	void Game::checkGameOver()
	{
		if (gameplayState->currentLevel->gateHealth <= 0)
		{
			this->currentState = this->gameOverState;
			this->gameplayState = nullptr;
		}
	}

	void Game::toggleFullscreen()
	{
		if (fullscreen)
		{
			window->create(sf::VideoMode(1024, 768), "Garrison");
			window->setVerticalSyncEnabled(true);
			window->setFramerateLimit(60);

			glewExperimental = GL_TRUE;
			glewInit();
			glViewport(0, 0, window->getSize().x, window->getSize().y);
			glEnable(GL_DEPTH_TEST);
			fullscreen = false;
		}
		else
		{
			window->create(sf::VideoMode::getDesktopMode(), "Garrison", sf::Style::Fullscreen);
			window->setVerticalSyncEnabled(true);
			window->setFramerateLimit(60);

			glewExperimental = GL_TRUE;
			glewInit();
			glViewport(0, 0, window->getSize().x, window->getSize().y);
			glEnable(GL_DEPTH_TEST);
			fullscreen = true;
		}
	}

	bool Game::isFullscreen()
	{
		return fullscreen;
	}

	void Game::setFieldOfView(float degrees)
	{
		this->fov = degrees;
		if (this->gameplayState != nullptr)
		{
			this->gameplayState->setFieldOfView(fov);
		}
	}

	float Game::getFieldOfView()
	{
		return fov;
	}
}