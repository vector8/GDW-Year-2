#include "PointParticleSystem.h"
#include <glm\gtc\type_ptr.hpp>
#include "Utilities.h"

namespace flopse
{
	PointParticleSystem::PointParticleSystem(int rate, int maxParticles, const glm::vec3 &position, ParticleSystemBehaviour behaviour) :
		ParticleSystem(rate, maxParticles, position, behaviour)
	{
		this->s = new Shader("shaders/colorShader.vert", "shaders/colorShader.frag");
		GLfloat point[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
		this->mesh = new Mesh(point, 1, s, false, false, true);
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
			// Translate and set colour of the mesh for each particle.
			transform.setPosition((*it)->position);
			//mesh->setPointColour(lerp((*it)->lifetime / (*it)->lifespan, (*it)->colourStart, (*it)->colourEnd)); // Uncomment to set colour.

			// Draw the point.
			mesh->shader->use();

			glUniformMatrix4fv(mesh->shader->modelLoc, 1, GL_FALSE, glm::value_ptr(transform.getTransformMatrix()));
			glUniformMatrix4fv(mesh->shader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(mesh->shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(mesh->VAO);
			glDrawArrays(GL_POINTS, 0, mesh->getNumberOfVertices());
			glBindVertexArray(0);
		}
	}
}