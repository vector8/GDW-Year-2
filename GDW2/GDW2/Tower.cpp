#include "Tower.h"
#include "Projectile.h"
#include "Game.h"

namespace flopse
{
	Tower::Tower(Mesh *m) : Entity(m)
	{

	}

	Tower::Tower(const glm::vec3 &pos, Mesh *m, float range, float delaySeconds, int damage) : Entity(pos, m), range(range), shotDelay(sf::seconds(delaySeconds)), damage(damage)
	{
		this->mesh->overlayColour = Colour(0.f, 0.f, 1.f, 1.f);
	}

	void Tower::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		shotTimer += dt;

		if (shotTimer >= shotDelay)
		{
			shotTimer = sf::Time::Zero;

			std::vector<Enemy*> enemies = Game::getGame()->getEnemies();
			for (int i = 0; i < enemies.size(); i++)
			{
				if (!enemies[i]->toBeDeleted)
				{
					float distanceToEnemy = glm::length(enemies[i]->getGlobalPosition() - this->getGlobalPosition());
					if (distanceToEnemy <= this->range)
					{
						Projectile *p = new Projectile(this->getGlobalPosition() + glm::vec3(0.f, this->mesh->getHeight(), 0.f), enemies[i], this->damage);
						Game::getGame()->getCurrentLevel()->attach(p);
						return;
					}
				}
			}
		}
	}
}