#include "SceneNode.h"
#include <glm\gtc\type_ptr.hpp>
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
		updateLocalTransform(window, dt);

		// update global transform
		globalTransform = parentTransform * localTransform.getTransformMatrix();
		
		Node<std::shared_ptr<SceneNode>>* current = children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				current->data->update(window, dt, globalTransform);
			}

			if (current->data->toBeDeleted)
			{
				Node<std::shared_ptr<SceneNode>>* nodeToDelete = current;
				current = current->next;
				this->detach(nodeToDelete->data); 
			}
			else
			{
				current = current->next;
			}
		}
	}

	void SceneNode::draw(const glm::vec3 &camPos, const glm::mat4 &view, const glm::mat4 &projection, const Light &light)
	{
		if (mesh)
		{
			// Set the shader
			mesh->shader->bind();

			glUniformMatrix4fv(mesh->shader->modelLoc, 1, GL_FALSE, glm::value_ptr(globalTransform));
			glUniformMatrix4fv(mesh->shader->viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(mesh->shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
			//glUniform4f(mesh->shader->objectColorLoc, mesh->overlayColour.getR(), mesh->overlayColour.getG(), mesh->overlayColour.getB(), mesh->overlayColour.getA());
			glUniform3f(mesh->shader->lightPosLoc, light.position.x, light.position.y, light.position.z);
			glUniform3f(mesh->shader->viewPosLoc, camPos.x, camPos.y, camPos.z);
			glUniform3f(mesh->shader->lightColourLoc, light.colour.getR(), light.colour.getG(), light.colour.getB());
			glUniform1f(mesh->shader->ambientLoc, light.ambientComponent);
			glUniform1f(mesh->shader->diffuseLoc, light.diffuseComponent);
			glUniform1f(mesh->shader->specularLoc, light.specularComponent);
			glUniform1f(mesh->shader->specularExponentLoc, light.specularExponent);
			glUniform1f(mesh->shader->constantAttenuationLoc, light.constantAttenuation);
			glUniform1f(mesh->shader->linearAttenuationLoc, light.linearAttenuation);
			glUniform1f(mesh->shader->quadraticAttenuationLoc, light.quadraticAttenuation);

			sf::Texture* t = mesh->getTexture();
			sf::Texture::bind(t);

			glBindVertexArray(mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, mesh->getNumberOfVertices());
			glBindVertexArray(GL_NONE);

			// Unbind the texture and shader.
			sf::Texture::bind(NULL);
			mesh->shader->unbind();
		}

		Node<std::shared_ptr<SceneNode>>* current = children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				current->data->draw(camPos, view, projection, light);
			}
			
			current = current->next;
		}
	}

	void SceneNode::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{

	}
}