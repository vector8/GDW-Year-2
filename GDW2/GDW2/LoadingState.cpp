#include "LoadingState.h"

#include <iostream>

#include "Game.h"

namespace flopse
{
	LoadingState::LoadingState(sf::RenderWindow* window) : window(window)
	{
		if (!bgTexture.loadFromFile("textures/LoadingScreen.png"))
		{
			std::cout << "ERROR LOADING textures/LoadingScreen.png" << std::endl;
		}

		bgSprite = new sf::Sprite(bgTexture);

		float xScale, yScale;

		xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		bgSprite->setScale(sf::Vector2f(xScale, yScale));
	}

	LoadingState::~LoadingState()
	{
		delete bgSprite;
	}

	void LoadingState::update(const sf::Time &dt)
	{
		if (firstPass)
		{
			firstPass = false;
		}
		else
		{
			Game::getGame()->setGameplayState();
			firstPass = true;
		}
	}

	void LoadingState::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		window->pushGLStates();

		window->clear(sf::Color::White);

		window->draw(*bgSprite);

		window->popGLStates();
	}

	void LoadingState::keyPressed(sf::Event::KeyEvent e)
	{
	}

	void LoadingState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
	}

	void LoadingState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
	}

	void LoadingState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
	}
}