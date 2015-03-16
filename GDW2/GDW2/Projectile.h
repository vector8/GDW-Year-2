#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Tower.h"

namespace flopse
{
	/*enum class ProjectileType
	{
	Arrow,
	Frost,
	Fire,
	Catapult
	};*/

	class Projectile : public Entity
	{
	private:
		//Projectile(std::shared_ptr<Mesh> m);
		//Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		//Projectile(const glm::vec3 &source, glm::vec3 tar,std::shared_ptr<Mesh> m, int damage);

		glm::vec3 sourcePos;
		std::shared_ptr<Enemy> target = nullptr;
		glm::vec3 targetPos;

		float interpParam = 0.f;
		int damage = 0;

		std::shared_ptr<Mesh> getProjectileMesh()
		{
			std::shared_ptr<Mesh> projectileMesh;

			if (type == TowerType::Arrow)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/Arrow.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.8f, 0.3f, 0.1f, 1.f);
			}
			else if (type == TowerType::Frost)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.1f, 0.1f, 1.0f, 1.f);
			}
			else if (type == TowerType::Fire)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(1.0f, 0.1f, 0.1f, 1.f);
			}
			else if (type == TowerType::Catapult)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.0f, 0.0f, 0.0f, 1.f);
			}
			else
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.5f, 0.f, 0.5f, 1.f);
			}

			return projectileMesh;
		}
		std::shared_ptr<Mesh> getProjectileMesh(TowerType t)
		{
			std::shared_ptr<Mesh> projectileMesh;

			if (t == TowerType::Arrow)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/Arrow.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.8f, 0.3f, 0.1f, 1.f);
			}
			else if (t == TowerType::Frost)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.1f, 0.1f, 1.0f, 1.f);
			}
			else if (t == TowerType::Fire)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(1.0f, 0.1f, 0.1f, 1.f);

			}
			else if (t == TowerType::Catapult)
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.0f, 0.0f, 0.0f, 1.f);
			}
			else
			{
				projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));
				projectileMesh->overlayColour = Colour(0.5f, 0.f, 0.5f, 1.f);
			}

			return projectileMesh;
		}

	public:
		Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage, TowerType type);
		Projectile(const glm::vec3 &source, glm::vec3 tar, int damage);

		TowerType type = TowerType::Barricade;

		static Projectile createProjectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage, TowerType t)
		{
			/*static std::shared_ptr<Mesh> ARROW_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/ArrowTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
			static std::shared_ptr<Mesh> FROST_MAGE_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
			static std::shared_ptr<Mesh> FIRE_MAGE_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/MageTower.bmf", Shader::getStandardShader(StandardShaders::Phong));
			static std::shared_ptr<Mesh> CATAPULT_PROJECTILE_MESH = std::make_shared<Mesh>("meshes/Catapult.bmf", Shader::getStandardShader(StandardShaders::Phong));
			static bool first = true;

			if (first)
			{
				ARROW_PROJECTILE_MESH->setDiffuseMap("textures/ArrowTower.png");
				FROST_MAGE_PROJECTILE_MESH->setDiffuseMap("textures/FrostTower.png");
				FIRE_MAGE_PROJECTILE_MESH->setDiffuseMap("textures/FireTower.png");
				CATAPULT_PROJECTILE_MESH->setDiffuseMap("textures/CatapultTower.png");
				first = false;
			}*/

			Projectile* pro = nullptr;

			switch (t)
			{
			case TowerType::Arrow:
				pro = new Projectile(source, tar, damage, TowerType::Arrow);
				break;
			case TowerType::Frost:
				pro = new Projectile(source, tar, damage, TowerType::Frost);
				break;
			case TowerType::Fire:
				pro = new Projectile(source, tar, damage, TowerType::Fire);
				break;
			case TowerType::Catapult:
				pro = new Projectile(source, tar, damage, TowerType::Catapult);
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