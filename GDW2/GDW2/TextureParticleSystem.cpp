#include "TextureParticleSystem.h"
#include <iostream>

namespace flopse
{
	TextureParticleSystem::TextureParticleSystem(const std::string &texturePath, int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour) :
	ParticleSystem(rate, maxParticles, position, behaviour)
	{
		if (!texture.loadFromFile(texturePath))
		{
			std::cout << "Error loading image file" << std::endl;
		}

		this->s = new Shader("shaders/ambientTexShader.vs", "shaders/ambientTexShader.frag");
	}

	TextureParticleSystem::~TextureParticleSystem()
	{

	}

	Shader* TextureParticleSystem::getShader()
	{
		return s;
	}
}