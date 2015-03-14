#pragma once
#include <glm\glm.hpp>
#include "SceneNode.h"

namespace flopse
{
	// Can represent a directional or point light.
	class Light : public SceneNode
	{
	private:

	public:
		Light();
		~Light();

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specularExponent = 0.f;
		float constantAttenuation = 0.f;
		float linearAttenuation = 0.f;
		float quadraticAttenuation = 0.f;
	};
}