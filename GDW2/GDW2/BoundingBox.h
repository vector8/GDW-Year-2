#pragma once
#include <glm\glm.hpp>

class BoundingBox
{
private:
	glm::vec3 pos;
	float width, height, depth;

public:
	BoundingBox(const glm::vec3 &pos, float width, float height, float depth);

	glm::vec3 getPosition() const;
	float getWidth() const;
	float getHeight() const;
	float getDepth() const;

	void setPosition(const glm::vec3 &pos);
	void setWidth(float width);
	void setHeight(float height);
	void setDepth(float depth);

	bool hasCollided(const BoundingBox &other);
};