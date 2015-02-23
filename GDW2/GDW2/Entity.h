#pragma once
#include "Mesh.h"
#include "ICollidable.h"
#include "SceneNode.h"
#include <glm\glm.hpp>

namespace flopse
{
	class Entity : public ICollidable, public SceneNode
	{
	public:
		Entity(std::shared_ptr<Mesh> m);
		Entity(const glm::vec3 &pos, std::shared_ptr<Mesh> m);
		virtual ~Entity();

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &t);
		void rotate(float degrees, const glm::vec3 &axis);
		//void scale(const glm::vec3 &s);

		glm::vec3 getGlobalPosition() const;
		glm::vec3 Entity::getGlobalFront() const;
	};
}