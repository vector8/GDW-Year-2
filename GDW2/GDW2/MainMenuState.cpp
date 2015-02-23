#include "MainMenuState.h"
#include "Game.h"
#include <iostream>

namespace flopse
{
	MainMenuState::MainMenuState(sf::RenderWindow* window) : window(window)
	{
		if (!buttonTexture.loadFromFile("textures/MainMenuButtons.png"))
		{
			std::cout << "ERROR LOADING textures/MainMenuButtons.png" << std::endl;
		}

		if (!bgTexture.loadFromFile("textures/MainMenuBackground.png"))
		{
			std::cout << "ERROR LOADING textures/MainMenuBackground.png" << std::endl;
		}

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		buttonSprite = new sf::Sprite(buttonTexture);
		buttonSprite->setScale(sf::Vector2f(xScale, yScale));
		bgSprite = new sf::Sprite(bgTexture);
		bgSprite->setScale(sf::Vector2f(xScale, yScale));

		createButtons();
	}
	
	MainMenuState::~MainMenuState()
	{
		delete bgSprite;
		delete buttonSprite;
	}

	void MainMenuState::createButtons()
	{
		int left = 0;
		int top = 7;
		int width = 1352;
		int height = 238;
		int x = 2620 * buttonSprite->getScale().x;
		int exitX = 3310 * buttonSprite->getScale().x;
		int y = 514 * buttonSprite->getScale().y;
		const int Y_OFFSET = 424 * buttonSprite->getScale().y;

		Button contBtn;
		contBtn.rect = sf::IntRect(left, top, width, height);
		contBtn.name = "continue";
		contBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(contBtn);
		top += height;
		y += Y_OFFSET;

		Button newBtn;
		newBtn.rect = sf::IntRect(left, top, width, height);
		newBtn.name = "new";
		newBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(newBtn);
		top += height;
		y += Y_OFFSET;

		Button loadBtn;
		loadBtn.rect = sf::IntRect(left, top, width, height);
		loadBtn.name = "load";
		loadBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(loadBtn);
		top += height;
		y += Y_OFFSET;

		Button optionsBtn;
		optionsBtn.rect = sf::IntRect(left, top, width, height);
		optionsBtn.name = "options";
		optionsBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(optionsBtn);
		top += height;
		y += Y_OFFSET;

		Button creditsBtn;
		creditsBtn.rect = sf::IntRect(left, top, width, height);
		creditsBtn.name = "credits";
		creditsBtn.pos = sf::Vector2i(x, y);
		buttons.push_back(creditsBtn);
		top += height;
		y += Y_OFFSET;

		Button exitBtn;
		exitBtn.rect = sf::IntRect(1795, 1189, 756, 207);
		exitBtn.name = "exit";
		exitBtn.pos = sf::Vector2i(exitX, y);
		buttons.push_back(exitBtn);
	}

	void MainMenuState::buttonClicked(std::string name)
	{
		if (name == "continue")
		{
			Game::getGame()->setGameplayState();
		}
		else if (name == "new")
		{
			Game::getGame()->newGame();
		}
		else if (name == "options")
		{
			Game::getGame()->setOptionsMenuState();
		}
		else if (name == "exit")
		{
			Game::getGame()->exit();
		}
	}

	void MainMenuState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);
		
		if (xScale != buttonSprite->getScale().x || yScale != buttonSprite->getScale().y)
		{
			this->buttonSprite->setScale(sf::Vector2f(xScale, yScale));
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

		for (int i = 0; i < buttons.size(); i++)
		{
			buttonSprite->setTextureRect(sf::IntRect(buttons[i].rect.left + static_cast<int>(buttons[i].state) * buttons[i].rect.width, buttons[i].rect.top, buttons[i].rect.width, buttons[i].rect.height));
			buttonSprite->setPosition(buttons[i].pos.x, buttons[i].pos.y);
			window->draw(*buttonSprite);
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
				if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite->getScale().x) &&
					e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite->getScale().y))
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
				if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite->getScale().x) &&
					e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite->getScale().y))
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
			if (e.x > buttons[i].pos.x && e.x < buttons[i].pos.x + (buttons[i].rect.width * buttonSprite->getScale().x) &&
				e.y > buttons[i].pos.y && e.y < buttons[i].pos.y + (buttons[i].rect.height * buttonSprite->getScale().y))
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