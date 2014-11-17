#pragma once
#include "ParticleSystem.h"
#include "Shader.h"
#include "Model.h"
#include "Transform.h"

namespace flopse
{
	class PointParticleSystem : public ParticleSystem
	{
	private:
		Shader* s;
		Model* model;
		Transform transform;

	public:
		PointParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour);
		~PointParticleSystem();

		Shader* getShader();

		void draw(Camera* cam, const glm::mat4 &projection);
	};
}