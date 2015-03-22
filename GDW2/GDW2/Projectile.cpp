#include "Projectile.h"
#include "Utilities.h"
#include "Game.h"
#include <glm\gtx\vector_angle.hpp>

namespace flopse
{
	Projectile::Projectile()
	{

	}

	Projectile::Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage) : Entity(source, m), sourcePos(source), target(tar), damage(damage)
	{

	}

	Projectile::Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, glm::vec3 tar, int damage) : Entity(source, m), sourcePos(source), targetPos(tar), damage(damage)
	{

	}

	Projectile::Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage) : Entity(source), sourcePos(source), target(tar), damage(damage)
	{

	}

	Projectile::Projectile(const glm::vec3 &source, glm::vec3 tar, int damage) : Entity(source), sourcePos(source), targetPos(tar), damage(damage)
	{
		boundingBox.width = 10;
		boundingBox.height = 10;
		boundingBox.depth = 10;
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
						enemies[i]->takeDamage(damage);

						if (this->slows)
						{
							enemies[i]->setSlow();
						}

						if (this->burns)
						{
							enemies[i]->setOnFire();
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
				target->takeDamage(damage);

				if (this->slows)
				{
					target->setSlow();
				}

				if (this->burns)
				{
					target->setOnFire();
				}

				this->toBeDeleted = true;
			}
			else
			{
				this->localTransform.setPosition(lerp(interpParam, sourcePos, target->getGlobalPosition() + glm::vec3(0.f, target->mesh->getHeight() / 2.f, 0.f)));
				//angle projectile towards target
				glm::vec3 dir = target->localTransform.getPosition() - this->localTransform.getPosition();
				dir.y = 0.f;
				dir = glm::normalize(dir);
				glm::vec3 up(0.f, 1.f, 0.f);
				glm::vec3 oldFront = this->localTransform.getFront();
				oldFront.y = 0.f;
				oldFront = glm::normalize(oldFront);
				float angle = glm::orientedAngle(oldFront, dir, up);
				this->localTransform.rotate(angle, up);
			}
		}
		else
		{
			this->toBeDeleted = true;
		}
	}
}