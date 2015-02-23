#pragma once
#include "Colour.h"
#include <glm\glm.hpp>

namespace flopse
{
	class Light
	{
	private:

	public:
		Light();
		~Light();

		glm::vec3 position;

		Colour colour;
		
		float ambientComponent = 0.f;
		float diffuseComponent = 0.f;
		float specularComponent = 0.f;
		float specularExponent = 0.f;

		float constantAttenuation = 0.f;
		float linearAttenuation = 0.f;
		float quadraticAttenuation = 0.f;
	};
}