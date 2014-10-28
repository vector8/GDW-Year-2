#pragma once
#include "Model.h"
#include <glm\glm.hpp>
#include "BoundingBox.h"

class Entity
{
protected:
	Model *model;
	BoundingBox* box;
	glm::vec3 pos;
	bool collidable;

public:
	Entity(const glm::vec3 &pos, const Model &model);

	BoundingBox* getBoundingBox() const;
	glm::vec3 getPosition() const;
	Model* getModel() const;
	bool isCollidable() const;

	void setCollidable(bool c);

	virtual void update(const sf::Window &window, const sf::Time &dt);
};