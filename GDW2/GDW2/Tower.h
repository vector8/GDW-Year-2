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
		Tower(Mesh *m);
		Tower(const glm::vec3 &pos, Mesh *m, float range, float delaySeconds, int damage);

		sf::Time shotTimer;
		sf::Time shotDelay;

		float range;
		int damage;

	public:
		static Tower* createTower(const TowerType &t, const glm::vec3 &pos)
		{
			static Mesh* ARROW_TOWER_MESH = new Mesh("obj/pyramid.obj", new Shader("shaders/texShader.vs", "shaders/grayShader.frag"));

			Tower* tow = nullptr;

			switch (t)
			{
			case TowerType::Arrow:
				tow = new Tower(pos, ARROW_TOWER_MESH, 500.f, 2.f, 12);
				break;
			case TowerType::Mage:
				break;
			default:
				break;
			}

			return tow;
		}

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}