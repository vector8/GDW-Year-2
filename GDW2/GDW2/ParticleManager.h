#pragma once
#include <vector>
#include <string>
#include "ParticleSystem.h"
#include "Camera.h"
#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include <memory>

namespace flopse
{
	class ParticleManager
	{
	private:
		ParticleManager();

	public:
		virtual ~ParticleManager();

		static ParticleManager* getInstance()
		{
			static ParticleManager* pm = new ParticleManager();

			return pm;
		}

		std::vector<std::shared_ptr<ParticleSystem>> particleSystems;

		std::shared_ptr<ParticleSystem> createParticleSystem(ParticleSystemBehaviour behaviour, int rate, int maxParticles, const glm::vec3 &position, const std::string &texturePath = "");

		void update(const sf::Time &dt);

		void draw(Camera* cam, glm::mat4 projection);
	};
}