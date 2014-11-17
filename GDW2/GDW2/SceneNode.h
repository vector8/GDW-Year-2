#pragma once
#include <glm\glm.hpp>
#include "Model.h"
#include "LinkedList.h"
#include "Transform.h"

namespace flopse
{
	class SceneNode
	{
	public:
		SceneNode();
		SceneNode(Model* model, SceneNode* parent = NULL);
		~SceneNode();

		Transform localTransform;
		glm::mat4 globalTransform;
		LinkedList<SceneNode*> children;
		SceneNode* parent;
		Model *model;
		
		void attach(SceneNode* n);
		void detach(SceneNode* n);

		void update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform);
		void draw(const glm::vec3 &camPos, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &lightPos);

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}