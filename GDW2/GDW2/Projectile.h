#pragma once
#include "Entity.h"
#include "Enemy.h"

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

		static std::shared_ptr<Mesh> getProjectileMesh()
		{
			static std::shared_ptr<Mesh> projectileMesh = std::make_shared<Mesh>("meshes/projectile.bmf", Shader::getStandardShader(StandardShaders::PhongNoTexture));

			projectileMesh->overlayColour = Colour(0.5f, 0.f, 0.5f, 1.f);

			return projectileMesh;
		}

	public:
		Projectile(const glm::vec3 &source, std::shared_ptr<Enemy> tar, int damage);
		Projectile(const glm::vec3 &source, glm::vec3 tar, int damage);
		virtual ~Projectile();

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}