#include "ParticleManager.h"

namespace flopse
{
	ParticleManager::ParticleManager()
	{
	}

	ParticleManager::~ParticleManager()
	{
	}

	std::shared_ptr<ParticleSystem> ParticleManager::createParticleSystem(ParticleSystemBehaviour behaviour, int rate, int maxParticles, const glm::vec3 &position, const std::string &texturePath)
	{
		std::shared_ptr<ParticleSystem> s = nullptr;

		if (texturePath.size() > 0)
		{
			// Create TextureParticleSystem
		}
		else
		{
			// Create PointParticleSystem
			//s = std::make_shared<PointParticleSystem>(rate, maxParticles, position, behaviour);
		}

		this->particleSystems.push_back(s);

		return s;
	}

	void ParticleManager::update(const sf::Time &dt)
	{
		for (std::vector<std::shared_ptr<ParticleSystem>>::iterator it = this->particleSystems.begin(); it != this->particleSystems.end(); it++)
		{
			//(*it)->update(dt);
		}
	}

	void ParticleManager::draw(Camera* cam, glm::mat4 projection)
	{
		for (std::vector<std::shared_ptr<ParticleSystem>>::iterator it = this->particleSystems.begin(); it != this->particleSystems.end(); it++)
		{
			//(*it)->draw(cam, projection);
		}
	}
}