#include "Entity.h"

namespace flopse
{
	Entity::Entity(Model* model) : ICollidable(glm::vec3(), model->getWidth(), model->getHeight(), model->getDepth()), SceneNode(model)
	{

	}

	Entity::Entity(const glm::vec3 &pos, Model* model) : ICollidable(pos, model->getWidth(), model->getHeight(), model->getDepth()), SceneNode(model)
	{

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

	void Entity::scale(const glm::vec3 &s)
	{
		localTransform.scale(s);

		boundingBox->width *= s.x;
		boundingBox->height *= s.y;
		boundingBox->depth *= s.z;
	}

	glm::vec3 Entity::getGlobalPosition() const
	{
		//glm::vec4 tpos(0.f, 0.f, 0.f, 1.f);

		//tpos = localTransform.getTransformMatrix() * tpos;
		//tpos = globalTransform * tpos;

		//return glm::vec3(tpos[0], tpos[1], tpos[2]);
		return localTransform.getPosition();
	}

	glm::vec3 Entity::getGlobalFront() const
	{
		//glm::vec4 tfront(0.f, 0.f, -1.f, 1.f);

		//tfront = localTransform.getTransformMatrix() * tfront;
		//tfront = globalTransform * tfront;

		//return glm::vec3(tfront[0], tfront[1], tfront[2]);
		return localTransform.getFront();
	}
}