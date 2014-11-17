#include "ParticleSystem.h"
#include <iostream>
#include "Utilities.h"
#include <glm\gtx\vector_angle.hpp>

namespace flopse
{
	ParticleSystem::ParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour) : 
		rate(rate), maxParticles(maxParticles), position(position), behaviour(behaviour)
	{
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::update(const sf::Time &dt)
	{
		int numToSpawn = 0;
		
		if (behaviour == ParticleSystemBehaviour::Emit)
		{
			numToSpawn = rate;

			// Generate new particles
			while (numToSpawn > 0 && particles.size() < maxParticles)
			{
				Particle* p = new Particle();
				p->position = position;
				float speed = randomRangef(2.f, 5.f);
				glm::vec3 velocity(randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f));
				velocity *= speed;
				p->velocity = velocity;
				p->lifespan = sf::seconds(randomRangef(0.25f, 0.75f));
				p->persistentForces.push_back(glm::vec3(0.f, 5.f, 0.f));

				particles.push_back(p);

				numToSpawn--;
			}

			// update existing particles
			for (std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
			{
				(*it)->update(dt);

				if ((*it)->lifetime >= (*it)->lifespan)
				{
					// Reset the dead particle.
					(*it)->reset();
					(*it)->position = position;
					float speed = randomRangef(2.f, 5.f);
					glm::vec3 velocity(randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f));
					velocity *= speed;
					(*it)->velocity = velocity;
					(*it)->lifespan = sf::seconds(randomRangef(0.25f, 0.75f));
					(*it)->persistentForces.push_back(glm::vec3(0.f, 5.f, 0.f));
				}
			}
		}
		else if (behaviour == ParticleSystemBehaviour::Swarm)
		{
			numToSpawn = maxParticles;

			// Generate new particles
			while (numToSpawn > 0 && particles.size() < maxParticles)
			{
				Particle* p = new Particle();
				p->position = glm::vec3(randomRangef(-1.f, 1.f) + this->position.x, randomRangef(-1.f, 1.f) + this->position.y, randomRangef(-1.f, 1.f) + this->position.z);
				p->minSpeed = randomRangef(0.1f, 0.5f);
				p->maxSpeed = randomRangef(3.f, 5.f);

				particles.push_back(p);

				numToSpawn--;
			}

			// update existing particles
			for (std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
			{
				// create impulse force to cause the particle to move towards the centre of the swarm in a natural-looking way.
				glm::vec3 direction;

				direction = (position - (*it)->position) + glm::cross((*it)->velocity, position - (*it)->position);

				//direction = glm::normalize(direction);

				(*it)->impulseForces.push_back(direction);
				
				(*it)->update(dt);
			}
		}
	}
}