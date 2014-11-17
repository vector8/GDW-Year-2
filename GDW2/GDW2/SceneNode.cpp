#include "SceneNode.h"
#include <glm\gtc\type_ptr.hpp>

namespace flopse
{
	SceneNode::SceneNode()
	{

	}

	SceneNode::SceneNode(Model* model, SceneNode* parent) : model(model), parent(parent)
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
		n->parent = NULL;
		children.remove(n);
	}

	void SceneNode::update(const sf::RenderWindow &window, const sf::Time &dt, const glm::mat4 &parentTransform)
	{
		updateLocalTransform(window, dt);

		// update global transform
		globalTransform = parentTransform * localTransform.getTransformMatrix();
		
		Node<SceneNode*>* current = children.head;

		for (int i = 0; i < children.size(); i++)
		{
			current->data->update(window, dt, globalTransform);
			current = current->next;
		}
	}

	void SceneNode::draw(const glm::vec3 &camPos, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &lightPos)
	{
		if (model)
		{
			// Set the shader
			model->shader->use();

			GLint modelLoc = glGetUniformLocation(model->shader->program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(globalTransform));

			GLint normalMatrixLoc = glGetUniformLocation(model->shader->program, "normalMatrix");
			glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(globalTransform))));

			GLint viewLoc = glGetUniformLocation(model->shader->program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			GLint projectionLoc = glGetUniformLocation(model->shader->program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			GLint objectColorLoc = glGetUniformLocation(model->shader->program, "objectColor");
			glUniform4f(objectColorLoc, model->overlayColour.getR(), model->overlayColour.getG(), model->overlayColour.getB(), model->overlayColour.getA());

			GLint lightPosLoc = glGetUniformLocation(model->shader->program, "lightPos");
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

			GLint viewPosLoc = glGetUniformLocation(model->shader->program, "viewPos");
			glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

			sf::Texture* t = model->getTexture();
			sf::Texture::bind(t);

			glBindVertexArray(model->VAO);
			glDrawArrays(GL_TRIANGLES, 0, model->getNumberOfVertices());
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