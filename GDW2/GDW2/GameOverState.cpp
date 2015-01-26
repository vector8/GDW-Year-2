#include "GameOverState.h"
#include "Game.h"
#include <iostream>

namespace flopse
{
	GameOverState::GameOverState(sf::RenderWindow* window) : window(window)
	{
		if (!bgTexture.loadFromFile("textures/GameOverBackground.png"))
		{
			std::cout << "ERROR LOADING textures/GameOverBackground.png" << std::endl;
		}

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		bgSprite = new sf::Sprite(bgTexture);
		bgSprite->setScale(sf::Vector2f(xScale, yScale));
	}

	void GameOverState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		if (xScale != bgSprite->getScale().x || yScale != bgSprite->getScale().y)
		{
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
		}
	}

	void GameOverState::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		window->pushGLStates();

		window->clear(sf::Color::White);

		window->draw(*bgSprite);

		window->popGLStates();
	}

	void GameOverState::keyPressed(sf::Event::KeyEvent e)
	{
		Game::getGame()->setMainMenuState();
	}

	void GameOverState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
	}

	void GameOverState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
		Game::getGame()->setMainMenuState();
	}

	void GameOverState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
	}
}