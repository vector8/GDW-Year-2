#pragma once
#include "State.h"
#include <vector>
#include <string>
#include "UIElement.h"
#include "SpriteWithAtlas.h"

namespace flopse
{
	class CreditsState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		SpriteWithAtlas buttonSprite;
		std::vector<Button> buttons;

		void createButtons();
		void buttonClicked(std::string name);

	public:
		CreditsState(sf::RenderWindow* window);
		virtual ~CreditsState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);

		void scaleChanged();
	};
}