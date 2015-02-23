#include "ICollidable.h"

namespace flopse
{
	ICollidable::ICollidable() : boundingBox(glm::vec3(), 0.f, 0.f, 0.f)
	{
	}

	ICollidable::ICollidable(const glm::vec3 &pos, float width, float height, float depth) : boundingBox(pos, width, height, depth)
	{
	}

	ICollidable::~ICollidable()
	{
	}

	void ICollidable::setPosition(const glm::vec3 &pos)
	{
		boundingBox.position = pos;
	}

	void ICollidable::setWidth(float width)
	{
		boundingBox.width = width;
	}

	void ICollidable::setHeight(float height)
	{
		boundingBox.height = height;
	}

	void ICollidable::setDepth(float depth)
	{
		boundingBox.depth = depth;
	}
}