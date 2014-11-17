#pragma once
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>

#include <vector>

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "ParticleManager.h"

namespace flopse
{
	class Game
	{
	private:
		Game();

		sf::RenderWindow window;
		sf::Clock clock;
		sf::Time elapsed, fpsTimer;
		glm::mat4 projection;
		glm::vec3 lightPos;

		int frames;
		bool running;

		void initializeEntities();

	public:
		std::vector<Entity*> entities;
		std::vector<BoundingBox*> colliders;

		Camera* cam;
		Player* player;
		ParticleManager* particleManager;
		SceneNode* root;

		static Game* getGame()
		{
			static Game* g = new Game();

			return g;
		}

		void run();
	};
}