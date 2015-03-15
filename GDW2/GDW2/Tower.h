#pragma once
#include "Entity.h"

namespace flopse
{
	enum class TowerType
	{
		Arrow,
		Frost,
		Fire,
		Catapult,
		Barricade
	};

	class Tower : public Entity
	{
	private:
		Tower(std::shared_ptr<Mesh> m);
		Tower(const glm::vec3 &pos, std::shared_ptr<Mesh> m, float range, float delaySeconds, int damage);

		sf::Time shotTimer;
		sf::Time shotDelay;

		float range;
		int damage;
		bool active = false;

	public:
		virtual ~Tower();

		TowerType type;

		static Tower createTower(const TowerType &t, const glm::vec3 &pos)
		{
			static std::shared_ptr<Mesh> ARROW_TOWER_MESH = std::make_shared<Mesh>("meshes/ArrowTower.bmf", Shader::getStandardShader(StandardShaders::TowerPlacement));
			static std::shared_ptr<Mesh> FROST_MAGE_TOWER_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::TowerPlacement));
			static std::shared_ptr<Mesh> FIRE_MAGE_TOWER_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::TowerPlacement));
			static std::shared_ptr<Mesh> CATAPULT_TOWER_MESH = std::make_shared<Mesh>("meshes/Catapult.bmf", Shader::getStandardShader(StandardShaders::TowerPlacement));
			static bool first = true;

			if (first)
			{
				ARROW_TOWER_MESH->setDiffuseMap("textures/ArrowTower.png");
				FROST_MAGE_TOWER_MESH->setDiffuseMap("textures/FrostTower.png");
				FIRE_MAGE_TOWER_MESH->setDiffuseMap("textures/FireTower.png");
				CATAPULT_TOWER_MESH->setDiffuseMap("textures/CatapultTower.png");
				first = false;
			}

			Tower* tow = nullptr;

			switch (t)
			{
			case TowerType::Arrow:
				tow = new Tower(pos, ARROW_TOWER_MESH, 1000.f, 1.f, 10);
				break;
			case TowerType::Frost:
				tow = new Tower(pos, FROST_MAGE_TOWER_MESH, 1000.f, 1.f, 2);
				break;
			case TowerType::Fire:
				tow = new Tower(pos, FIRE_MAGE_TOWER_MESH, 1000.f, 1.f, 1);
				break;
			case TowerType::Catapult:
				tow = new Tower(pos, CATAPULT_TOWER_MESH, 1000.f, 2.f, 5);
				break;
			default:
				break;
			}

			tow->type = t;

			Tower tower(*tow);
			delete tow;

			return tower;
		}

		void activate();

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}