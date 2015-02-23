#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include "Particle.h"
#include "Shader.h"
#include "Camera.h"

namespace flopse
{
	enum class ParticleSystemBehaviour
	{
		Emit,
		Swarm
	};

	class ParticleSystem
	{
	private:

	public:
		ParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour);
		virtual ~ParticleSystem();

		int rate = 0;
		int maxParticles = 0;
		ParticleSystemBehaviour behaviour;

		glm::vec3 position;

		std::vector<Particle*> particles;

		virtual void update(const sf::Time &dt);
		virtual Shader* getShader() = 0;
		virtual void draw(Camera* cam, const glm::mat4 &projection) = 0;
	};
}