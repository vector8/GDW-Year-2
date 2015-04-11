#include "OptionsMenuState.h"
#include "Game.h"
#include <iostream>
#include "SoundManager.h"

#define SCALE_CONSTANT 0.4f

namespace flopse
{
	OptionsMenuState::OptionsMenuState(sf::RenderWindow* window) : window(window), buttonSprite("Options")
	{
		if (!bgTexture.loadFromFile("textures/MenuBackGround.png"))
		{
			std::cout << "ERROR LOADING textures/MenuBackGround.png" << std::endl;
		}

		bgSprite = new sf::Sprite(bgTexture);

		scaleChanged();

		createButtons();
	}

	OptionsMenuState::~OptionsMenuState()
	{
		delete bgSprite;
	}

	void OptionsMenuState::createButtons()
	{
		int x = 256 * buttonSprite.getSprite()->getScale().x;
		int y = 600 * buttonSprite.getSprite()->getScale().y;
		int yOffset = 100;
		int xOffset = 50 * buttonSprite.getSprite()->getScale().x;
		SpriteFrame currentFrame;

		UIElement volume;
		currentFrame = buttonSprite.getFrame("MasterVolume.png");
		volume.rect = currentFrame.rect;
		volume.name = currentFrame.name;
		volume.pos = sf::Vector2i(x, y);
		staticElements.push_back(volume);
		int tempY = y;
		int tempHeight = currentFrame.rect.height;
		int x2 = x + currentFrame.rect.width * buttonSprite.getSprite()->getScale().x + xOffset;
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		currentFrame = buttonSprite.getFrame("Slider.png");
		volumeSlider.rect = currentFrame.rect;
		volumeSlider.name = currentFrame.name;
		int	y2 = tempY + ((tempHeight / 2) - (currentFrame.rect.height / 2)) * buttonSprite.getSprite()->getScale().y;
		tempHeight = currentFrame.rect.height;
		volumeSlider.pos = sf::Vector2i(x2, y2);

		currentFrame = buttonSprite.getFrame("SliderTab.png");
		volumeSliderTab.rect = currentFrame.rect;
		volumeSliderTab.name = currentFrame.name;
		volumeSliderTab.pos = sf::Vector2i(x2 + ((volumeSlider.rect.width * SoundManager::getSoundManager()->getMasterVolume()) - (volumeSliderTab.rect.width / 2)) * buttonSprite.getSprite()->getScale().x, 
			y2 + ((tempHeight / 2) - (currentFrame.rect.height / 2)) * buttonSprite.getSprite()->getScale().y);

		UIElement fov;
		currentFrame = buttonSprite.getFrame("FOV.png");
		fov.rect = currentFrame.rect;
		fov.name = currentFrame.name;
		fov.pos = sf::Vector2i(x, y);
		staticElements.push_back(fov);
		tempY = y;
		tempHeight = currentFrame.rect.height;
		x2 = x + currentFrame.rect.width * buttonSprite.getSprite()->getScale().x + xOffset;
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		currentFrame = buttonSprite.getFrame("Slider.png");
		fovSlider.rect = currentFrame.rect;
		fovSlider.name = currentFrame.name;
		y2 = tempY + ((tempHeight / 2) - (currentFrame.rect.height / 2)) * buttonSprite.getSprite()->getScale().y;
		tempHeight = currentFrame.rect.height;
		fovSlider.pos = sf::Vector2i(x2, y2);

		currentFrame = buttonSprite.getFrame("SliderTab.png");
		fovSliderTab.rect = currentFrame.rect;
		fovSliderTab.name = currentFrame.name;
		fovSliderTab.pos = sf::Vector2i(x2 + ((fovSlider.rect.width / 2) - (fovSliderTab.rect.width / 2)) * buttonSprite.getSprite()->getScale().x, 
			y2 + ((tempHeight / 2) - (currentFrame.rect.height / 2)) * buttonSprite.getSprite()->getScale().y);

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

	void OptionsMenuState::buttonClicked(std::string name)
	{
		if (name == "backbutton.png")
		{
			Game::getGame()->setMainMenuState();
		}
	}

	void OptionsMenuState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		if (xScale != bgSprite->getScale().x || yScale != bgSprite->getScale().y)
		{
			buttonSprite.setScale(SCALE_CONSTANT * ((float)window->getSize().x / 1920.f), SCALE_CONSTANT * ((float)window->getSize().y / 1080.f));
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	
			this->staticElements.clear();
			this->createButtons();
		}
	}

	void OptionsMenuState::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		window->pushGLStates();

		window->clear(sf::Color::White);

		window->draw(*bgSprite);

