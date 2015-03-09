#include "Entity.h"

namespace flopse
{
	Entity::Entity(std::shared_ptr<Mesh> m) : ICollidable(glm::vec3(), m->getWidth(), m->getHeight(), m->getDepth()), SceneNode(m)
	{

	}

	Entity::Entity(const glm::vec3 &pos, std::shared_ptr<Mesh> m) : ICollidable(pos, m->getWidth(), m->getHeight(), m->getDepth()), SceneNode(m)
	{
		localTransform.setPosition(pos);
	}

	Entity::~Entity()
	{

	}

	void Entity::setPosition(const glm::vec3 &pos)
	{
		localTransform.setPosition(pos);

		boundingBox.position = pos;
	}

	void Entity::translate(const glm::vec3 &t)
	{
		localTransform.translate(t);

		boundingBox.position += t;
	}

	void Entity::rotate(float degrees, const glm::vec3 &axis)
	{
		localTransform.rotate(degrees, axis);

		// TODO: figure out how to manipulate bounding box for rotate...
	}

	/*void Entity::scale(const glm::vec3 &s)
	{
		localTransform.scale(s);

		boundingBox.width *= s.x;
		boundingBox.height *= s.y;
		boundingBox.depth *= s.z;
	}*/
}