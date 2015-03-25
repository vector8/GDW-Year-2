#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include "Game.h"

#include <iostream>
#include <chrono>
#include <thread>

#include "SoundManager.h"
#include "Utilities.h"

namespace flopse
{
	Game::Game() : window(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Garrison", sf::Style::Fullscreen)), running(true), frames(0), fullscreen(false)
	{
		//window->setVerticalSyncEnabled(true);		// These seem to reduce framerate dramatically after several minutes, even though nothing actually changes in the scene.
		//window->setFramerateLimit(60);			// Not really sure why...

		glewExperimental = GL_TRUE;
		glewInit();
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		glEnable(GL_DEPTH_TEST);

		setMainMenuState();
		
		// Seed the random number generator so other classes don't have to.
		seedRandomNumberGenerator();
	}

	Game::~Game()
	{
		delete gameplayState;
		delete loadingState;
		delete mainMenuState;
		delete optionsMenuState;
		delete gameOverState;
		delete window;
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
							/*if (fullscreen)
							{
								window->create(sf::VideoMode(1024, 768), "Garrison");
								fullscreen = false;
							}
							else
							{
								window->create(sf::VideoMode::getDesktopMode(), "Garrison", sf::Style::Fullscreen);
								fullscreen = true;
							}*/
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

				SoundManager::getSoundManager()->update();

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

	std::shared_ptr<Player> Game::getPlayer() const
	{
		return gameplayState->player;
	}

	std::shared_ptr<Level> Game::getCurrentLevel() const
	{
		return gameplayState->currentLevel;
	}

	std::shared_ptr<Camera> Game::getCamera() const
	{
		return gameplayState->cam;
	}

	std::vector<BoundingBox> Game::getColliders() const
	{
		return gameplayState->currentLevel->colliders;
	}

	std::vector<BoundingBox> Game::getTowerBlockers() const
	{
		return gameplayState->currentLevel->towerBlockers;
	}

	std::vector<std::shared_ptr<Enemy>> Game::getEnemies() const
	{
		return gameplayState->currentLevel->enemies;
	}

	void Game::newGame()
	{
		shouldDeleteGameplayState = true;
		setLoadingState();
	}

	void Game::setGameplayState()
	{
		if (this->currentState != nullptr && (this->currentState == this->loadingState || this->currentState == this->mainMenuState))
		{
			this->mainMenuState->stopBackgroundMusic();
		}

		if (shouldDeleteGameplayState)
		{
			delete this->gameplayState;
			this->gameplayState = nullptr;
			shouldDeleteGameplayState = false;
		}

		if (this->gameplayState == nullptr)
		{
			this->gameplayState = new GameplayState(window);
		}

		if (nextLevel)
		{
			this->gameplayState->currentLevel = Level::createLevel(this->gameplayState->levelNumber, this->gameplayState->player);
			this->gameplayState->root = this->gameplayState->currentLevel;
			nextLevel = false;
		}

		this->currentState = this->gameplayState;
		window->setMouseCursorVisible(false);
		// Reset mouse to middle of screen
		sf::Mouse::setPosition(sf::Vector2i((int)(window->getSize().x) / 2, (int)(window->getSize().y) / 2), *window);
		this->gameplayState->currentLevel->startBackgroundMusic();
		clock.restart();
	}

	void Game::setLoadingState()
	{
		if (this->loadingState == nullptr)
		{
			this->loadingState = new LoadingState(window);
		}

		this->currentState = this->loadingState;
	}

	void Game::setMainMenuState()
	{
		if (this->currentState != nullptr && this->currentState == this->gameplayState)
		{
			this->gameplayState->currentLevel->stopBackgroundMusic();
		}

		if (this->mainMenuState == nullptr)
		{
			this->mainMenuState = new MainMenuState(window);
		}

		this->currentState = this->mainMenuState;
		this->mainMenuState->startBackgroundMusic();
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
		if (this->currentState != nullptr && this->currentState == this->gameplayState)
		{

			this->gameplayState->currentLevel->stopBackgroundMusic();
		}

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
		if (this->gameplayState != nullptr)
		{
			if (gameplayState->currentLevel->gateHealth <= 0)
			{
				shouldDeleteGameplayState = true;
				this->setGameOverState();
			}
			else if (gameplayState->currentLevel->enemyCount < 1)
			{
				this->gameplayState->levelNumber++;
				if (this->gameplayState->levelNumber > 5)
				{
					shouldDeleteGameplayState = true;
					setMainMenuState();
				}
				else
				{
					nextLevel = true;
					setLoadingState();
				}
				clock.restart();
			}
		}
	}

	void Game::toggleFullscreen()
	{
		/*if (fullscreen)
		{
			window->create(sf::VideoMode(1024, 768), "Garrison");

			glewExperimental = GL_TRUE;
			glewInit();
			glViewport(0, 0, window->getSize().x, window->getSize().y);
			glEnable(GL_DEPTH_TEST);
			fullscreen = false;
		}
		else
		{
			window->create(sf::VideoMode::getDesktopMode(), "Garrison", sf::Style::Fullscreen);

			glewExperimental = GL_TRUE;
			glewInit();
			glViewport(0, 0, window->getSize().x, window->getSize().y);
			glEnable(GL_DEPTH_TEST);
			fullscreen = true;
		}*/
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