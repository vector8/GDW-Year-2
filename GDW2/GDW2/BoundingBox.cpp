#include "BoundingBox.h"

BoundingBox::BoundingBox(const glm::vec3 &pos, float width, float height, float depth) : pos(pos), width(width), height(height), depth(depth)
{

}

glm::vec3 BoundingBox::getPosition() const
{
	return pos;
}

float BoundingBox::getWidth() const
{
	return width;
}

float BoundingBox::getHeight() const
{
	return height;
}

float BoundingBox::getDepth() const
{
	return depth;
}

void BoundingBox::setPosition(const glm::vec3 &pos)
{
	this->pos = glm::vec3(pos);
}

void BoundingBox::setWidth(float width)
{
	this->width = width;
}

void BoundingBox::setHeight(float height)
{
	this->height = height;
}

void BoundingBox::setDepth(float depth)
{
	this->depth = depth;
}

bool BoundingBox::hasCollided(const BoundingBox &other)
{
	bool overlapsX = false, overlapsY = false, overlapsZ = false;

	// when performing this calculation, we assume that pos is at the very centre of the bounding box.
	// models should be created to reflect this, otherwise we need to update this method accordingly.

	// they overlap in any given direction if the distance between their points (in that direction) is less than or equal to
	// the sum of the radii of those boxes along that axis (i.e., half of the width for the x direction).
	overlapsX = abs(other.getPosition().x - this->pos.x) <= ((this->width / 2.f) + (other.getWidth() / 2.f));
	overlapsY = abs(other.getPosition().y - this->pos.y) <= ((this->height / 2.f) + (other.getHeight() / 2.f));
	overlapsZ = abs(other.getPosition().z - this->pos.z) <= ((this->depth / 2.f) + (other.getDepth() / 2.f));

	// two bounding boxes have collided only if they overlap in all three of x, y and z directions.
	return overlapsX && overlapsY && overlapsZ;
}