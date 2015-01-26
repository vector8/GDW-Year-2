#pragma once
#include "State.h"
#include <vector>
#include <string>
#include "Button.h"

namespace flopse
{
	class MainMenuState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture buttonTexture;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		sf::Sprite* buttonSprite;
		std::vector<Button> buttons;

		void createButtons();
		void buttonClicked(std::string name);

	public:
		MainMenuState(sf::RenderWindow* window);

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);
	};
}