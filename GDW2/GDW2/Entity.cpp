#include "Entity.h"

namespace flopse
{
	Entity::Entity(Mesh *m) : ICollidable(glm::vec3(), m->getWidth(), m->getHeight(), m->getDepth()), SceneNode(m)
	{

	}

	Entity::Entity(const glm::vec3 &pos, Mesh *m) : ICollidable(pos, m->getWidth(), m->getHeight(), m->getDepth()), SceneNode(m)
	{
		localTransform.setPosition(pos);
	}

	void Entity::setPosition(const glm::vec3 &pos)
	{
		localTransform.setPosition(pos);

		boundingBox->position = pos;
	}

	void Entity::translate(const glm::vec3 &t)
	{
		localTransform.translate(t);

		boundingBox->position += t;
	}

	void Entity::rotate(float degrees, const glm::vec3 &axis)
	{
		localTransform.rotate(degrees, axis);

		// TODO: figure out how to manipulate bounding box for rotate...
	}

	/*void Entity::scale(const glm::vec3 &s)
	{
		localTransform.scale(s);

		boundingBox->width *= s.x;
		boundingBox->height *= s.y;
		boundingBox->depth *= s.z;
	}*/

	glm::vec3 Entity::getGlobalPosition() const
	{
		glm::vec4 tpos(0.f, 0.f, 0.f, 1.f);

		tpos = globalTransform * tpos;

		return glm::vec3(tpos.x, tpos.y, tpos.z);
	}

	glm::vec3 Entity::getGlobalFront() const
	{
		glm::vec4 tfront(0.f, 0.f, -1.f, 0.f);

		tfront = globalTransform * tfront;

		return glm::normalize(glm::vec3(tfront.x, tfront.y, tfront.z));
	}
}