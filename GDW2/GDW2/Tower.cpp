#include "Tower.h"
#include "Projectile.h"
#include "Game.h"
#include "SoundManager.h"
#include <glm\gtx\vector_angle.hpp>
#include <typeinfo>

namespace flopse
{
	Tower::Tower(std::shared_ptr<Mesh> m) : Entity(m)
	{

	}

	Tower::Tower(const glm::vec3 &pos, std::shared_ptr<Mesh> m, float range, float delaySeconds, int damage) : Entity(pos, m), range(range), shotDelay(sf::seconds(delaySeconds)), damage(damage)
	{

	}

	Tower::~Tower()
	{

	}

	void Tower::activate()
	{
		static std::shared_ptr<Mesh> ARROW_TOWER_MESH = std::make_shared<Mesh>("meshes/ArrowTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
		static std::shared_ptr<Mesh> FROST_MAGE_TOWER_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
		static std::shared_ptr<Mesh> FIRE_MAGE_TOWER_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
		static std::shared_ptr<Mesh> CATAPULT_TOWER_MESH = std::make_shared<Mesh>("meshes/Catapult.bmf", Shader::getStandardShader(StandardShaders::Phong));
		static bool first = true;

		if (first)
		{
			ARROW_TOWER_MESH->setDiffuseMap("textures/ArrowTower.png");
			FROST_MAGE_TOWER_MESH->setDiffuseMap("textures/FrostTower.png");
			FIRE_MAGE_TOWER_MESH->setDiffuseMap("textures/FireTower.png");
			CATAPULT_TOWER_MESH->setDiffuseMap("textures/CatapultTower.png");
			first = false;
		}

		if (!active)
		{
			switch (this->type)
			{
			case TowerType::Arrow:
				this->mesh = ARROW_TOWER_MESH;
				break;
			case TowerType::Frost:
				this->mesh = FROST_MAGE_TOWER_MESH;
				break;
			case TowerType::Fire:
				this->mesh = FIRE_MAGE_TOWER_MESH;
				break;
			case TowerType::Catapult:
				this->mesh = CATAPULT_TOWER_MESH;
				break;
			default:
				break;
			}

			active = true;
		}
	}

	void Tower::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		if (active)
		{
			shotTimer += dt;

			std::vector<std::shared_ptr<Enemy>> enemies = Game::getGame()->getEnemies();
			for (int i = 0; i < enemies.size(); i++)
			{
				if (!enemies[i]->toBeDeleted)
				{
					glm::vec3 enemyPosition = enemies[i]->getGlobalPosition();
					float distanceToEnemy = glm::length(enemyPosition - this->getGlobalPosition());
					if (distanceToEnemy <= this->range && !(enemyPosition.x == 0.0 && enemyPosition.z == 0.0))
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
							auto p = std::make_shared<Projectile>(Projectile::createProjectile(this->getGlobalPosition() + glm::vec3(0, this->mesh->getHeight(), 0) + this->getGlobalFront()*(this->mesh->getDepth() / 2), enemies[i], this->damage, this->type));
							Game::getGame()->getCurrentLevel()->attach(p);
							switch (this->type)
							{
							case TowerType::Fire:
								SoundManager::playSoundAt(DefaultSounds::Ping, this->getGlobalPosition());
								break;
							case TowerType::Frost:
								SoundManager::playSoundAt(DefaultSounds::Frost, this->getGlobalPosition());
								break;
							case TowerType::Arrow:
								SoundManager::playSoundAt(DefaultSounds::Arrow, this->getGlobalPosition());
								break;
							case TowerType::Catapult:
								SoundManager::playSoundAt(DefaultSounds::Catapult, this->getGlobalPosition());
								break;
							}
						}
						return;
					}
				}
			}
		}
		else
		{
			if (Game::getGame()->getPlayer()->jumping)
			{
				mesh->validPlacement = false;
			} else if (std::abs(this->getGlobalPosition().y - Game::getGame()->getCurrentLevel()->groundLevel) >= 20.0)
			{
				mesh->validPlacement = false;
			} else
			{
				bool collided = false;
				boundingBox.position = this->getGlobalPosition();
				std::vector<BoundingBox> blockers = Game::getGame()->getTowerBlockers();
				for (auto it = blockers.begin(); it != blockers.end(); it++)
				{
					if (boundingBox.hasCollided(*it))
					{
						collided = true;
						break;
					}
				}

				mesh->validPlacement = !collided;
			}
		}
	}
}