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
#include "LoadingState.h"
#include "LevelTransitionState.h"
#include "CreditsState.h"

namespace flopse
{
	class Game
	{
	private:
		Game();

		sf::Clock clock;
		sf::Time elapsed, fpsTimer;

		State* currentState = nullptr;
		GameplayState* gameplayState = nullptr;
		LoadingState* loadingState = nullptr;
		MainMenuState* mainMenuState = nullptr;
		OptionsMenuState* optionsMenuState = nullptr;
		GameOverState* gameOverState = nullptr;
		LevelTransitionState* levelTransitionState = nullptr;
		CreditsState* creditsState = nullptr;

		int frames;
		bool running;
		float fov = 45.f;
		int loadedLevelNumber = 0;

	public:
		virtual ~Game();

		static Game* getGame()
		{
			static Game* g = new Game();

			return g;
		}

		sf::RenderWindow* window;
		bool shouldDeleteGameplayState = false;
		bool nextLevel = false;

		std::shared_ptr<Player> getPlayer() const;
		std::shared_ptr<Level> getCurrentLevel() const;
		std::shared_ptr<Camera> getCamera() const;
		std::vector<BoundingBox> getColliders() const;
		std::vector<BoundingBox> getTowerBlockers() const;
		std::vector<std::shared_ptr<Enemy>> getEnemies() const;

		void run();

		void newGame();

		void saveGame();
		void loadGame();

		void setGameplayState();
		void setLoadingState();
		void setMainMenuState();
		void setOptionsMenuState();
		void setGameOverState();
		void setLevelTransitionState();
		void setCreditsState();

		void exit();
		void checkGameOver();

		void toggleFullscreen();
		bool isFullscreen();

		void setFieldOfView(float degrees);
		float getFieldOfView();
	};
}