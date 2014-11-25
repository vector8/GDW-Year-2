#include "SceneNode.h"
#include <glm\gtc\type_ptr.hpp>

namespace flopse
{
	SceneNode::SceneNode()
	{

	}

	SceneNode::SceneNode(Mesh *m, SceneNode* parent) : mesh(m), parent(parent)
	{

	}

	SceneNode::~SceneNode()
	{

	}

	void SceneNode::attach(SceneNode* n)
	{
		n->parent = this;
		children.add(n);
	}

	void SceneNode::detach(SceneNode* n)
	{
		n->parent = nullptr;
		children.remove(n);
	}

	void SceneNode::update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform)
	{
		updateLocalTransform(window, dt);

		// update global transform
		globalTransform = parentTransform * localTransform.getTransformMatrix();
		
		Node<SceneNode*>* current = children.head;

		while (current)
		{
			current->data->update(window, dt, globalTransform);

			if (current->data->toBeDeleted)
			{
				Node<SceneNode*>* nodeToDelete = current;
				current = current->next;
				this->detach(nodeToDelete->data); 
			}
			else
			{
				current = current->next;
			}
		}
	}

	void SceneNode::draw(const glm::vec3 &camPos, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &lightPos)
	{
		if (mesh)
		{
			// Set the shader
			mesh->shader->use();

			glUniformMatrix4fv(mesh->shader->modelLoc, 1, GL_FALSE, glm::value_ptr(globalTransform));
			glUniformMatrix4fv(mesh->shader->normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(globalTransform))));
			glUniformMatrix4fv(mesh->shader->viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mesh->shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform4f(mesh->shader->objectColorLoc, mesh->overlayColour.getR(), mesh->overlayColour.getG(), mesh->overlayColour.getB(), mesh->overlayColour.getA());
			glUniform3f(mesh->shader->lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(mesh->shader->viewPosLoc, camPos.x, camPos.y, camPos.z);

			sf::Texture* t = mesh->getTexture();
			sf::Texture::bind(t);

			glBindVertexArray(mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, mesh->getNumberOfVertices());
			glBindVertexArray(0);

			sf::Texture::bind(NULL);
		}

		Node<SceneNode*>* current = children.head;

		for (int i = 0; i < children.size(); i++)
		{
			current->data->draw(camPos, view, projection, lightPos);
			current = current->next;
		}
	}

	void SceneNode::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{

	}
}