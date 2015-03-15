#include "Tower.h"
#include "Projectile.h"
#include "Game.h"
#include "SoundManager.h"
#include <glm\gtx\vector_angle.hpp>

namespace flopse
{
	Tower::Tower(std::shared_ptr<Mesh> m) : Entity(m)
	{

	}

	Tower::Tower(const glm::vec3 &pos, std::shared_ptr<Mesh> m, float range, float delaySeconds, int damage) : Entity(pos, m), range(range), shotDelay(sf::seconds(delaySeconds)), damage(damage)
	{
		this->mesh->overlayColour = Colour(0.f, 0.f, 1.f, 1.f);
	}

	Tower::~Tower()
	{

	}

	void Tower::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		shotTimer += dt;

		std::vector<std::shared_ptr<Enemy>> enemies = Game::getGame()->getEnemies();
		for (int i = 0; i < enemies.size(); i++)
		{
			if (!enemies[i]->toBeDeleted)
			{
				float distanceToEnemy = glm::length(enemies[i]->getGlobalPosition() - this->getGlobalPosition());
				if (distanceToEnemy <= this->range)
				{
					// Turn Tower to face enemy.
					glm::vec3 dir = enemies[i]->localTransform.getPosition() - this->localTransform.getPosition();
					dir.y = 0.f;
					dir = glm::normalize(dir);
					glm::vec3 up(0.f, 1.f, 0.f);
					glm::vec3 oldFront = this->localTransform.getFront();
					oldFront.y = 0.f;
					oldFront = glm::normalize(oldFront);
					float angle = glm::orientedAngle(oldFront, dir, up);
					this->localTransform.rotate(angle, up);
					//Shoot enemy
					if (shotTimer >= shotDelay)
					{
						shotTimer = sf::Time::Zero;
						auto p = std::make_shared<Projectile>(this->getGlobalPosition() + glm::vec3(0.f, this->mesh->getHeight(), 0.f), enemies[i], this->damage);
						Game::getGame()->getCurrentLevel()->attach(p);
						SoundManager::playSoundAt(DefaultSounds::Ping, this->getGlobalPosition(), false);
					}
					return;
				}
			}
		}
	}
}