#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Test");
	bool running = true;
	sf::Clock clock;
	sf::Time elapsed, fpsTimer;
	bool paused = false;

	// FPS STUFF
	int frames = 0;
	// END FPS STUFF

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 800, 600);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 projection;
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

	FirstPersonCamera fpcam;
	fpcam.lookAt(glm::vec3(0.f, 0.f, 0.f));

	sf::Mouse::setPosition(sf::Vector2i(400, 300), window);
	window.setMouseCursorVisible(false);

	/*GLfloat vertices[] = 
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 0
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,	// 1
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	// 2
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// 3
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// 4
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	// 5
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,		// 6
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 7
	};*/

	GLfloat rubiksCube[] =
	{
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,	// 0
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,	// 1
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,	// 2
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,	// 0
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,	// 2
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,	// 3
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	// 1
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	// 2
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,		// 6
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	// 1
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,	// 5
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,		// 6
		0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f,	// 2
		0.5f, -0.5f, -0.5f, 1.0f, 0.5f, 0.0f,	// 3
		0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.0f,	// 7
		0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f,	// 2
		0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.0f,		// 6
		0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.0f,	// 7
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 0
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 3
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 7
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 0
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 4
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	// 7
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// 0
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,	// 1
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,	// 5
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// 0
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,	// 4
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,	// 5
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,	// 4
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,	// 5
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,		// 6
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,	// 4
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,		// 6
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f		// 7
	};

	GLfloat vertices3[] = {
		// vertex					uv			normal
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
										
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
										
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
										
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
										
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	GLfloat vertices2[] = 
	{
		-0.5f, -0.5f, 0.0f,		1.0f, 0.73f, 0.0f,
		0.0f, -0.5f, 0.0f,		1.0f, 0.65f, 0.0f,
		-0.25f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f, 
		-0.25f, 0.0f, 0.0f,		1.0f, 0.73f, 0.0f,
		0.25f, 0.0f, 0.0f,		1.0f, 0.65f, 0.0f,
		0.0f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f, 
		0.0f, -0.5f, 0.0f,		1.0f, 0.73f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.65f, 0.0f,
		0.25f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f
	};

	GLuint indices[] =
	{
		0,1,2,
		0,2,3,
		1,2,6,
		1,5,6,
		2,3,7,
		2,6,7,
		0,3,7,
		0,4,7,
		0,1,5,
		0,4,5,
		4,5,6,
		4,6,7
	};

	Shader *shader = new Shader("shaders/vshader.vs", "shaders/fshader.frag");
	Shader *grayShader = new Shader("shaders/posvshader.vs", "shaders/grayshader.frag");

	std::vector<Model*> models;
	Model *o = new Model(vertices3, 36, grayShader);
	o->scale(glm::vec3(3.f, 3.f, 3.f));
	//Model *o1 = new Model(vertices2, 9, shader);
	Model *tieFighter = new Model("obj/tiefighter_merged.obj", grayShader);
	tieFighter->setPosition(glm::vec3(0.f, 0.f, 5.f));

	//tieFighter->rotate(90, glm::vec3(0.f, 1.f, 0.f));
	//tieFighter->setDefaultDirection(glm::vec3(1.f, 0.f, 0.f));
	//tieFighter->setDirection(glm::vec3(0.f, 0.f, -1.f));
	//tieFighter->setDirection(glm::vec3(1.f, 0.f, 0.f));
	//tieFighter->scaleLocal(glm::vec3(0.3f, 0.3f, 0.3f));
	//tieFighter->rotateLocal(90, glm::vec3(0.0f, 1.0f, 0.0f));
	//tieFighter->translateGlobal(glm::vec3(0.0f, 0.0f, -0.2f));
	models.push_back(tieFighter);
	models.push_back(o);

	//o1->translateLocal(glm::vec3(-0.75f, 0.75f, 0.0f));
	//o1->scaleLocal(glm::vec3(0.2, 0.2, 0.2));

	ThirdPersonCamera tpcam(tieFighter, 10.f);

	Camera* currentCam = &tpcam;

	GLfloat pitch = 0.f, yaw = 0.f;

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

			// update tiefighter
			GLfloat speed = 0.005f;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				tieFighter->translate(speed * glm::cross(tieFighter->getUp(), glm::cross(tieFighter->getFront(), tieFighter->getUp())));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				tieFighter->setPosition(tieFighter->getPosition() - speed * glm::cross(tieFighter->getUp(), glm::cross(tieFighter->getFront(), tieFighter->getUp())));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				tieFighter->setPosition(tieFighter->getPosition() - speed * glm::cross(tieFighter->getFront(), tieFighter->getUp()));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				tieFighter->setPosition(tieFighter->getPosition() + speed * glm::cross(tieFighter->getFront(), tieFighter->getUp()));

			sf::Vector2i mouse = sf::Mouse::getPosition(window);
			GLfloat xoffset = mouse.x - 400;
			GLfloat yoffset = 300 - mouse.y;

			GLfloat sensitivity = 0.1;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;

			if (std::abs(pitch + yoffset) <= 89.f)
			{
				pitch += yoffset;
			}
			else if (yoffset > 0)
			{
				yoffset = 89.f - pitch;
				pitch = 89.f;
			}
			else
			{
				yoffset = -89.f - pitch;
				pitch = -89.f;
			}

			tieFighter->rotate(-xoffset, tieFighter->getUp());
			tieFighter->rotate(yoffset, glm::cross(tieFighter->getFront(), tieFighter->getUp()));

			// update cam
			currentCam->update(window);

			// Reset mouse to middle of screen
			sf::Mouse::setPosition(sf::Vector2i(400, 300), window);

			// Clear the color / depth buffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						
			// Draw each model
			for (std::vector<Model*>::iterator it = models.begin(); it != models.end(); it++)
			{
				// Set the shader
				(*it)->shader->use();

				GLint modelLoc = glGetUniformLocation((*it)->shader->program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr((*it)->getTransform()));

				GLint normalMatrixLoc = glGetUniformLocation((*it)->shader->program, "normalMatrix");
				glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse((*it)->getTransform()))));

				GLint viewLoc = glGetUniformLocation((*it)->shader->program, "view");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(currentCam->view));

				GLint projectionLoc = glGetUniformLocation((*it)->shader->program, "projection");
				glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

				GLint objectColorLoc = glGetUniformLocation((*it)->shader->program, "objectColor");
				glUniform3f(objectColorLoc, 1.f, 1.f, 1.f);

				GLint lightPosLoc = glGetUniformLocation((*it)->shader->program, "lightPos");
				glUniform3f(lightPosLoc, 110.f, 90.f, 100.f);

				GLint viewPosLoc = glGetUniformLocation((*it)->shader->program, "viewPos");
				glm::vec3 camPos = currentCam->getPosition();
				glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);

				glBindVertexArray((*it)->VAO);
				glDrawArrays(GL_TRIANGLES, 0, (*it)->getNumberOfVertices());
				glBindVertexArray(0);
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

	return 0;
}