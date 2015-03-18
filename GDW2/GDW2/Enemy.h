#pragma once
#include <memory>

#include "Entity.h"
#include "Path.h"

namespace flopse
{
	enum class EnemyType
	{
		Orc,
		Elf,
		Goblin,
		E_ORC,
		E_ELF,
		Golem,
		TreasureGoblin,
		LichKing,
		KnifeHands
	};

	class Enemy : public Entity
	{
	private:
		Enemy(std::shared_ptr<Mesh> m);
		Enemy(const glm::vec3 &pos, std::shared_ptr<Mesh> m, int hp, int dmg, float spd, bool slowed, bool onFire, std::shared_ptr<Path> path = nullptr);

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
			static std::shared_ptr<Mesh> ORC_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/GoblinTexture.png", "textures/GoblinSpecularMap.png");
			static std::shared_ptr<Mesh> ELF_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/GoblinTexture.png", "textures/GoblinSpecularMap.png");
			static std::shared_ptr<Mesh> GOBLIN_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/GoblinTexture.png", "textures/GoblinSpecularMap.png");
			static std::shared_ptr<Mesh> E_ORC_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/GoblinTexture.png", "textures/GoblinSpecularMap.png");
			static std::shared_ptr<Mesh> E_ELF_MESH = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/GoblinTexture.png", "textures/GoblinSpecularMap.png");
			static std::shared_ptr<Mesh> GOLEM_MESH = std::make_shared<Mesh>("meshes/GolemWalkFrame1.bmf", Shader::getStandardShader(StandardShaders::Phong), "textures/golemTexture.png", "textures/GoblinSpecularMap.png");

			Enemy* e = nullptr;

			switch (t)
			{
			case EnemyType::Orc:
				e = new Enemy(pos, ORC_MESH, 100, 5, 400.f, false, false, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::Elf:
				e = new Enemy(pos, ELF_MESH, 80, 5, 600.f, false, false, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::Goblin:
				e = new Enemy(pos, GOBLIN_MESH, 40, 2.5f, 400.f, false, false, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::E_ORC:
				e = new Enemy(pos, E_ORC_MESH, 250, 10, 320.f, false, false, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::E_ELF:
				e = new Enemy(pos, E_ELF_MESH, 150, 7.5f, 720.f, false, false, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				break;
			case EnemyType::Golem:
				e = new Enemy(pos, GOLEM_MESH, 1800, 40, 320.f, false, false, path);
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