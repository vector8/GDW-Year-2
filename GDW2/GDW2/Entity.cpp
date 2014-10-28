#include "Entity.h"

Entity::Entity(const glm::vec3 &pos, const Model &model) : pos(pos), model(new Model(model)), box(new BoundingBox(pos, model.getWidth(), model.getHeight(), model.getDepth())), collidable(true)
{

}

BoundingBox* Entity::getBoundingBox() const
{
	return box;
}

glm::vec3 Entity::getPosition() const
{
	return pos;
}

Model* Entity::getModel() const
{
	return model;
}

bool Entity::isCollidable() const
{
	return collidable;
}

void Entity::setCollidable(bool c)
{
	collidable = c;
}

void Entity::update(const sf::Window &window, const sf::Time &dt)
{

}