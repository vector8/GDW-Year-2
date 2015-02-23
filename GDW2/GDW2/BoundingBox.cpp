#include "BoundingBox.h"

namespace flopse
{
	BoundingBox::BoundingBox(const glm::vec3 &pos, float width, float height, float depth) : position(pos), width(width), height(height), depth(depth)
	{

	}

	BoundingBox::~BoundingBox()
	{

	}

	bool BoundingBox::hasCollided(const BoundingBox &other)
	{
		bool overlapsX = false, overlapsY = false, overlapsZ = false;

		// when performing this calculation, we assume that pos is at the center of the bottom plane of the bounding box.
		// models should be created to reflect this, otherwise we need to update this method accordingly.

		// they overlap in any given direction if the distance between their points (in that direction) is less than //or equal to
		// the sum of the radii of those boxes along that axis (i.e., half of the width for the x direction).
		overlapsX = abs(other.position.x - this->position.x) < ((this->width / 2.f) + (other.width / 2.f));
		overlapsY = abs((other.position.y + other.height / 2.f) - (this->position.y + this->height / 2.f)) < ((this->height / 2.f) + (other.height / 2.f));
		overlapsZ = abs(other.position.z - this->position.z) < ((this->depth / 2.f) + (other.depth / 2.f));

		// two bounding boxes have collided only if they overlap in all three of x, y and z directions.
		return overlapsX && overlapsY && overlapsZ;
	}
}