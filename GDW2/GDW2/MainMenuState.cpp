#include "MainMenuState.h"
#include "Game.h"
#include <iostream>

#define SCALE_CONSTANT 0.75f

namespace flopse
{
	MainMenuState::MainMenuState(sf::RenderWindow* window) : window(window), buttonSprite("MenuButtons")
	{
		if (!bgTexture.loadFromFile("textures/MenuBackGround.png"))
		{
			std::cout << "ERROR LOADING textures/MenuBackGround.png" << std::endl;
		}

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		bgSprite = new sf::Sprite(bgTexture);
		bgSprite->setScale(sf::Vector2f(xScale, yScale));

		buttonSprite.setScale(SCALE_CONSTANT * xScale, SCALE_CONSTANT * yScale);

		createButtons();
	}
	
	MainMenuState::~MainMenuState()
	{
		delete bgSprite;
	}

	void MainMenuState::createButtons()
	{
		int x = 3000 * bgSprite->getScale().x;
		int exitX = 3310 * bgSprite->getScale().x;
		int y = 300 * bgSprite->getScale().y;
		const int Y_OFFSET = 620 * buttonSprite.getSprite()->getScale().y;
		SpriteFrame currentFrame;

		currentFrame = buttonSprite.getFrame("ContinueButton.png");
		contBtn.rect = currentFrame.rect;
		contBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("ContinueSelected.png");
		contBtn.hoverRect = currentFrame.rect;
		contBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("ContinuePressed.png");
		contBtn.pressedRect = currentFrame.rect;
		contBtn.pressedName = currentFrame.name;
		contBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(contBtn);
		y += Y_OFFSET;

		currentFrame = buttonSprite.getFrame("NewGameButton.png");
		newBtn.rect = currentFrame.rect;
		newBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("NewGameSelected.png");
		newBtn.hoverRect = currentFrame.rect;
		newBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("NewGamePressed.png");
		newBtn.pressedRect = currentFrame.rect;
		newBtn.pressedName = currentFrame.name;
		newBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(newBtn);
		y += Y_OFFSET;

		currentFrame = buttonSprite.getFrame("LoadGameButton.png");
		loadBtn.rect = currentFrame.rect;
		loadBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("LoadGameSelected.png");
		loadBtn.hoverRect = currentFrame.rect;
		loadBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("LoadGamePressed.png");
		loadBtn.pressedRect = currentFrame.rect;
		loadBtn.pressedName = currentFrame.name;
		loadBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(loadBtn);
		y += Y_OFFSET;

		currentFrame = buttonSprite.getFrame("OptionsButton.png");
		optionsBtn.rect = currentFrame.rect;
		optionsBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("OptionsSelected.png");
		optionsBtn.hoverRect = currentFrame.rect;
		optionsBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("OptionsPressed.png");
		optionsBtn.pressedRect = currentFrame.rect;
		optionsBtn.pressedName = currentFrame.name;
		optionsBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(optionsBtn);
		y += Y_OFFSET;

		currentFrame = buttonSprite.getFrame("CreditButton.png");
		creditsBtn.rect = currentFrame.rect;
		creditsBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("CreditSelected.png");
		creditsBtn.hoverRect = currentFrame.rect;
		creditsBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("CreditsPressed.png");
		creditsBtn.pressedRect = currentFrame.rect;
		creditsBtn.pressedName = currentFrame.name;
		creditsBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(creditsBtn);
		y += Y_OFFSET;

		currentFrame = buttonSprite.getFrame("ExitButton.png");
		exitBtn.rect = currentFrame.rect;
		exitBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("ExitSelected.png");
		exitBtn.hoverRect = currentFrame.rect;
		exitBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("ExitPressed.png");
		exitBtn.pressedRect = currentFrame.rect;
		exitBtn.pressedName = currentFrame.name;
		exitBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(exitBtn);
	}

	void MainMenuState::buttonClicked(const std::string &name)
	{
		if (name == contBtn.name)
		{
			Game::getGame()->setGameplayState();
		}
		else if (name == newBtn.name)
		{
			Game::getGame()->newGame();
		}
		else if (name == optionsBtn.name)
		{
			Game::getGame()->setOptionsMenuState();
		}
		else if (name == exitBtn.name)
		{
			Game::getGame()->exit();
		}
	}

	void MainMenuState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);
		
		if (xScale != bgSprite->getScale().x || yScale != bgSprite->getScale().y)
		{
			this->buttonSprite.setScale(SCALE_CONSTANT * xScale, SCALE_CONSTANT * yScale);
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	// TODO maybe not clear?
			this->createButtons();
		}
	}

	void MainMenuState::draw()
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

			buttonSprite.setPosition(buttons[i].pos.x, buttons[i].pos.y);
			window->draw(*s); 
		}

		window->popGLStates();
	}

	void MainMenuState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		default:
			break;
		}
	}

	void MainMenuState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
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

	void MainMenuState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
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

	void MainMenuState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
		for (int i = 0; i < buttons.size(); i++)
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