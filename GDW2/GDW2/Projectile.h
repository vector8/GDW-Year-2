#pragma once
#include "Entity.h"
#include "Enemy.h"

namespace flopse
{
	class Projectile : public Entity
	{
	private:
		glm::vec3 sourcePos;
		Enemy* target = nullptr;
		glm::vec3 targetPos;

		float interpParam = 0.f;
		int damage = 0;

		static Mesh* getProjectileMesh()
		{
			static Mesh* projectileMesh = new Mesh("obj/projectile.obj", new Shader("shaders/texShader.vs", "shaders/grayShader.frag"));

			projectileMesh->overlayColour = Colour(0.5f, 0.f, 0.5f, 1.f);

			return projectileMesh;
		}

	public:
		Projectile(const glm::vec3 &source, Enemy* tar, int damage);
		Projectile(const glm::vec3 &source, glm::vec3 tar, int damage);

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}