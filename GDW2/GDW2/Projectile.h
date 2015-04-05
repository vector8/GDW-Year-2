#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Tower.h"
#include "ParticleSystem.h"

namespace flopse
{
	class Projectile : public Entity
	{
	private:

		glm::vec3 sourcePos;
		std::shared_ptr<Enemy> target = nullptr;
		glm::vec3 targetPos;

		float interpParam = 0.f;
		int damage = 0;

	public:
		Projectile();
		Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, glm::vec3 tar, int damage);
		Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		Projectile(const glm::vec3 &source, glm::vec3 tar, int damage);

		bool slows = false;
		bool burns = false;

		static Projectile createProjectile(const glm::vec3 &source, const glm::vec3 &tar, int damage)
		{
			Projectile projectile(source, tar, damage);

			std::shared_ptr<ParticleSystem> magicEffect = std::make_shared<ParticleSystem>(15, 50, "textures/MagicParticle.png");
			magicEffect->lerpAlpha = glm::vec2(1.f, 0.f);
			magicEffect->lerpSize = glm::vec2(10.f, 2.5f);
			magicEffect->rangeLifetime = glm::vec2(0.5f, 1.f);
			magicEffect->rangeVelocity = glm::vec2(1.f, 2.f);
			magicEffect->rangeX = glm::vec2(-10.f, 10.f);
			magicEffect->rangeY = glm::vec2(-10.f, 10.f);
			magicEffect->rangeZ = glm::vec2(-10.f, 10.f);

			projectile.attach(magicEffect);

			return projectile;
		}

		static Projectile createProjectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage, TowerType t)
		{
			static std::shared_ptr<Mesh> ARROW_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/Arrow.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static std::shared_ptr<Mesh> CATAPULT_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));

			Projectile projectile;

			switch (t)
			{
			case TowerType::Arrow:
				projectile = Projectile(ARROW_PROJECTILE_MESH, source, tar, damage);
				projectile.overlayColour = Colour(0.8f, 0.3f, 0.1f, 1.f);
				break;
			case TowerType::Frost:
			{
				projectile = Projectile(source, tar, damage);
				projectile.slows = true;

				std::shared_ptr<ParticleSystem> magicEffect = std::make_shared<ParticleSystem>(15, 50, "textures/Snowflake.png");
				magicEffect->lerpAlpha = glm::vec2(1.f, 0.f);
				magicEffect->lerpSize = glm::vec2(10.f, 2.5f);
				magicEffect->rangeLifetime = glm::vec2(0.5f, 1.f);
				magicEffect->rangeVelocity = glm::vec2(1.f, 2.f);
				magicEffect->rangeX = glm::vec2(-10.f, 10.f);
				magicEffect->rangeY = glm::vec2(-10.f, 10.f);
				magicEffect->rangeZ = glm::vec2(-10.f, 10.f);

				projectile.attach(magicEffect);
			}
				break;
			case TowerType::Fire:
			{
				projectile = Projectile(source, tar, damage);
				projectile.burns = true;

				std::shared_ptr<ParticleSystem> magicEffect = std::make_shared<ParticleSystem>(15, 50, "textures/Flame.png");
				magicEffect->lerpAlpha = glm::vec2(1.f, 0.f);
				magicEffect->lerpSize = glm::vec2(10.f, 2.5f);
				magicEffect->rangeLifetime = glm::vec2(0.5f, 1.f);
				magicEffect->rangeVelocity = glm::vec2(1.f, 2.f);
				magicEffect->rangeX = glm::vec2(-10.f, 10.f);
				magicEffect->rangeY = glm::vec2(-10.f, 10.f);
				magicEffect->rangeZ = glm::vec2(-10.f, 10.f);

				projectile.attach(magicEffect);
			}
				break;
			case TowerType::Catapult:
				projectile = Projectile(CATAPULT_PROJECTILE_MESH, source, tar, damage);
				projectile.overlayColour = Colour(0.f, 0.f, 0.f, 1.f);
				break;
			default:
				break;
			}

			return projectile;
		}

		virtual ~Projectile();

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}