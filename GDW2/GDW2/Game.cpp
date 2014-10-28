#include "Game.h"

#include <SFML\OpenGL.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Model.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Constants.h"

Game::Game() : window(sf::VideoMode(800, 600), "Project Chainsaw"), running(true), frames(0), projection(glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f))
{
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
	window.setMouseCursorVisible(false);

	initializeEntities();

	cam = new ThirdPersonCamera(player->getModel(), 10.f);
}

void Game::initializeEntities()
{
	Shader *grayShader = new Shader("shaders/posvshader.vs", "shaders/grayshader.frag");
	Shader *texShader = new Shader("shaders/texshader.vs", "shaders/texshader.frag");

	Model crateModel(vertices3, 36, texShader);
	crateModel.scale(glm::vec3(3.f, 3.f, 3.f));
	crateModel.setTexture("textures/container.jpg");
	crateModel.translate(glm::vec3(0.f, crateModel.getHeight() / 2, 0.f));
	Entity *crate = new Entity(crateModel.getPosition(), crateModel);

	Model tieFighter("obj/tiefighter_merged.obj", grayShader);
	tieFighter.setPosition(glm::vec3(0.f, 0.f, 5.f));
	tieFighter.translate(glm::vec3(0.f, tieFighter.getHeight() / 2, 0.f));
	player = new Player(tieFighter.getPosition(), tieFighter);

	Model terrainModel("obj/terrain.obj", grayShader);
	Entity *terrain = new Entity(terrainModel.getPosition(), terrainModel);
	terrain->setCollidable(false);

	entities.push_back(crate);
	entities.push_back(player);
	entities.push_back(terrain);

	colliders.push_back(crate->getBoundingBox());
	colliders.push_back(new BoundingBox(glm::vec3(-5.5f, 0.f, 4.5f), 3.f, 5.f, 3.f));	// mound1
	colliders.push_back(new BoundingBox(glm::vec3(-4.5f, 0.f, -7.f), 3.f, 5.f, 3.f));	// mound2
	colliders.push_back(new BoundingBox(glm::vec3(0.f, -1.f, 0.f), terrain->getModel()->getWidth(), 1.99f, terrain->getModel()->getDepth()));	// floor
}

void Game::run()
{
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

			player->update(window, elapsed);

			// update cam
			cam->update(window, elapsed);

			// Reset mouse to middle of screen
			sf::Mouse::setPosition(sf::Vector2i(400, 300), window);

			// Clear the color / depth buffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw each model
			for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
			{
				// Set the shader
				(*it)->getModel()->shader->use();

				GLint modelLoc = glGetUniformLocation((*it)->getModel()->shader->program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr((*it)->getModel()->getTransform()));

				GLint normalMatrixLoc = glGetUniformLocation((*it)->getModel()->shader->program, "normalMatrix");
				glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse((*it)->getModel()->getTransform()))));

				GLint viewLoc = glGetUniformLocation((*it)->getModel()->shader->program, "view");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));

				GLint projectionLoc = glGetUniformLocation((*it)->getModel()->shader->program, "projection");
				glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

				GLint objectColorLoc = glGetUniformLocation((*it)->getModel()->shader->program, "objectColor");
				glUniform3f(objectColorLoc, 1.f, 1.f, 1.f);

				GLint lightPosLoc = glGetUniformLocation((*it)->getModel()->shader->program, "lightPos");
				glUniform3f(lightPosLoc, 110.f, 90.f, 100.f);

				GLint viewPosLoc = glGetUniformLocation((*it)->getModel()->shader->program, "viewPos");
				glm::vec3 camPos = cam->getPosition();
				glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

				sf::Texture* t = (*it)->getModel()->getTexture();
				sf::Texture::bind(t);

				glBindVertexArray((*it)->getModel()->VAO);
				glDrawArrays(GL_TRIANGLES, 0, (*it)->getModel()->getNumberOfVertices());
				glBindVertexArray(0);

				sf::Texture::bind(NULL);
			}

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
		}
	}
}