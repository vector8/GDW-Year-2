#pragma once
#include "ParticleSystem.h"
#include "Shader.h"

namespace flopse
{
	class TextureParticleSystem : public ParticleSystem
	{
	private:
		Shader* s;

	public:
		TextureParticleSystem(const std::string &texturePath, int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour);
		~TextureParticleSystem();

		sf::Texture texture;

		//void draw(sf::RenderWindow window);
		Shader* getShader();
	};
}