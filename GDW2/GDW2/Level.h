#pragma once
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "Path.h"
#include "Enemy.h"

namespace flopse
{
	class Level : public Entity
	{
	private:
		sf::Time spawnTimer;

		void initializeEntities();
		void createPath();
		void createColliders();

	public:
		Level(Player* p);

		glm::vec3 lightPos;
		Camera* cam;
		Player* player;
		ParticleManager* particleManager;

		Path* path;

		std::vector<Entity*> entities;
		std::vector<BoundingBox*> colliders;
		std::vector<Enemy*> enemies;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}