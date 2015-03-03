#pragma once
#include <glm\glm.hpp>

namespace flopse
{
	// Can represent a directional or point light.
	class Light
	{
	private:

	public:
		Light();
		~Light();

		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specularExponent = 0.f;
		float constantAttenuation = 0.f;
		float linearAttenuation = 0.f;
		float quadraticAttenuation = 0.f;
	};
}