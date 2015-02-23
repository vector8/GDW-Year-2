#include "Enemy.h"
#include <glm\gtx\vector_angle.hpp>
#include "Game.h"
#include "SoundManager.h"

namespace flopse
{
	Enemy::Enemy(const glm::vec3 &pos, std::shared_ptr<Mesh> m, int hp, int dmg, float spd, std::shared_ptr<Path> path) : Entity(pos, m), health(hp), maxHealth(hp), damage(dmg), speed(spd),
		path(path), lifeTime(sf::Time::Zero)
	{

	}

	Enemy::~Enemy()
	{
	}

	void Enemy::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		if (health > 0)
		{
			lifeTime += dt;
			float distance = lifeTime.asSeconds() * speed;

			
			if (path)
			{
				if (distance > path->getMaxDistance())
				{
					this->localTransform.setPosition(path->getPoint(path->getMaxDistance()));
					this->boundingBox.position = this->getGlobalPosition();

					attackTimer += dt;
					if (attackTimer >= attackDelay)
					{
						attackTimer = sf::Time::Zero;
						Game::getGame()->getCurrentLevel()->gateHealth -= this->damage;
						SoundManager::playSoundAt(DefaultSounds::MetalSmack, this->getGlobalPosition(), false);
						Game::getGame()->checkGameOver();
					}
				}
				else
				{
					this->localTransform.setPosition(path->getPoint(distance));
					this->boundingBox.position = this->getGlobalPosition();

					glm::vec3 nextFront = path->getPoint(distance + 10.f) - this->localTransform.getPosition();
					glm::vec3 up(0.f, 1.f, 0.f);
					float angle = glm::orientedAngle(glm::normalize(this->localTransform.getFront()), glm::normalize(nextFront), up);

					this->localTransform.rotate(angle, up);
				}
			}
		}
	}
}