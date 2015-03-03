#pragma once
#include "ParticleSystem.h"
#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include <memory>

namespace flopse
{
	class PointParticleSystem : public ParticleSystem
	{
	private:
		std::shared_ptr<Shader> s;
		std::shared_ptr<Mesh> mesh;
		Transform transform;

	public:
		PointParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour);
		virtual ~PointParticleSystem();

		std::shared_ptr<Shader> getShader();

		void draw(Camera* cam, const glm::mat4 &projection);
	};
}