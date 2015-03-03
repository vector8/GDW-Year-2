#pragma once
#include <glm\glm.hpp>
#include <memory>
#include "Mesh.h"
#include "LinkedList.h"
#include "Transform.h"

namespace flopse
{
	class SceneNode
	{
	public:
		SceneNode();
		SceneNode(std::shared_ptr<Mesh> m, SceneNode* parent = nullptr);
		virtual ~SceneNode();

		Transform localTransform;
		glm::mat4 globalTransform;
		LinkedList<std::shared_ptr<SceneNode>> children;
		SceneNode* parent = nullptr;
		std::shared_ptr<Mesh> mesh = nullptr;

		bool toBeDeleted = false;
		
		void attach(const std::shared_ptr<SceneNode> &n);
		void detach(const std::shared_ptr<SceneNode> &n);

		void update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform);

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		glm::mat4 getInverseGlobalTransform();
	};
}