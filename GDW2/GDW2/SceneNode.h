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
		Colour overlayColour;
		bool usesCustomDraw = false;

		bool toBeDeleted = false;
		
		virtual void attach(const std::shared_ptr<SceneNode> &n);
		void detach(const std::shared_ptr<SceneNode> &n);

		void update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform);

		// THIS METHOD MUST ONLY USE LOCAL TRANSFORM, IT MUST NEVER USE THE GLOBAL TRANSFORM!!!
		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		// Update stuff that needs to use the global transform in this method.
		virtual void postUpdate(const sf::RenderWindow &window, const sf::Time &dt);

		glm::mat4 getInverseGlobalTransform();

		glm::vec3 getGlobalPosition() const;
		glm::vec3 getGlobalFront() const;

		// Overwriteable method in case a node needs different behaviour than the default (such as particle system)
		virtual void draw();
	};
}