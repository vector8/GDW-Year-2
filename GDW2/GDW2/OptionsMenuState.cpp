#include "OptionsMenuState.h"
#include "Game.h"
#include <iostream>
#include "SoundManager.h"

namespace flopse
{
	OptionsMenuState::OptionsMenuState(sf::RenderWindow* window) : window(window), buttonSprite("Options")
	{
		if (!bgTexture.loadFromFile("textures/MainMenuBackground.png"))
		{
			std::cout << "ERROR LOADING textures/MainMenuBackground.png" << std::endl;
		}

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);
		
		bgSprite = new sf::Sprite(bgTexture);
		bgSprite->setScale(sf::Vector2f(xScale, yScale));

		buttonSprite.setScale(xScale, yScale);

		createButtons();
	}

	OptionsMenuState::~OptionsMenuState()
	{
		delete bgSprite;
	}

	void OptionsMenuState::createButtons()
	{
		int x = 256 * buttonSprite.getSprite()->getScale().x;
		int y = 256 * buttonSprite.getSprite()->getScale().y;
		int yOffset = 10 * buttonSprite.getSprite()->getScale().y;
		int xOffset = 50 * buttonSprite.getSprite()->getScale().x;
		SpriteFrame currentFrame;

		Button keyMapping;
		currentFrame = buttonSprite.getFrame("KeyMapping.png");
		keyMapping.rect = currentFrame.rect;
		keyMapping.name = currentFrame.name;
		keyMapping.pos = sf::Vector2i(x, y);
		buttons.push_back(keyMapping);
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		Button volume;
		currentFrame = buttonSprite.getFrame("MasterVolume.png");
		volume.rect = currentFrame.rect;
		volume.name = currentFrame.name;
		volume.pos = sf::Vector2i(x, y);
		buttons.push_back(volume);
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

		Button fullscreen;
		currentFrame = buttonSprite.getFrame("FullScreen.png");
		fullscreen.rect = currentFrame.rect;
		fullscreen.name = currentFrame.name;
		fullscreen.pos = sf::Vector2i(x, y);
		buttons.push_back(fullscreen);

		tempY = y;
		tempHeight = currentFrame.rect.height;
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		Button fullscreenOn;
		currentFrame = buttonSprite.getFrame("FullScreenOn.png");
		fullscreenOn.rect = currentFrame.rect;
		fullscreenOn.name = currentFrame.name;
		y2 = tempY + ((tempHeight / 2) - (currentFrame.rect.height / 2)) * buttonSprite.getSprite()->getScale().y;
		fullscreenOn.pos = sf::Vector2i(x2, y2);
		fullscreenOn.visible = Game::getGame()->isFullscreen();
		buttons.push_back(fullscreenOn);

		Button fullscreenOff;
		currentFrame = buttonSprite.getFrame("FullScreenOff.png");
		fullscreenOff.rect = currentFrame.rect;
		fullscreenOff.name = currentFrame.name;
		fullscreenOff.pos = sf::Vector2i(x2, y2);
		fullscreenOff.visible = !Game::getGame()->isFullscreen();
		buttons.push_back(fullscreenOff);

		Button mouseSensitivity;
		currentFrame = buttonSprite.getFrame("MouseSensitivity.png");
		mouseSensitivity.rect = currentFrame.rect;
		mouseSensitivity.name = currentFrame.name;
		mouseSensitivity.pos = sf::Vector2i(x, y);
		buttons.push_back(mouseSensitivity);
		y += currentFrame.rect.height * buttonSprite.getSprite()->getScale().y + yOffset;

		Button fov;
		currentFrame = buttonSprite.getFrame("FOV.png");
		fov.rect = currentFrame.rect;
		fov.name = currentFrame.name;
		fov.pos = sf::Vector2i(x, y);
		buttons.push_back(fov);
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
		backBtn.pos = sf::Vector2i(window->getSize().x - ((256 + backBtn.rect.width) * buttonSprite.getSprite()->getScale().x), window->getSize().y - ((256 + backBtn.rect.height) * buttonSprite.getSprite()->getScale().y));
		buttons.push_back(backBtn);
	}

	void OptionsMenuState::buttonClicked(std::string name)
	{
		if (name == "KeyMapping.png")
		{
			// have a keymapping state later..
		}
		else if (name == "FullScreenOn.png")
		{
			buttons[3].visible = false;
			buttons[4].visible = true;

			Game::getGame()->toggleFullscreen();
		}
		else if (name == "FullScreenOff.png")
		{
			buttons[3].visible = true;
			buttons[4].visible = false;
			Game::getGame()->toggleFullscreen();
		}
		else if (name == "backbutton.png")
		{
			Game::getGame()->setMainMenuState();
		}
	}

	void OptionsMenuState::update(const sf::Time &dt)
	{
		window->setMouseCursorVisible(true);

		float xScale = (float)(window->getSize().x) / (float)(bgTexture.getSize().x);
		float yScale = (float)(window->getSize().y) / (float)(bgTexture.getSize().y);

		if (xScale != buttonSprite.getSprite()->getScale().x || yScale != buttonSprite.getSprite()->getScale().y)
		{
			this->buttonSprite.setScale(xScale, yScale);
			this->bgSprite->setScale(sf::Vector2f(xScale, yScale));
			this->buttons.clear();	// TODO maybe not clear?
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
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i].visible)
			{
				buttonSprite.setToFrame(buttons[i].name);
				buttonSprite.setPosition(buttons[i].pos.x, buttons[i].pos.y);
				window->draw(*s);
			}
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
}