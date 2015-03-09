#pragma once
#include <glm\glm.hpp>
#include "Shader.h"
#include "Camera.h"

namespace flopse
{
	struct ParticleList
	{
		glm::vec3 *positions = nullptr;
		glm::vec3 *velocities = nullptr;
		float *sizes = nullptr;
		float *alphas = nullptr;
		float *ages = nullptr;
		float *lifetimes = nullptr;
	};

	enum class ParticleSystemBehaviour
	{
		Emit,
		Swarm
	};

	class ParticleSystem : public SceneNode
	{
	private:
		ParticleList particles;
		sf::Texture texture;

		int rate = 0;
		int maxParticles = 0;
		int numCurrentParticles = 0;

		GLuint vao = GL_NONE;
		GLuint vboPosition = GL_NONE;
		GLuint vboSize = GL_NONE;
		GLuint vboAlpha = GL_NONE;

	public:
		ParticleSystem(int rate, int maxParticles, const std::string &textureFile);
		virtual ~ParticleSystem();

		glm::vec2 rangeX = glm::vec2(-10.f, 10.f);
		glm::vec2 rangeY = glm::vec2(0.f, 10.f);
		glm::vec2 rangeZ = glm::vec2(-10.f, 10.f);
		glm::vec2 rangeVelocity = glm::vec2(0.1f, 1.f);
		glm::vec2 rangeLifetime = glm::vec2(7.f, 9.5f);
		// lerp variables are dynamic based on lifetime of particle
		glm::vec2 lerpAlpha = glm::vec2(0.5f, 0.f);
		glm::vec2 lerpSize = glm::vec2(0.f, 1.f);

		ParticleSystemBehaviour behaviour;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
		virtual void draw();
	};
}