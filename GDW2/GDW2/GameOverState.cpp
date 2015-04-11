#include "GameOverState.h"
#include "Game.h"
#include <iostream>

#define SCALE_CONSTANT 0.75f

namespace flopse
{
	GameOverState::GameOverState(sf::RenderWindow* window) : window(window), buttonSprite("GameOverText")
	{
		if (!bgTexture.loadFromFile("textures/GameOverBackground.png"))
		{
			std::cout << "ERROR LOADING textures/GameOverBackground.png" << std::endl;
		}

		bgSprite = new sf::Sprite(bgTexture);

		scaleChanged();

		createButtons();
	}

	GameOverState::~GameOverState()
	{
		delete bgSprite;
	}

	void GameOverState::createButtons()
	{
		SpriteFrame currentFrame;

		currentFrame = buttonSprite.getFrame("Menu.png");
		mainMenuBtn.rect = currentFrame.rect;
		mainMenuBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("MenuSelected.png");
		mainMenuBtn.pressedRect = currentFrame.rect;
		mainMenuBtn.pressedName = currentFrame.name;
		mainMenuBtn.pos = sf::Vector2i((window->getSize().x / 3.f - mainMenuBtn.rect.width / 2.f), window->getSize().y * 2.f / 3.f);
		buttons.push_back(mainMenuBtn);

		currentFrame = buttonSprite.getFrame("Restart.png");
		retryBtn.rect = currentFrame.rect;
		retryBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("RestartSelected.png");
		retryBtn.pressedRect = currentFrame.rect;
		retryBtn.pressedName = currentFrame.name;
		retryBtn.pos = sf::Vector2i((window->getSize().x * 2.f / 3.f - retryBtn.rect.width / 2.f), window->getSize().y * 2.f / 3.f);
		buttons.push_back(retryBtn);
	}

	void GameOverState::buttonClicked(const std::string &name)
	{
		if (name == mainMenuBtn.name)
		{
			Game::getGame()->shouldDeleteGameplayState = true;
			Game::getGame()->setMainMenuState();
		}
		else if (name == retryBtn.name)
		{
			Game::getGame()->nextLevel = true;
			Game::getGame()->setLoadingState();
		}
	}

	void GameOverState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		if (xScale != bgSprite->getScale().x || yScale != bgSprite->getScale().y)
		{
			this->buttonSprite.setScale(SCALE_CONSTANT * xScale, SCALE_CONSTANT * yScale);
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	
			this->createButtons();
		}
	}

	void GameOverState::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		window->pushGLStates();

		window->clear(sf::Color::White);

		window->draw(*bgSprite);

		sf::Sprite* s = buttonSprite.getSprite();
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].state == ButtonState::Normal)
			{
				buttonSprite.setToFrame(buttons[i].name);
			}
			else if (buttons[i].state == ButtonState::Pressed)
			{
				buttonSprite.setToFrame(buttons[i].pressedName);
			}

			buttonSprite.setPosition(buttons[i].pos);
			window->draw(*s);
		}

		window->popGLStates();
	}

	void GameOverState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		default:
			break;
		}
	}

	void GameOverState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
			for (int i = 0; i < buttons.size(); i++)
			{
				if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite.getSprite()->getScale().x) &&
					e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite.getSprite()->getScale().y))
				{
					buttons[i].state = ButtonState::Pressed;
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	void GameOverState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
			for (int i = 0; i < buttons.size(); i++)
			{
				if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite.getSprite()->getScale().x) &&
					e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite.getSprite()->getScale().y))
				{
					if (buttons[i].state == ButtonState::Pressed)
					{
						buttonClicked(buttons[i].name);
					}
				}

				buttons[i].state = ButtonState::Normal;
			}
			break;
		default:
			break;
		}
	}

	void GameOverState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
	}

	void GameOverState::scaleChanged()
	{
		float xScale, yScale;

		if (window->getSize().x <= window->getSize().y)
		{
			xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
			yScale = xScale;
		}
		else
		{
			yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);
			xScale = yScale;
		}

		bgSprite->setScale(sf::Vector2f(xScale, yScale));
		buttonSprite.setScale(SCALE_CONSTANT * xScale, SCALE_CONSTANT * yScale);
	}
}