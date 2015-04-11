#include "MainMenuState.h"
#include "Game.h"
#include <iostream>

#define SCALE_CONSTANT 0.28f

namespace flopse
{
	MainMenuState::MainMenuState(sf::RenderWindow* window) : window(window), buttonSprite("MenuButtons")
	{
		bgMusic = new Sound("sounds/Level Music/Teller of the Tales.wav", true, false);

		if (!bgTexture.loadFromFile("textures/MenuBackGround.png"))
		{
			std::cout << "ERROR LOADING textures/MenuBackGround.png" << std::endl;
		}

		bgSprite = new sf::Sprite(bgTexture);

		scaleChanged();

		createButtons();
	}
	
	MainMenuState::~MainMenuState()
	{
		delete bgSprite;
	}

	void MainMenuState::createButtons()
	{
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

		int x = window->getSize().x - currentFrame.rect.width * buttonSprite.getSprite()->getScale().x - 100;
		int y = 50;
		int yOffset = 10;

		contBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(contBtn);
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

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
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		currentFrame = buttonSprite.getFrame("SaveButton.png");
		saveBtn.rect = currentFrame.rect;
		saveBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("SaveSelected.png");
		saveBtn.hoverRect = currentFrame.rect;
		saveBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("SavePressed.png");
		saveBtn.pressedRect = currentFrame.rect;
		saveBtn.pressedName = currentFrame.name;
		saveBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(saveBtn);
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

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
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

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
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		currentFrame = buttonSprite.getFrame("CreditsButton.png");
		creditsBtn.rect = currentFrame.rect;
		creditsBtn.name = currentFrame.name;
		currentFrame = buttonSprite.getFrame("CreditsSelected.png");
		creditsBtn.hoverRect = currentFrame.rect;
		creditsBtn.hoverName = currentFrame.name;
		currentFrame = buttonSprite.getFrame("CreditsPressed.png");
		creditsBtn.pressedRect = currentFrame.rect;
		creditsBtn.pressedName = currentFrame.name;
		creditsBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(creditsBtn);
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

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
			Game::getGame()->setLoadingState();
		}
		else if (name == newBtn.name)
		{
			Game::getGame()->newGame();
		}
		else if (name == saveBtn.name)
		{
			Game::getGame()->saveGame();
		}
		else if (name == loadBtn.name)
		{
			Game::getGame()->loadGame();
		}
		else if (name == optionsBtn.name)
		{
			Game::getGame()->setOptionsMenuState();
		}
		else if (name == creditsBtn.name)
		{
			Game::getGame()->setCreditsState();
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
			buttonSprite.setScale(SCALE_CONSTANT * ((float)window->getSize().x / 1920.f), SCALE_CONSTANT * ((float)window->getSize().y / 1080.f));
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	
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

			buttonSprite.setPosition(buttons[i].pos);
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
	void MainMenuState::startBackgroundMusic()
	{
		if (bgMusic != nullptr)
		{
			bgMusic->play();
		}
	}
	void MainMenuState::stopBackgroundMusic()
	{
		if (bgMusic != nullptr)
		{
			bgMusic->setPaused(true);
		}
	}

	void MainMenuState::scaleChanged()
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
		buttonSprite.setScale(SCALE_CONSTANT * ((float)window->getSize().x / 1920.f), SCALE_CONSTANT * ((float)window->getSize().y / 1080.f));
	}
}