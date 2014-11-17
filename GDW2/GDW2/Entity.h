#pragma once
#include "Model.h"
#include "ICollidable.h"
#include "SceneNode.h"
#include <glm\glm.hpp>

namespace flopse
{
	class Entity : public ICollidable, public SceneNode
	{
	public:
		Entity(Model* model);
		Entity(const glm::vec3 &pos, Model* model);

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &t);
		void rotate(float degrees, const glm::vec3 &axis);
		void scale(const glm::vec3 &s);

		glm::vec3 getGlobalPosition() const;
		glm::vec3 Entity::getGlobalFront() const;
	};
}