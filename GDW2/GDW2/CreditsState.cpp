#include "CreditsState.h"
#include "Game.h"
#include <iostream>
#include "SoundManager.h"

#define SCALE_CONSTANT 0.3f

namespace flopse
{
	CreditsState::CreditsState(sf::RenderWindow* window) : window(window), buttonSprite("Options")
	{
		if (!bgTexture.loadFromFile("textures/CreditsScreen.png"))
		{
			std::cout << "ERROR LOADING textures/CreditsScreen.png" << std::endl;
		}

		bgSprite = new sf::Sprite(bgTexture);

		scaleChanged();

		createButtons();
	}

	CreditsState::~CreditsState()
	{
		delete bgSprite;
	}

	void CreditsState::createButtons()
	{
		SpriteFrame currentFrame;

		Button backBtn;
		currentFrame = buttonSprite.getFrame("backbutton.png");
		backBtn.rect = currentFrame.rect;
		backBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("backbuttonselected.png");
		backBtn.hoverRect = currentFrame.rect;
		backBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("backbuttonpressed.png");
		backBtn.pressedRect = currentFrame.rect;
		backBtn.pressedName = currentFrame.name;
		backBtn.pos = sf::Vector2i(window->getSize().x - ((256 + backBtn.rect.width) * buttonSprite.getSprite()->getScale().x), window->getSize().y - ((256 + backBtn.rect.height) * buttonSprite.getSprite()->getScale().y));
		buttons.push_back(backBtn);
	}

	void CreditsState::buttonClicked(std::string name)
	{
		if (name == "backbutton.png")
		{
			Game::getGame()->setMainMenuState();
		}
	}

	void CreditsState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		if (xScale != bgSprite->getScale().x || yScale != bgSprite->getScale().y)
		{
			this->buttonSprite.setScale(SCALE_CONSTANT, SCALE_CONSTANT);
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	
			this->createButtons();
		}
	}

	void CreditsState::draw()
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
			else if (buttons[i].state == ButtonState::Hover)
			{
				buttonSprite.setToFrame(buttons[i].hoverName);
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

	void CreditsState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		default:
			break;
		}
	}

	void CreditsState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].visible)
				{
					if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite.getSprite()->getScale().x) &&
						e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite.getSprite()->getScale().y))
					{
						buttons[i].state = ButtonState::Pressed;
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	void CreditsState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
			for (int i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].visible)
				{
					if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite.getSprite()->getScale().x) &&
						e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite.getSprite()->getScale().y))
					{
						if (buttons[i].state == ButtonState::Pressed)
						{
							buttonClicked(buttons[i].name);
						}
					}
				}

				buttons[i].state = ButtonState::Normal;
			}
			break;
		default:
			break;
		}
	}

	void CreditsState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].visible)
			{
				if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite.getSprite()->getScale().x) &&
					e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite.getSprite()->getScale().y))
				{
					if (buttons[i].state == ButtonState::Normal)
					{
						buttons[i].state = ButtonState::Hover;
					}
				}
				else if (buttons[i].state == ButtonState::Hover)
				{
					buttons[i].state = ButtonState::Normal;
				}
			}
		}
	}

	void CreditsState::scaleChanged()
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
		buttonSprite.setScale(SCALE_CONSTANT, SCALE_CONSTANT);
	}
}