#pragma once
#include "State.h"
#include <vector>
#include <string>
#include "Button.h"
#include "SpriteWithAtlas.h"

namespace flopse
{
	struct Slider
	{
		sf::Vector2i pos;
		sf::IntRect rect;
		std::string name;
	};

	class OptionsMenuState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		SpriteWithAtlas buttonSprite;
		std::vector<Button> buttons;

		Slider volumeSlider;
		Button volumeSliderTab;

		Slider fovSlider;
		Button fovSliderTab;

		void createButtons();
		void buttonClicked(std::string name);

	public:
		OptionsMenuState(sf::RenderWindow* window);
		virtual ~OptionsMenuState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);
	};
}