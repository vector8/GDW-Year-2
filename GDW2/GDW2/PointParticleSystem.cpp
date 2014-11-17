#include "PointParticleSystem.h"
#include <glm\gtc\type_ptr.hpp>
#include "Utilities.h"

namespace flopse
{
	PointParticleSystem::PointParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour) :
		ParticleSystem(rate, maxParticles, position, behaviour)
	{
		this->s = new Shader("shaders/colorShader.vs", "shaders/colorShader.frag");
		GLfloat point[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
		this->model = new Model(point, 1, s, false, false, true);
	}

	PointParticleSystem::~PointParticleSystem()
	{

	}

	Shader* PointParticleSystem::getShader()
	{
		return s;
	}

	void PointParticleSystem::draw(Camera* cam, const glm::mat4 &projection)
	{
		for (std::vector<Particle*>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			// Translate and set colour of the model for each particle.
			transform.setPosition((*it)->position);
			//model->setPointColour(lerp((*it)->lifetime / (*it)->lifespan, (*it)->colourStart, (*it)->colourEnd)); // Uncomment to set colour.

			// Draw the point.
			model->shader->use();

			GLint modelLoc = glGetUniformLocation(model->shader->program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()));

			GLint viewLoc = glGetUniformLocation(model->shader->program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));

			GLint projectionLoc = glGetUniformLocation(model->shader->program, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(model->VAO);
			glDrawArrays(GL_POINTS, 0, model->getNumberOfVertices());
			glBindVertexArray(0);
		}
	}
}