#include "ParticleManager.h"
#include "PointParticleSystem.h"

namespace flopse
{
	ParticleManager::ParticleManager()
	{
	}

	ParticleManager::~ParticleManager()
	{
	}

	ParticleSystem* ParticleManager::createParticleSystem(ParticleSystemBehaviour behaviour, int rate, int maxParticles, const glm::vec3 &position, const std::string &texturePath)
	{
		ParticleSystem* s;

		if (texturePath.size() > 0)
		{
			// Create TextureParticleSystem
		}
		else
		{
			// Create PointParticleSystem
			s = new PointParticleSystem(rate, maxParticles, position, behaviour);
		}

		this->particleSystems.push_back(s);

		return s;
	}

	void ParticleManager::update(const sf::Time &dt)
	{
		for (std::vector<ParticleSystem*>::iterator it = this->particleSystems.begin(); it != this->particleSystems.end(); it++)
		{
			(*it)->update(dt);
		}
	}

	void ParticleManager::draw(Camera* cam, glm::mat4 projection)
	{
		for (std::vector<ParticleSystem*>::iterator it = this->particleSystems.begin(); it != this->particleSystems.end(); it++)
		{
			(*it)->draw(cam, projection);
		}
	}
}