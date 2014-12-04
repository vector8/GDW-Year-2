#pragma once
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>

#include <vector>

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "GameOverState.h"

namespace flopse
{
	class Game
	{
	private:
		Game();

		sf::Clock clock;
		sf::Time elapsed, fpsTimer;

		State* currentState;
		GameplayState* gameplayState;
		MainMenuState* mainMenuState;
		GameOverState* gameOverState;

		int frames;
		bool running;
		bool fullscreen;

	public:

		static Game* getGame()
		{
			static Game* g = new Game();

			return g;
		}

		sf::RenderWindow* window;

		Player* getPlayer() const;
		Level* getCurrentLevel() const;
		Camera* getCamera() const;
		std::vector<Entity*> getEntities() const;
		std::vector<BoundingBox*> getColliders() const;
		std::vector<Enemy*> getEnemies() const;

		void run();

		void newGame();
		void setGameplayState();
		void setMainMenuState();
		void setGameOverState();
		void exit();
		void checkGameOver();
	};
}