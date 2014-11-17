#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include "Game.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>
#include <chrono>
#include <thread>

#include "Model.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Constants.h"

namespace flopse
{
	Game::Game() : window(sf::VideoMode(800, 600), "Project Chainsaw"), running(true), frames(0), projection(glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f))
	{
		glewExperimental = GL_TRUE;
		glewInit();
		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);

		sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
		window.setMouseCursorVisible(false);

		lightPos = glm::vec3(110.f, 90.f, 100.f);

		particleManager = ParticleManager::getInstance();

		initializeEntities();

		cam = new ThirdPersonCamera(player, 10.f);
	}

	void Game::initializeEntities()
	{
		Shader *grayShader = new Shader("shaders/texShader.vs", "shaders/grayShader.frag");
		Shader *texShader = new Shader("shaders/texShader.vs", "shaders/texShader.frag");

		Model *crateModel = new Model(vertices3, 36, texShader);
		crateModel->setTexture("textures/container.jpg");
		Entity *crate = new Entity(crateModel);
		crate->scale(glm::vec3(3.f, 3.f, 3.f));
		crate->translate(glm::vec3(0.f, crateModel->getHeight() / 2, 0.f));

		/*Model *gobModel = new Model("obj/Goblin.obj", texShader);
		gobModel->setTexture("textures/GoblinTexture.png");
		Entity *goblin = new Entity(gobModel);*/

		Model *playerModel = new Model("obj/Goblin.obj", texShader);
		playerModel->setTexture("textures/GoblinTexture.png");
		player = new Player(playerModel);
		player->setPosition(glm::vec3(0.f, 0.1f, 5.f));
		player->translate(glm::vec3(0.f, playerModel->getHeight() / 2, 0.f));
		player->scale(glm::vec3(0.05f, 0.05f, 0.05f));
		player->rotate(90.f, glm::vec3(0.f, 1.f, 0.f));

		Model *terrainModel = new Model("obj/terrain.obj", grayShader);
		Entity *terrain = new Entity(terrainModel);

		terrain->attach(player);
		terrain->attach(crate);
		//terrain->attach(goblin);

		root = terrain;

		colliders.push_back(crate->boundingBox);
		colliders.push_back(new BoundingBox(glm::vec3(-5.5f, 0.f, 4.5f), 3.f, 5.f, 3.f));	// mound1
		colliders.push_back(new BoundingBox(glm::vec3(-4.5f, 0.f, -7.f), 3.f, 5.f, 3.f));	// mound2
		colliders.push_back(new BoundingBox(glm::vec3(0.f, -5.f, 0.f), terrain->model->getWidth(), 10.f, terrain->model->getDepth()));	// floor
	}

	void Game::run()
	{
		//ParticleSystem *s = particleManager->createParticleSystem(ParticleSystemBehaviour::Swarm, 4, 1000, glm::vec3(5.f, 5.f, 5.f));

		clock.restart();

		while (window.isOpen())
		{
			frames++;
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
					window.close();
					running = false;
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
					case sf::Keyboard::Space:
						player->jump();
						break;
					case sf::Keyboard::M:
						//s->position.z *= -1.f;
						break;
					case sf::Keyboard::Escape:
						window.close();
						running = false;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

			if (running)
			{
				elapsed = clock.restart();

				fpsTimer += elapsed;

				cam->update(window, elapsed);

				// Update
				root->update(window, elapsed, glm::mat4());

				particleManager->update(elapsed);

				// Reset mouse to middle of screen
				sf::Mouse::setPosition(sf::Vector2i(400, 300), window);

				// Clear the color / depth buffer
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Draw
				root->draw(cam->getPosition(), cam->view, projection, lightPos);
				//for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
				//{
				//	// Set the shader
				//	(*it)->model->shader->use();

				//	GLint modelLoc = glGetUniformLocation((*it)->model->shader->program, "model");
				//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr((*it)->model->getTransform()));

				//	GLint normalMatrixLoc = glGetUniformLocation((*it)->model->shader->program, "normalMatrix");
				//	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse((*it)->model->getTransform()))));

				//	GLint viewLoc = glGetUniformLocation((*it)->model->shader->program, "view");
				//	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));

				//	GLint projectionLoc = glGetUniformLocation((*it)->model->shader->program, "projection");
				//	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

				//	GLint objectColorLoc = glGetUniformLocation((*it)->model->shader->program, "objectColor");
				//	glUniform4f(objectColorLoc, (*it)->model->overlayColour.getR(), (*it)->model->overlayColour.getG(), (*it)->model->overlayColour.getB(), (*it)->model->overlayColour.getA());

				//	GLint lightPosLoc = glGetUniformLocation((*it)->model->shader->program, "lightPos");
				//	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

				//	GLint viewPosLoc = glGetUniformLocation((*it)->model->shader->program, "viewPos");
				//	glm::vec3 camPos = cam->getPosition();
				//	glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

				//	sf::Texture* t = (*it)->model->getTexture();
				//	sf::Texture::bind(t);

				//	glBindVertexArray((*it)->model->VAO);
				//	glDrawArrays(GL_TRIANGLES, 0, (*it)->model->getNumberOfVertices());
				//	glBindVertexArray(0);

				//	sf::Texture::bind(NULL);
				//}

				particleManager->draw(cam, projection);

				// calculate current fps
				if (fpsTimer.asSeconds() > 1)
				{
					fpsTimer -= sf::seconds(1.0f);
					int fps = frames;
					frames = 0;

					std::cout << fps << std::endl;
				}

				// Swap the buffers
				window.display();

				// Sleep if less than 1/60 second
				/*if (elapsed.asSeconds() < (1.f / 60.f))
				{
				std::this_thread::sleep_for(std::chrono::milliseconds((long)(((1.f/60.f) - elapsed.asSeconds()) * 1000.f)));
				}*/
			}
		}
	}
}