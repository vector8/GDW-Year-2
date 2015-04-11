#pragma once
#include "State.h"

namespace flopse
{
	class LoadingState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;

		bool firstPass = true;

	public:
		LoadingState(sf::RenderWindow* window);
		virtual ~LoadingState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);
	};
}