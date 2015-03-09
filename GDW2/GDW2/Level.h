#pragma once
#include <memory>
#include <list>

#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "Path.h"
#include "Enemy.h"
#include "Light.h"

namespace flopse
{
	struct EnemySpawn
	{
		std::shared_ptr<Enemy> enemy = nullptr;
		sf::Time spawnTime;
	};

	class Level : public Entity
	{
	private:
		sf::Time elapsed;

		void initializeEntities();
		void createPath();
		void createColliders();
		void createEnemies();

	public:
		Level(const std::shared_ptr<Player> &p);
		virtual ~Level();

		Light dirLight;
		Light pointLights[NUM_POINT_LIGHTS];
		std::shared_ptr<Camera> cam = nullptr;
		std::shared_ptr<Player> player = nullptr;
		std::shared_ptr<Camera> shadowCamera = nullptr;
		float fogFactor = 0.0f;

		std::shared_ptr<Path> path = nullptr;

		int gateHealth = 200;
		int maxGateHealth = 200;
		int enemyCount = 0;

		std::vector<BoundingBox> colliders;
		std::vector<std::shared_ptr<Enemy>> enemies;
		std::list<EnemySpawn> enemySpawns;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}