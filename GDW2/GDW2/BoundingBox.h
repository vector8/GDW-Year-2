#pragma once
#include <glm\glm.hpp>

namespace flopse
{
	class BoundingBox
	{
	public:
		BoundingBox(const glm::vec3 &pos, float width, float height, float depth);
		virtual ~BoundingBox();

		glm::vec3 position;

		float width, height, depth;

		bool hasCollided(const BoundingBox &other);
	};
}