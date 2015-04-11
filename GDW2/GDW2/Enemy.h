#pragma once
#include <memory>

#include "Entity.h"
#include "Path.h"
#include "Animation.h"
#include "ParticleSystem.h"

#define SLOW_DURATION 3.f
#define BURN_DURATION 3.f
#define SLOW_FACTOR 0.5f
#define BURN_DPS 10.f
#define DAMAGE_INDICATOR_DURATION 0.25f

namespace flopse
{
	enum class EnemyType
	{
		Orc,
		Elf,
		Goblin,
		E_ORC,
		E_ELF,
		Golem,
	};

	class Enemy : public Entity
	{
	private:
		Enemy(std::shared_ptr<Mesh> m);
		Enemy(const glm::vec3 &pos, std::shared_ptr<Mesh> m, int hp, int dmg, float spd, std::shared_ptr<Path> path = nullptr);

		int damage = 0;
		float speed = 0.f;

		bool onFire = false;

		std::shared_ptr<ParticleSystem> fireParticles;

		sf::Time lifeTime;
		sf::Time attackTimer;
		sf::Time attackDelay;
		sf::Time slowTimer;
		sf::Time burnTimer;
		sf::Time currentBurnTime;
		sf::Time damageIndicatorTimer;

	public:
		virtual ~Enemy();

		// Stats
		int health = 0;
		int maxHealth = 0;
		int value = 0;

		std::shared_ptr<Path> path;

		std::shared_ptr<Animation> runAnimation = nullptr;
		std::shared_ptr<Animation> attackAnimation = nullptr;

		static Enemy createEnemy(const EnemyType &t, const glm::vec3 &pos, std::shared_ptr<Path> path = nullptr)
		{
			static std::vector<Keyframe> goblinRunFrames;
			static std::vector<Keyframe> goblinAttackFrames;
			static std::vector<Keyframe> elfRunFrames;
			static std::vector<Keyframe> armorElfRunFrames;
			static std::vector<Keyframe> orcRunFrames;
			static std::vector<Keyframe> heavyOrcRunFrames;
			static std::vector<Keyframe> golemRunFrames;
			static std::vector<Keyframe> golemAttackFrames;
			
			static bool first = true;

			// Cache all animations here.
			if (first)
			{
				std::shared_ptr<Shader> s = Shader::getStandardShader(StandardShaders::Phong);

				// Goblin Run Animation
				for (int i = 1; i <= 8; i++)
				{
					std::string filename = "meshes/GoblinRun" + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/GoblinTexture.png");
					frame.mesh->setSpecularMap("textures/GoblinSpecularMap.png");

					if (i == 8)
					{
						frame.duration = sf::seconds(0.033);
					}
					else
					{
						frame.duration = sf::seconds(0.1);
					}

					goblinRunFrames.push_back(frame);
				}

				// Elf Walk Animation
				for (int i = 1; i <= 6; i++)
				{
					std::string filename = "meshes/ElfWalkFrame" + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/elfTexture.png");
					frame.mesh->setSpecularMap("textures/BlankSpecular.png");

					frame.duration = sf::seconds(0.1);

					elfRunFrames.push_back(frame);
				}

				// Armor Elf Walk Animation
				for (int i = 1; i <= 6; i++)
				{
					std::string filename = "meshes/ElfArmorWalkFrame" + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/elfArmorTexture.png");
					frame.mesh->setSpecularMap("textures/BlankSpecular.png");

					frame.duration = sf::seconds(0.1);

					armorElfRunFrames.push_back(frame);
				}

				// Orc Run Animation
				for (int i = 1; i <= 5; i++)
				{
					std::string filename = "meshes/OrcRun" + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/orc.png");
					frame.mesh->setSpecularMap("textures/BlankSpecular.png");

					frame.duration = sf::seconds(0.1);

					orcRunFrames.push_back(frame);
				}

				// Heavy Orc run animation
				for (int i = 1; i <= 5; i++)
				{
					std::string filename = "meshes/heavy orc run " + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/heavy orc uv.png");
					frame.mesh->setSpecularMap("textures/BlankSpecular.png");

					frame.duration = sf::seconds(0.1);

					heavyOrcRunFrames.push_back(frame);
				}

				// Golem Run Animation
				for (int i = 1; i <= 6; i++)
				{
					std::string filename = "meshes/GolemWalkFrame" + std::to_string(i) + ".bmf";
					Keyframe frame;
					frame.mesh = std::make_shared<Mesh>(filename, s);
					frame.mesh->setDiffuseMap("textures/golemTexture.png");
					frame.mesh->setSpecularMap("textures/BlankSpecular.png");

					frame.duration = sf::seconds(0.1);

					golemRunFrames.push_back(frame);
				}

				first = false;
			}

			Enemy* e = nullptr;

			switch (t)
			{
			case EnemyType::Orc:
				e = new Enemy(pos, orcRunFrames[0].mesh, 100, 5, 400.f, path);
				e->value = 5;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(orcRunFrames);
				break;
			case EnemyType::Elf:
				e = new Enemy(pos, elfRunFrames[0].mesh, 80, 5, 600.f, path);
				e->value = 5;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(elfRunFrames);
				break;
			case EnemyType::Goblin:
				e = new Enemy(pos, goblinRunFrames[0].mesh, 40, 2.5f, 400.f, path);
				e->value = 2;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(goblinRunFrames);
				break;
			case EnemyType::E_ORC:
				e = new Enemy(pos, heavyOrcRunFrames[0].mesh, 250, 10, 320.f, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(heavyOrcRunFrames);
				break;
			case EnemyType::E_ELF:
				e = new Enemy(pos, armorElfRunFrames[0].mesh, 150, 7.5f, 720.f, path);
				e->value = 10;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(armorElfRunFrames);
				break;
			case EnemyType::Golem:
				e = new Enemy(pos, golemRunFrames[0].mesh, 1800, 40, 320.f, path);
				e->value = 50;
				e->attackDelay = sf::seconds(1.f);
				e->runAnimation = std::make_shared<Animation>(golemRunFrames);
				break;
			default:
				break;
			}

			Enemy enemy(*e);

			delete e;

			return enemy;
		}

		// Status effects
		void setSlow();
		void setOnFire();

		void takeDamage(int damage);
		
		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};

}