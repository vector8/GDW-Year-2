#pragma once
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Path.h"
#include "Enemy.h"

namespace flopse
{
	struct EnemySpawn
	{
		Enemy* enemy;
		sf::Time spawnTime;
	};

	class Level : public Entity
	{
	private:
		sf::Time elapsed;
		int spawnCounter = 0;

		void initializeEntities();
		void createPath();
		void createColliders();
		void createEnemies();

	public:
		Level(Player* p);

		glm::vec3 lightPos;
		Camera* cam;
		Player* player;
		ParticleManager* particleManager;

		Path* path;

		int gateHealth = 200;

		std::vector<Entity*> entities;
		std::vector<BoundingBox*> colliders;
		std::vector<Enemy*> enemies;
		std::vector<EnemySpawn> enemySpawns;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}