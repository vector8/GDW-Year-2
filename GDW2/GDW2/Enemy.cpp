#include "Enemy.h"
#include <glm\gtx\vector_angle.hpp>
#include "Game.h"
#include "SoundManager.h"
#include "Level.h"
#include "Path.h"
#include "PointCollection.h"

namespace flopse
{
	Enemy::Enemy(const glm::vec3 &pos, std::shared_ptr<Mesh> m, int hp, int dmg, float spd, std::shared_ptr<Path> path) : Entity(pos, m), health(hp), maxHealth(hp), damage(dmg), speed(spd),
		path(path), lifeTime(sf::Time::Zero), fireParticles(std::make_shared<ParticleSystem>(15, 50, "textures/Flame.png"))
	{
		fireParticles->lerpAlpha = glm::vec2(1.f, 0.f);
		fireParticles->lerpSize = glm::vec2(10.f, 2.5f);
		fireParticles->rangeLifetime = glm::vec2(0.5f, 1.f);
		fireParticles->rangeVelocity = glm::vec2(1.f, 2.f);

		float width = 1.25f * mesh->getWidth() / 2.f;
		float height = 1.25f * mesh->getHeight() / 2.f;
		float depth = 1.25f * mesh->getDepth() / 2.f;

		fireParticles->rangeX = glm::vec2(-width, width);
		fireParticles->rangeY = glm::vec2(-height, height);
		fireParticles->rangeZ = glm::vec2(-depth, depth);
	}

	Enemy::~Enemy()
	{
	}

	void Enemy::setSlow()
	{
		slowTimer = sf::seconds(SLOW_DURATION);
	}

	void Enemy::setOnFire()
	{
		burnTimer = sf::seconds(BURN_DURATION);

		if (!onFire)
		{
			attach(fireParticles);
			onFire = true;
		}
	}

	void Enemy::takeDamage(int damage)
	{
		health -= damage;

		if (health <= 0)
		{
			toBeDeleted = true;
			Game::getGame()->getPlayer()->gold += value;
			Game::getGame()->getCurrentLevel()->enemyCount--;
		}
		else
		{
			damageIndicatorTimer = sf::seconds(DAMAGE_INDICATOR_DURATION);
			overlayColour = Colour(1.f, 0.4f, 0.4f, 1.f);
		}
	}

	void Enemy::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		if (health > 0)
		{
			sf::Time delta = dt;

			if (slowTimer > sf::Time::Zero)
			{
				slowTimer -= dt;
				delta = delta * SLOW_FACTOR;

				if (damageIndicatorTimer <= sf::Time::Zero)
				{
					overlayColour = Colour(0.62f, 0.82f, 0.98f, 1.f);
				}
			}
			else if (damageIndicatorTimer <= sf::Time::Zero)
			{
				overlayColour = Colour();
			}

			if (burnTimer > sf::Time::Zero)
			{
				burnTimer -= dt;
				currentBurnTime += dt;

				if (currentBurnTime > sf::seconds(1.f))
				{
					currentBurnTime -= sf::seconds(1.f);
					
					takeDamage(BURN_DPS);
				}
			}
			else if (onFire)
			{
				onFire = false;
				detach(fireParticles);
			}

			if (damageIndicatorTimer > sf::Time::Zero)
			{
				damageIndicatorTimer -= dt;
			}

			lifeTime += delta;
			float distance = lifeTime.asSeconds() * speed;
			
			if (path)
			{
				if (distance > path->getMaxDistance())
				{
					this->localTransform.setPosition(path->getPoint(path->getMaxDistance()));
					this->boundingBox.position = this->getGlobalPosition();

					attackTimer += delta;
					if (attackTimer >= attackDelay)
					{
						attackTimer = sf::Time::Zero;
						Game::getGame()->getCurrentLevel()->gateHealth -= this->damage;
						SoundManager::playSoundAt(DefaultSounds::MetalSmack, this->getGlobalPosition());
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

					// Update the running animation
					if (runAnimation != nullptr)
					{
						runAnimation->update(delta);
						mesh = runAnimation->getCurrentMesh();
					}
				}
			}
			
		}
	}
}