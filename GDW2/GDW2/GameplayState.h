#pragma once
#include "State.h"
#include <glm\glm.hpp>
#include "Level.h"

namespace flopse
{
	class GameplayState : public State
	{
	private:
		glm::mat4 projection;
		sf::RenderWindow* window;

	public:
		SceneNode* root;
		Player* player;
		Level* currentLevel;

		GameplayState(sf::RenderWindow* window);

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);
	};
}