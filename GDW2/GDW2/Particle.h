#pragma once
#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>
#include <vector>
#include "Colour.h"

namespace flopse
{
	class Particle
	{
	private:

	public:
		Particle();
		virtual ~Particle();

		sf::Time lifespan = sf::seconds(1.f);		// Total age
		sf::Time lifetime = sf::seconds(0.f);		// Current age
		glm::vec3 position{ 0.f, 0.f, 0.f };
		glm::vec3 velocity{ 0.f, 0.f, 0.f };
		glm::vec3 acceleration{ 0.f, 0.f, 0.f };
		Colour colourStart{ 0, 0, 0, 255 };
		Colour colourEnd{ 0, 0, 0, 255 };
		float mass = 1.f;
		std::vector<glm::vec3> persistentForces;
		std::vector<glm::vec3> impulseForces;
		float maxSpeed = 1.f;
		float minSpeed = 0.f;

		void update(const sf::Time &dt);

		void reset();
	};
}