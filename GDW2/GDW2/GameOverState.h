#pragma once
#include "State.h"
#include "UIElement.h"
#include "SpriteWithAtlas.h"

namespace flopse
{
	class GameOverState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		SpriteWithAtlas buttonSprite;
		std::vector<Button> buttons;
		Button mainMenuBtn;
		Button retryBtn;

		void createButtons();
		void buttonClicked(const std::string &name);

	public:
		GameOverState(sf::RenderWindow* window);
		virtual ~GameOverState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);

		void scaleChanged();
	};
}