#pragma once
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>

#include <vector>
#include <memory>

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "OptionsMenuState.h"

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
		OptionsMenuState* optionsMenuState;
		GameOverState* gameOverState;

		int frames;
		bool running;
		bool fullscreen;
		float fov = 45.f;

	public:
		virtual ~Game();

		static Game* getGame()
		{
			static Game* g = new Game();

			return g;
		}

		sf::RenderWindow* window;

		std::shared_ptr<Player> getPlayer() const;
		Level* getCurrentLevel() const;
		std::shared_ptr<Camera> getCamera() const;
		std::vector<BoundingBox> getColliders() const;
		std::vector<std::shared_ptr<Enemy>> getEnemies() const;

		void run();

		void newGame();
		void setGameplayState();
		void setMainMenuState();
		void setOptionsMenuState();
		void setGameOverState();
		void exit();
		void checkGameOver();

		void toggleFullscreen();
		bool isFullscreen();

		void setFieldOfView(float degrees);
		float getFieldOfView();
	};
}