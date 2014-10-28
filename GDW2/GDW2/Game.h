#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>

#include <vector>

#include "Entity.h"
#include "Player.h"
#include "Camera.h"

class Game
{
private:
	Game();

	sf::RenderWindow window;
	sf::Clock clock;
	sf::Time elapsed, fpsTimer;
	glm::mat4 projection;

	int frames;
	bool running;

	Camera* cam;
	Player* player;

	void initializeEntities();

public:
	std::vector<Entity*> entities;
	std::vector<BoundingBox*> colliders;

	static Game* getGame()
	{
		static Game* g = new Game();

		return g;
	}

	void run();
};