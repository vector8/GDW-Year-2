#pragma once
#include <memory>

#include "Entity.h"
#include "Path.h"

namespace flopse
{
	enum class EnemyType
	{
		Goblin,
		TreasureGoblin,
		LichKing,
		KnifeHands
	};

	class Enemy : public Entity
	{
	private:
		Enemy(std::shared_ptr<Mesh> m);
		Enemy(const glm::vec3 &pos, std::shared_ptr<Mesh> m, int hp, int dmg, float spd, std::shared_ptr<Path> path = nullptr);

		int damage = 0;
		float speed = 0.f;

		sf::Time lifeTime;
		sf::Time attackTimer;
		sf::Time attackDelay;
		std::shared_ptr<Path> path;

	public:
		virtual ~Enemy();

		static Enemy createEnemy(const EnemyType &t, const glm::vec3 &pos, std::shared_ptr<Path> path = nullptr)
		{
			static std::shared_ptr<Mesh> GOBLIN_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag"), "textures/GoblinTexture.png");

			Enemy* e = nullptr;

			switch (t)
			{
			case EnemyType::Goblin:
				e = new Enemy(pos, GOBLIN_MESH, 30, 5, 400.f, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::TreasureGoblin:
				break;
			case EnemyType::LichKing:
				break;
			case EnemyType::KnifeHands:
				break;
			default:
				break;
			}

			Enemy enemy(*e);

			delete e;

			return enemy;
		}

		int health = 0;
		int maxHealth = 0;
		int value = 0;
		
		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}