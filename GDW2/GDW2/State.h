#pragma once
#include <SFML\Graphics.hpp>

namespace flopse
{
	class State
	{
	public:
		State();
		virtual ~State();

		virtual void update(const sf::Time &dt) = 0;
		virtual void draw() = 0;

		virtual void keyPressed(sf::Event::KeyEvent e) = 0;
		virtual void mouseButtonPressed(sf::Event::MouseButtonEvent e) = 0;
		virtual void mouseButtonReleased(sf::Event::MouseButtonEvent e) = 0;
		virtual void mouseMoved(sf::Event::MouseMoveEvent e) = 0;
	};
}