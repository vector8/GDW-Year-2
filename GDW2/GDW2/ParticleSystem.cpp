#include "ParticleSystem.h"
#include <iostream>
#include "Utilities.h"
#include <glm\gtx\vector_angle.hpp>

namespace flopse
{
	ParticleSystem::ParticleSystem(int rate, int maxParticles, const std::string &textureFile) : 
		rate(rate), maxParticles(maxParticles)
	{
		usesCustomDraw = true;

		texture = sf::Texture();
		if (!texture.loadFromFile(textureFile))
		{
			std::cout << "!!!!ERROR LOADING PARTICLE TEXTURE!!!!" << std::endl;
		}

		particles.positions = new glm::vec3[maxParticles];
		particles.velocities = new glm::vec3[maxParticles];
		particles.sizes = new float[maxParticles];
		particles.alphas = new float[maxParticles];
		particles.ages = new float[maxParticles];
		particles.lifetimes = new float[maxParticles];

		// Set up OpenGL memory
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vboPosition);
		glGenBuffers(1, &vboSize);
		glGenBuffers(1, &vboAlpha);

		// Position
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		// Size
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboSize);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		// Alpha
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, vboAlpha);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		glBindVertexArray(GL_NONE);
	}

	ParticleSystem::~ParticleSystem()
	{
		if (vao != GL_NONE)
		{
			glDeleteVertexArrays(1, &vao);
			vao = GL_NONE;
		}

		if (vboPosition != GL_NONE)
		{
			glDeleteBuffers(1, &vboPosition);
			vboPosition = GL_NONE;
		}

		if (vboSize != GL_NONE)
		{
			glDeleteBuffers(1, &vboSize);
			vboSize = GL_NONE;
		}

		if (vboAlpha != GL_NONE)
		{
			glDeleteBuffers(1, &vboAlpha);
			vboAlpha = GL_NONE;
		}

		if (particles.positions != nullptr)
		{
			delete[] particles.positions;
			particles.positions = nullptr;
			delete[] particles.velocities;
			particles.velocities = nullptr;
			delete[] particles.sizes;
			particles.sizes = nullptr;
			delete[] particles.alphas;
			particles.alphas = nullptr;
			delete[] particles.ages;
			particles.ages = nullptr;
			delete[] particles.lifetimes;
			particles.lifetimes = nullptr;
		}
	}

	void ParticleSystem::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		int numToSpawn = rate;

		// Generate new particles
		while (numToSpawn > 0 && numCurrentParticles < maxParticles)
		{
			particles.alphas[numCurrentParticles] = lerpAlpha.x;
			particles.ages[numCurrentParticles] = 0.f;
			particles.lifetimes[numCurrentParticles] = randomRangef(rangeLifetime.x, rangeLifetime.y);
			particles.sizes[numCurrentParticles] = lerpSize.x;
			particles.positions[numCurrentParticles] = glm::vec3(randomRangef(rangeX.x, rangeX.y), randomRangef(rangeY.x, rangeY.y), randomRangef(rangeZ.x, rangeZ.y));

			// Send the particle in a random direction, with a velocity between our range.
			particles.velocities[numCurrentParticles] = randomRangef(rangeVelocity.x, rangeVelocity.y) * glm::normalize(glm::vec3(randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f), randomRangef(-1.f, 1.f)));

			numCurrentParticles++;
			numToSpawn--;
		}

		// update existing particles
		for (int i = 0; i < numCurrentParticles; i++)
		{
			particles.ages[i] += dt.asSeconds();

			if (particles.ages[i] > particles.lifetimes[i])
			{
				// remove the particle by replacing it with the one at the top of the stack.
				particles.alphas[i] = particles.alphas[numCurrentParticles - 1];
				particles.ages[i] = particles.ages[numCurrentParticles - 1];
				particles.lifetimes[i] = particles.lifetimes[numCurrentParticles - 1];
				particles.sizes[i] = particles.sizes[numCurrentParticles - 1];
				particles.positions[i] = particles.positions[numCurrentParticles - 1];
				particles.velocities[i] = particles.velocities[numCurrentParticles - 1];

				numCurrentParticles--;
			}

			particles.positions[i] += particles.velocities[i] * dt.asSeconds();

			float interp = particles.ages[i] / particles.lifetimes[i];

			particles.alphas[i] = lerp(interp, lerpAlpha.x, lerpAlpha.y);
			particles.sizes[i] = lerp(interp, lerpSize.x, lerpSize.y);
		}

		// Update OpenGL on the changes
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * numCurrentParticles, &particles.positions[0]);

		glBindBuffer(GL_ARRAY_BUFFER, vboSize);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numCurrentParticles, &particles.sizes[0]);

		glBindBuffer(GL_ARRAY_BUFFER, vboAlpha);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numCurrentParticles, &particles.alphas[0]);

		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}

	void ParticleSystem::draw()
	{
		if (numCurrentParticles == 0)
		{
			return;
		}

		sf::Texture::bind(&texture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, numCurrentParticles);
		glBindVertexArray(GL_NONE);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		sf::Texture::bind(NULL);
	}
}