		sf::Sprite* s = buttonSprite.getSprite();
		for (int i = 0; i < staticElements.size(); i++)
		{
			if (staticElements[i].visible)
			{
				buttonSprite.setToFrame(staticElements[i].name);
				buttonSprite.setPosition(staticElements[i].pos);
				window->draw(*s);
			}
		}

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

		buttonSprite.setToFrame(volumeSlider.name);
		buttonSprite.setPosition(volumeSlider.pos.x, volumeSlider.pos.y);
		window->draw(*s);

		buttonSprite.setToFrame(volumeSliderTab.name);
		buttonSprite.setPosition(volumeSliderTab.pos.x, volumeSliderTab.pos.y);
		window->draw(*s);

		buttonSprite.setToFrame(fovSlider.name);
		buttonSprite.setPosition(fovSlider.pos.x, fovSlider.pos.y);
		window->draw(*s);

		buttonSprite.setToFrame(fovSliderTab.name);
		buttonSprite.setPosition(fovSliderTab.pos.x, fovSliderTab.pos.y);
		window->draw(*s);

		window->popGLStates();
	}

	void OptionsMenuState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		default:
			break;
		}
	}

	void OptionsMenuState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
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

			if (e.x > volumeSliderTab.pos.x && e.x < volumeSliderTab.pos.x + (volumeSliderTab.rect.width * buttonSprite.getSprite()->getScale().x) &&
				e.y > volumeSliderTab.pos.y && e.y < volumeSliderTab.pos.y + (volumeSliderTab.rect.height * buttonSprite.getSprite()->getScale().y))
			{
				volumeSliderTab.state = ButtonState::Pressed;
			}

			if (e.x > fovSliderTab.pos.x && e.x < fovSliderTab.pos.x + (fovSliderTab.rect.width * buttonSprite.getSprite()->getScale().x) &&
				e.y > fovSliderTab.pos.y && e.y < fovSliderTab.pos.y + (fovSliderTab.rect.height * buttonSprite.getSprite()->getScale().y))
			{
				fovSliderTab.state = ButtonState::Pressed;
			}

			break;
		default:
			break;
		}
	}

	void OptionsMenuState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
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

			if (volumeSliderTab.state == ButtonState::Pressed)
			{
				float xScale = buttonSprite.getSprite()->getScale().x;
				float halfTabWidth = (volumeSliderTab.rect.width * xScale) / 2.f;
				float sliderWidth = volumeSlider.rect.width * xScale;

				float volume = (float)(volumeSliderTab.pos.x + halfTabWidth - volumeSlider.pos.x) / sliderWidth;
				SoundManager::getSoundManager()->setMasterVolume(volume);

				volumeSliderTab.state = ButtonState::Normal;
			}
			else if (fovSliderTab.state == ButtonState::Pressed)
			{
				float xScale = buttonSprite.getSprite()->getScale().x;
				float halfTabWidth = (fovSliderTab.rect.width * xScale) / 2.f;
				float sliderWidth = fovSlider.rect.width * xScale;

				float fov = 45.f + ((float)(fovSliderTab.pos.x + halfTabWidth - fovSlider.pos.x) / sliderWidth) * 75.f;
				Game::getGame()->setFieldOfView(fov);

				fovSliderTab.state = ButtonState::Normal;
			}
			break;
		default:
			break;
		}
	}

	void OptionsMenuState::mouseMoved(sf::Event::MouseMoveEvent e)
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

		if (volumeSliderTab.state == ButtonState::Pressed)
		{
			int posx = 0;
			int sliderWidth = volumeSlider.rect.width * buttonSprite.getSprite()->getScale().x;
			int sliderTabWidth = volumeSliderTab.rect.width * buttonSprite.getSprite()->getScale().x;

			if (e.x > volumeSlider.pos.x + sliderWidth)
			{
				posx = volumeSlider.pos.x + sliderWidth;
			}
			else if (e.x < volumeSlider.pos.x)
			{
				posx = volumeSlider.pos.x;
			}
			else
			{
				posx = e.x;
			}

			volumeSliderTab.pos.x = posx - sliderTabWidth / 2;
		}
		else if (fovSliderTab.state == ButtonState::Pressed)
		{
			int posx = 0;
			int sliderWidth = fovSlider.rect.width * buttonSprite.getSprite()->getScale().x;
			int sliderTabWidth = fovSliderTab.rect.width * buttonSprite.getSprite()->getScale().x;

			if (e.x > fovSlider.pos.x + sliderWidth)
			{
				posx = fovSlider.pos.x + sliderWidth;
			}
			else if (e.x < fovSlider.pos.x)
			{
				posx = fovSlider.pos.x;
			}
			else
			{
				posx = e.x;
			}

			fovSliderTab.pos.x = posx - sliderTabWidth / 2;
		}
	}

	void OptionsMenuState::scaleChanged()
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