#include "Projectile.h"
#include "Utilities.h"
#include "Game.h"

namespace flopse
{
	Projectile::Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage) : Entity(source, Projectile::getProjectileMesh()), sourcePos(source), target(tar), damage(damage)
	{

	}

	Projectile::Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage, TowerType type) : Entity(source, Projectile::getProjectileMesh(type)), sourcePos(source), target(tar), damage(damage), type(type)
	{
		this->type = type;
	}

	Projectile::Projectile(const glm::vec3 &source, glm::vec3 tar, int damage) : Entity(source, Projectile::getProjectileMesh()), sourcePos(source), targetPos(tar), damage(damage)
	{

	}

	Projectile::~Projectile()
	{

	}

	void Projectile::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		interpParam += dt.asSeconds();

		if (target == nullptr)
		{
			if (interpParam >= 1.f)
			{
				interpParam = 1.f;
				this->toBeDeleted = true;
			}
			
			this->localTransform.setPosition(lerp(interpParam, sourcePos, targetPos));
			this->boundingBox.position = this->getGlobalPosition();

			std::vector<std::shared_ptr<Enemy>> enemies = Game::getGame()->getEnemies();
			for (int i = 0; i < enemies.size(); i++)
			{
				if (!enemies[i]->toBeDeleted)
				{
					if (this->boundingBox.hasCollided(enemies[i]->boundingBox))
					{
						enemies[i]->health -= damage;

						if (enemies[i]->health <= 0)
						{
							enemies[i]->toBeDeleted = true;
							Game::getGame()->getPlayer()->gold += enemies[i]->value;
							Game::getGame()->getCurrentLevel()->enemyCount--;
						}

						this->toBeDeleted = true;
						break;
					}
				}
			}
		}
		else if (!target->toBeDeleted)
		{
			if (interpParam >= 1.f)
			{
				target->health -= damage;

				if (target->health <= 0)
				{
					target->toBeDeleted = true;
					Game::getGame()->getPlayer()->gold += target->value;
					Game::getGame()->getCurrentLevel()->enemyCount--;
				}

				this->toBeDeleted = true;
			}
			else
			{
				this->localTransform.setPosition(lerp(interpParam, sourcePos, target->getGlobalPosition() + glm::vec3(0.f, target->mesh->getHeight() / 2.f, 0.f)));
			}
		}
		else
		{
			this->toBeDeleted = true;
		}
	}
}