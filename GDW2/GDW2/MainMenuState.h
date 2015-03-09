#pragma once
#include "State.h"
#include <vector>
#include <string>
#include "Button.h"
#include "SpriteWithAtlas.h"

namespace flopse
{
	class MainMenuState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		SpriteWithAtlas buttonSprite;
		std::vector<Button> buttons;
		Button contBtn;
		Button newBtn;
		Button loadBtn;
		Button optionsBtn;
		Button creditsBtn;
		Button exitBtn;

		void createButtons();
		void buttonClicked(const std::string &name);

	public:
		MainMenuState(sf::RenderWindow* window);
		virtual ~MainMenuState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);
	};
}