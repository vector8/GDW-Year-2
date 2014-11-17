#pragma once
#include <vector>
#include <string>
#include "ParticleSystem.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>

namespace flopse
{
	class ParticleManager
	{
	private:
		ParticleManager();
		~ParticleManager();

	public:
		static ParticleManager* getInstance()
		{
			static ParticleManager* pm = new ParticleManager();

			return pm;
		}

		std::vector<ParticleSystem*> particleSystems;

		ParticleSystem* createParticleSystem(ParticleSystemBehaviour behaviour, int rate, int maxParticles, const glm::vec3 &position, const std::string &texturePath = "");

		void update(const sf::Time &dt);

		void draw(Camera* cam, glm::mat4 projection);
	};
}