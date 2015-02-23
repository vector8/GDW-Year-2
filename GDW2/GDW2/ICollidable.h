#pragma once
#include "BoundingBox.h"

namespace flopse
{
	class ICollidable
	{
	private:
	public:
		ICollidable();
		ICollidable(const glm::vec3 &pos, float width, float height, float depth);
		virtual ~ICollidable();

		void setPosition(const glm::vec3 &pos);
		void setWidth(float width);
		void setHeight(float height);
		void setDepth(float depth);

		BoundingBox boundingBox;
	};
}