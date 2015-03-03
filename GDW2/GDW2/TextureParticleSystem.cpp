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

		this->s = std::make_shared<Shader>("shaders/ambientTexShader.vert", "shaders/ambientTexShader.frag");
	}

	TextureParticleSystem::~TextureParticleSystem()
	{

	}

	std::shared_ptr<Shader> TextureParticleSystem::getShader()
	{
		return s;
	}
}