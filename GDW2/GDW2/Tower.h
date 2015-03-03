#pragma once
#include "Entity.h"

namespace flopse
{
	enum class TowerType
	{
		Arrow,
		Mage
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

	public:
		virtual ~Tower();

		static Tower createTower(const TowerType &t, const glm::vec3 &pos)
		{
			static std::shared_ptr<Mesh> ARROW_TOWER_MESH = std::make_shared<Mesh>("meshes/pyramid.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static std::shared_ptr<Mesh> FIRE_MAGE_TOWER_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
			static bool first = true;

			if (first)
			{
				FIRE_MAGE_TOWER_MESH->setDiffuseMap("textures/FireTower.png");
				first = false;
			}

			Tower* tow = nullptr;

			switch (t)
			{
			case TowerType::Arrow:
				tow = new Tower(pos, ARROW_TOWER_MESH, 500.f, 2.f, 12);
				break;
			case TowerType::Mage:
				tow = new Tower(pos, FIRE_MAGE_TOWER_MESH, 500.f, 2.f, 12);
				break;
			default:
				break;
			}

			Tower tower(*tow);
			delete tow;

			return tower;
		}

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}