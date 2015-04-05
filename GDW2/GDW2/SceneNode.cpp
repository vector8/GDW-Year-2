#include "SceneNode.h"
#include <iostream>

namespace flopse
{
	SceneNode::SceneNode()
	{

	}

	SceneNode::SceneNode(std::shared_ptr<Mesh> m, SceneNode* parent) : mesh(m), parent(parent)
	{

	}

	SceneNode::~SceneNode()
	{
	}

	void SceneNode::attach(const std::shared_ptr<SceneNode> &n)
	{
		n->parent = this;
		children.add(n);
	}

	void SceneNode::detach(const std::shared_ptr<SceneNode> &n)
	{
		n->parent = nullptr;
		if (children.remove(n))
		{
			std::cout << "Node deleted." << std::endl;
		}
		else
		{
			std::cout << "Node deletion failed." << std::endl;
		}
	}

	void SceneNode::update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform)
	{
		// update local transform
		updateLocalTransform(window, dt);

		// update global transform
		globalTransform = parentTransform * localTransform.getTransformMatrix();

		// execute any logic required after global transform is updated
		postUpdate(window, dt);
		
		std::shared_ptr<Node<std::shared_ptr<SceneNode>>> current = children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				current->data->update(window, dt, globalTransform);
			}

			if (current->data->toBeDeleted)
			{
				std::shared_ptr<Node<std::shared_ptr<SceneNode>>> nodeToDelete = current;
				current = current->next;
				this->detach(nodeToDelete->data); 
			}
			else
			{
				current = current->next;
			}
		}
	}

	void SceneNode::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{

	}

	void SceneNode::postUpdate(const sf::RenderWindow &window, const sf::Time &dt)
	{

	}

	glm::mat4 SceneNode::getInverseGlobalTransform()
	{
		glm::mat4 inverse;

		glm::mat3 rotation(globalTransform[0][0], globalTransform[0][1], globalTransform[0][2], 
							globalTransform[1][0], globalTransform[1][1], globalTransform[1][2], 
							globalTransform[2][0], globalTransform[2][1], globalTransform[2][2]);

		glm::vec3 translate(globalTransform[3][0], globalTransform[3][1], globalTransform[3][2]);

		rotation = glm::transpose(rotation);

		translate = -1.f * (rotation * translate);

		inverse[0][0] = rotation[0][0];	inverse[1][0] = rotation[1][0];	inverse[2][0] = rotation[2][0];	inverse[3][0] = translate[0];
		inverse[0][1] = rotation[0][1];	inverse[1][1] = rotation[1][1];	inverse[2][1] = rotation[2][1];	inverse[3][1] = translate[1];
		inverse[0][2] = rotation[0][2];	inverse[1][2] = rotation[1][2];	inverse[2][2] = rotation[2][2];	inverse[3][2] = translate[2];
		inverse[0][3] = 0.f;			inverse[1][3] = 0.f;			inverse[2][3] = 0.f;			inverse[3][3] = 1.f;

		return inverse;
	}

	glm::vec3 SceneNode::getGlobalPosition() const
	{
		glm::vec4 tpos(0.f, 0.f, 0.f, 1.f);

		tpos = globalTransform * tpos;

		return glm::vec3(tpos.x, tpos.y, tpos.z);
	}

	glm::vec3 SceneNode::getGlobalFront() const
	{
		glm::vec4 tfront(0.f, 0.f, 1.f, 0.f);

		tfront = globalTransform * tfront;

		return glm::normalize(glm::vec3(tfront.x, tfront.y, tfront.z));
	}

	// Overwriteable method in case a node needs different behaviour than the default (such as particle system)
	void SceneNode::draw()
	{

	}
}