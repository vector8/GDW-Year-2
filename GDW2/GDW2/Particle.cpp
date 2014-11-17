#include "Particle.h"

namespace flopse
{
	Particle::Particle()
	{

	}

	Particle::~Particle()
	{

	}

	void Particle::update(const sf::Time &dt)
	{
		glm::vec3 sumForces(0.f, 0.f, 0.f);
		for (int i = 0; i < persistentForces.size(); i++)
		{
			sumForces += persistentForces[i];
		}
		for (int i = 0; i < impulseForces.size(); i++)
		{
			sumForces += impulseForces[i];
		}
		impulseForces.clear();

		acceleration = sumForces / mass;

		velocity = velocity + acceleration * dt.asSeconds();
		float speed = glm::length(velocity);
		if (speed > maxSpeed)
		{
			velocity = maxSpeed * glm::normalize(velocity);
		}
		else if (speed < minSpeed)
		{
			velocity = minSpeed * glm::normalize(velocity);
		}

		position = position + velocity * dt.asSeconds() + 0.5f * acceleration * dt.asSeconds() * dt.asSeconds();
		lifetime += dt;
	}

	void Particle::reset()
	{
		lifespan = sf::seconds(1.f);
		lifetime = sf::seconds(0.f);
		position.x = 0.f;
		position.y = 0.f;
		position.z = 0.f;
		velocity.x = 0.f;
		velocity.y = 0.f;
		velocity.z = 0.f;
		acceleration.x = 0.f;
		acceleration.y = 0.f;
		acceleration.z = 0.f;
		colourStart.setR(255);
		colourStart.setG(255);
		colourStart.setB(255);
		colourStart.setA(255);
		colourEnd.setR(255);
		colourEnd.setG(255);
		colourEnd.setB(255);
		colourEnd.setA(255);
		mass = 1.f;
		persistentForces.clear();
		impulseForces.clear();
		maxSpeed = 1.f;
		minSpeed = 0.f;
	}
}