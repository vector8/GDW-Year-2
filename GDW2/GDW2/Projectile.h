#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Tower.h"

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
		Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		Projectile(std::shared_ptr<Mesh> m, const glm::vec3 &source, glm::vec3 tar, int damage);

		static Projectile createProjectile(const glm::vec3 &source, const glm::vec3 &tar, int damage)
		{
			static std::shared_ptr<Mesh> PLAYER_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static bool first = true;

			if (first)
			{
				PLAYER_PROJECTILE_MESH->overlayColour = Colour(0.5f, 0.f, 0.5f, 1.f);
				first = false;
			}

			Projectile projectile(PLAYER_PROJECTILE_MESH, source, tar, damage);

			return projectile;
		}

		static Projectile createProjectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage, TowerType t)
		{
			static std::shared_ptr<Mesh> ARROW_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/Arrow.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static std::shared_ptr<Mesh> FROST_MAGE_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static std::shared_ptr<Mesh> FIRE_MAGE_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static std::shared_ptr<Mesh> CATAPULT_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
			static bool first = true;

			if (first)
			{
				ARROW_PROJECTILE_MESH->overlayColour = Colour(0.8f, 0.3f, 0.1f, 1.f);
				FROST_MAGE_PROJECTILE_MESH->overlayColour = Colour(0.1f, 0.1f, 1.f, 1.f);
				FIRE_MAGE_PROJECTILE_MESH->overlayColour = Colour(1.f, 0.1f, 0.1f, 1.f);
				CATAPULT_PROJECTILE_MESH->overlayColour = Colour(0.f, 0.f, 0.f, 1.f);
				first = false;
			}

			Projectile* pro = nullptr;

			switch (t)
			{
			case TowerType::Arrow:
				pro = new Projectile(ARROW_PROJECTILE_MESH, source, tar, damage);
				break;
			case TowerType::Frost:
				pro = new Projectile(FROST_MAGE_PROJECTILE_MESH, source, tar, damage);
				break;
			case TowerType::Fire:
				pro = new Projectile(FIRE_MAGE_PROJECTILE_MESH, source, tar, damage);
				break;
			case TowerType::Catapult:
				pro = new Projectile(CATAPULT_PROJECTILE_MESH, source, tar, damage);
				break;
			default:
				break;
			}

			Projectile projectile(*pro);
			delete pro;

			return projectile;
		}

		virtual ~Projectile();

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}