#include "GameplayState.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Tower.h"
#include "Projectile.h"
#include "SoundManager.h"
#include "Game.h"
#include "Utilities.h"
#include "ParticleSystem.h"

namespace flopse
{
	GameplayState::GameplayState(sf::RenderWindow* window) : window(window), hud(window),
		mainBuffer(1), downscaleBuffer1(1), downscaleBuffer2(1), fullscaleBuffer1(1), fullscaleBuffer2(1), shadowMapBuffer(1),
		grayscalePostShader(Shader::getStandardShader(StandardShaders::Grayscale)),
		bloomHighPassShader(Shader::getStandardShader(StandardShaders::BloomHighPass)),
		bloomCompositeShader(Shader::getStandardShader(StandardShaders::BloomComposite)),
		blurHorizontalShader(Shader::getStandardShader(StandardShaders::BlurHorizontal)),
		blurVerticalShader(Shader::getStandardShader(StandardShaders::BlurVertical))
	{
		player = std::make_shared<Player>();

		cam = std::make_shared<ThirdPersonCamera>();
		cam->localTransform.translate(glm::vec3(-80.f, (player->mesh->getHeight() / 2.f) + 30.f, -250.f));
		cam->projection = glm::perspective(Game::getGame()->getFieldOfView(), 1024.0f / 768.0f, 0.1f, 100000.0f);
		//cam->projection = glm::ortho(-512.0f, 512.0f, -384.0f, 384.0f, -1000.f, 1000.f);
		//cam->projection = glm::ortho(-350.f, 350.f, -350.f, 350.f, -10.f, 10000.f);
		player->attachCam(cam);

		currentLevel = Level::createLevel(1, player);//std::make_shared<Level>(player);
		root = currentLevel;

		SoundManager::getSoundManager()->setListener(player);

		initFullScreenQuad();

		mainBuffer.initDepthTexture(window->getSize().x, window->getSize().y);
		mainBuffer.initColorTexture(0, window->getSize().x, window->getSize().y, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
		assert(mainBuffer.checkFBO());

		shadowMapBuffer.initDepthTexture(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
		shadowMapBuffer.initColorTexture(0, SHADOW_RESOLUTION, SHADOW_RESOLUTION, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
		assert(shadowMapBuffer.checkFBO());

		downscaleBuffer1.initColorTexture(0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
		assert(downscaleBuffer1.checkFBO());

		downscaleBuffer2.initColorTexture(0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
		assert(downscaleBuffer2.checkFBO());

		fullscaleBuffer1.initDepthTexture(window->getSize().x, window->getSize().y);
		fullscaleBuffer1.initColorTexture(0, window->getSize().x, window->getSize().y, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
		assert(fullscaleBuffer1.checkFBO());

		fullscaleBuffer2.initDepthTexture(window->getSize().x, window->getSize().y);
		fullscaleBuffer2.initColorTexture(0, window->getSize().x, window->getSize().y, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
		assert(fullscaleBuffer2.checkFBO());
	}

	GameplayState::~GameplayState()
	{
	}

	void GameplayState::applyBloomEffect(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer)
	{
		// Compute high pass
		glViewport(0, 0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE);
		bloomHighPassShader->bind();
		glUniform1f(bloomHighPassShader->thresholdLoc, BLOOM_THRESHOLD);
		downscaleBuffer1.bind();
		glBindTexture(GL_TEXTURE_2D, inputBuffer.getColorHandle(0));
		drawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		downscaleBuffer1.unbind();
		bloomHighPassShader->unbind();

		// Compute blur
		glViewport(0, 0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE);
		for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
		{
			// Horizontal
			blurHorizontalShader->bind();
			glUniform1f(blurHorizontalShader->pixelSizeLoc, 1.0f / (window->getSize().x / BLOOM_DOWNSCALE));
			downscaleBuffer2.bind();
			glBindTexture(GL_TEXTURE_2D, downscaleBuffer1.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			downscaleBuffer2.unbind();
			blurHorizontalShader->unbind();

			// Vertical
			blurVerticalShader->bind();
			glUniform1f(blurVerticalShader->pixelSizeLoc, 1.0f / (window->getSize().y / BLOOM_DOWNSCALE));
			downscaleBuffer1.bind();
			glBindTexture(GL_TEXTURE_2D, downscaleBuffer2.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			downscaleBuffer1.unbind();
			blurVerticalShader->unbind();
		}

		// Composite to result buffer
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		bloomCompositeShader->bind();
		glUniform1i(bloomCompositeShader->sceneLoc, 0);
		glUniform1i(bloomCompositeShader->bloomLoc, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputBuffer.getColorHandle(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, downscaleBuffer1.getColorHandle(0));
		outputBuffer.bind();
		drawFullScreenQuad();
		outputBuffer.unbind();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		bloomCompositeShader->unbind();
	}

	void GameplayState::applyBlur(FrameBuffer &inputBuffer, FrameBuffer &outputBuffer, int passes)
	{
		// Compute blur
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		for (int i = 0; i < passes; i++)
		{
			// Horizontal
			blurHorizontalShader->bind();
			glUniform1f(blurHorizontalShader->pixelSizeLoc, 1.0f / window->getSize().x);

			outputBuffer.bind();
			glBindTexture(GL_TEXTURE_2D, inputBuffer.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			outputBuffer.unbind();

			blurHorizontalShader->unbind();

			// Vertical
			blurVerticalShader->bind();
			glUniform1f(blurVerticalShader->pixelSizeLoc, 1.0f / window->getSize().y);

			outputBuffer.bind();
			glBindTexture(GL_TEXTURE_2D, outputBuffer.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			outputBuffer.unbind();

			blurVerticalShader->unbind();
		}
	}

	void GameplayState::applyGrayscaleEffect(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer)
	{
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		grayscalePostShader->bind();
		glBindTexture(GL_TEXTURE_2D, inputBuffer.getColorHandle(0));
		outputBuffer.bind();
		drawFullScreenQuad();
		outputBuffer.unbind();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		grayscalePostShader->unbind();
	}

	void GameplayState::applyShadows(const FrameBuffer &sceneBuffer, const FrameBuffer &shadowBuffer, FrameBuffer &outputBuffer)
	{
		std::shared_ptr<Shader> shadowCompositeShader = Shader::getStandardShader(StandardShaders::ShadowComposite);
		shadowCompositeShader->bind();

		glUniform1i(shadowCompositeShader->sceneLoc, 0);
		glUniform1i(shadowCompositeShader->shadowLocs.shadows, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sceneBuffer.getColorHandle(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowBuffer.getColorHandle(0));
		outputBuffer.bind();
		drawFullScreenQuad();
		outputBuffer.unbind();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		shadowCompositeShader->unbind();
	}

	void GameplayState::update(const sf::Time &dt)
	{
		root->update(*window, dt, glm::mat4());

		hud.update(dt);
	}

	void GameplayState::draw()
	{
		// Clear all buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.f);
		mainBuffer.clear();
		glClearColor(1.0f, 1.0f, 1.0f, 1.f);
		downscaleBuffer1.clear();
		downscaleBuffer2.clear();
		fullscaleBuffer1.clear();
		fullscaleBuffer2.clear();
		shadowMapBuffer.clear();

		// Render the scene
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		//glViewport(0, 0, window->getSize().x / 2, window->getSize().y / 2);
		mainBuffer.bind();
		draw(root, cam, currentLevel);
		mainBuffer.unbind();

		// Render to the shadow map
		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
		shadowMapBuffer.bind();
		drawToShadowMap(root, currentLevel->shadowCamera);
		shadowMapBuffer.unbind();

		// Draw shadows in the scene
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		fullscaleBuffer1.bind();
		drawShadows(root, cam);
		fullscaleBuffer1.unbind();

		// Blur shadows
		//applyBlur(fullscaleBuffer1, fullscaleBuffer2, 100);

		// Composite shadows and scene.
		applyShadows(mainBuffer, fullscaleBuffer1, fullscaleBuffer2);

		//applyBloomEffect(fullscaleBuffer2, fullscaleBuffer1);
		//applyGrayscaleEffect(fullscaleBuffer1, fullscaleBuffer2);

		/*glViewport(window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);
		applyShadowEffect(mainBuffer, shadowMapBuffer, fullscaleBuffer1);*/

		//fullscaleBuffer1.moveToBackBuffer(window->getSize().x, window->getSize().y);

		/*Shader s("shaders/PosUVStraightPassThrough.vert", "shaders/DrawFullScreenQuad.frag");
		s.bind();
		glBindTexture(GL_TEXTURE_2D, shadowMapBuffer.getDepthHandle());
		fullscaleBuffer1.bind();
		drawFullScreenQuad();
		fullscaleBuffer1.unbind();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		s.unbind();*/

		mainBuffer.moveToBackBuffer(0, 0, window->getSize().x, window->getSize().y, 0, 0, window->getSize().x, window->getSize().y);
		//fullscaleBuffer2.moveToBackBuffer(0, 0, window->getSize().x, window->getSize().y, 0, 0, window->getSize().x, window->getSize().y);
		//fullscaleBuffer2.moveToBackBuffer(0, 0, window->getSize().x, window->getSize().y, 0, window->getSize().y / 2, window->getSize().x / 2, window->getSize().y);
		//mainBuffer.moveToBackBuffer(0, 0, window->getSize().x, window->getSize().y, 0, 0, window->getSize().x / 2, window->getSize().y / 2);
		//shadowMapBuffer.moveToBackBuffer(SHADOW_RESOLUTION, SHADOW_RESOLUTION, window->getSize().x, window->getSize().y);
		//fullscaleBuffer1.moveToBackBuffer(0, 0, window->getSize().x, window->getSize().y, window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);
		//shadowMapBuffer.moveToBackBuffer(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION, window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);
		//fullscaleBuffer1.moveToBackBuffer(window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y, window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);

		hud.draw();
	}

	void GameplayState::draw(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam, const std::shared_ptr<Level> &lvl)
	{
		if (node->usesCustomDraw)
		{
			std::shared_ptr<Shader> s = Shader::getStandardShader(StandardShaders::Billboard);
			s->bind();
			glUniformMatrix4fv(s->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(s->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(s->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			node->draw();
			s->unbind();
		}
		else if (node->mesh)
		{
			// Set the shader
			std::shared_ptr<Shader> shader = node->mesh->shader;
			shader->bind();

			glUniformMatrix4fv(shader->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(shader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			glUniform4f(shader->objectColorLoc, node->mesh->overlayColour.getR(), node->mesh->overlayColour.getG(), node->mesh->overlayColour.getB(), node->mesh->overlayColour.getA());
			glUniform1f(shader->blendLoc, node->mesh->animationBlend);

			glm::vec3 camPos = cam->getGlobalPosition();
			glUniform3f(shader->viewPosLoc, camPos.x, camPos.y, camPos.z);

			// Tower placement
			glUniform1i(shader->validPlacementLoc, node->mesh->validPlacement);

			// Fog
			glUniform1f(shader->fogFactorLoc, lvl->fogFactor);

			// Material
			glUniform1i(shader->materialLocs.diffuse, 0);
			glUniform1i(shader->materialLocs.specular, 1);
			glUniform1f(shader->materialLocs.specularExponent, 32.f);

			// Point Lights
			for (int i = 0; i < NUM_POINT_LIGHTS; i++)
			{
				glm::vec3 pos = lvl->pointLights[i]->getGlobalPosition();
				glUniform3f(shader->pointLightLocs[i].position, pos.x, pos.y, pos.z);
				glUniform3f(shader->pointLightLocs[i].ambient, lvl->pointLights[i]->ambient.r, lvl->pointLights[i]->ambient.g, lvl->pointLights[i]->ambient.b);
				glUniform3f(shader->pointLightLocs[i].diffuse, lvl->pointLights[i]->diffuse.r, lvl->pointLights[i]->diffuse.g, lvl->pointLights[i]->diffuse.b);
				glUniform3f(shader->pointLightLocs[i].specular, lvl->pointLights[i]->specular.r, lvl->pointLights[i]->specular.g, lvl->pointLights[i]->specular.b);
				glUniform1f(shader->pointLightLocs[i].constantAttenuation, lvl->pointLights[i]->constantAttenuation);
				glUniform1f(shader->pointLightLocs[i].linearAttenuation, lvl->pointLights[i]->linearAttenuation);
				glUniform1f(shader->pointLightLocs[i].quadraticAttenuation, lvl->pointLights[i]->quadraticAttenuation);
			}

			// Directional Lights
			glm::vec3 front = lvl->dirLight->getGlobalFront();
			glUniform3f(shader->directionalLightLocs.direction, front.x, front.y, front.z);
			glUniform3f(shader->directionalLightLocs.ambient, lvl->dirLight->ambient.r, lvl->dirLight->ambient.g, lvl->dirLight->ambient.b);
			glUniform3f(shader->directionalLightLocs.diffuse, lvl->dirLight->diffuse.r, lvl->dirLight->diffuse.g, lvl->dirLight->diffuse.b);
			glUniform3f(shader->directionalLightLocs.specular, lvl->dirLight->specular.r, lvl->dirLight->specular.g, lvl->dirLight->specular.b);


			// Diffuse map
			glActiveTexture(GL_TEXTURE0);
			std::shared_ptr<sf::Texture> t = node->mesh->getDiffuseMap();
			if (t != nullptr)
			{
				sf::Texture::bind(&(*t));
			}

			// Specular map
			glActiveTexture(GL_TEXTURE1);
			t = node->mesh->getSpecularMap();
			if (t != nullptr)
			{
				sf::Texture::bind(&(*t));
			}

			// Shadow map
			//glActiveTexture(GL_TEXTURE2);
			//glBindTexture(GL_TEXTURE_2D, shadowMap);

			glBindVertexArray(node->mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, node->mesh->getNumberOfVertices());
			glBindVertexArray(GL_NONE);

			// Unbind the texture and shader.
			//glBindTexture(GL_TEXTURE_2D, GL_NONE);
			//glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, GL_NONE);
			shader->unbind();
		}

		Node<std::shared_ptr<SceneNode>>* current = node->children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				draw(current->data, cam, lvl);
			}

			current = current->next;
		}
	}

	void GameplayState::drawToShadowMap(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam)
	{
		if (node->mesh)
		{
			// Set the shader
			std::shared_ptr<Shader> shader = node->mesh->shader;
			//std::shared_ptr<Shader> shadowMapShader = Shader::getStandardShader(StandardShaders::ShadowMap);
			shader->bind();

			glUniformMatrix4fv(shader->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(shader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			glUniform4f(shader->objectColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
			glUniform1f(shader->blendLoc, node->mesh->animationBlend);

			glm::vec3 camPos = cam->getGlobalPosition();
			glUniform3f(shader->viewPosLoc, camPos.x, camPos.y, camPos.z);
			glUniform3f(shader->lightPosLoc, 500.f, 500.f, 500.f);

			glBindVertexArray(node->mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, node->mesh->getNumberOfVertices());
			glBindVertexArray(GL_NONE);

			// Unbind the shader.
			shader->unbind();
		}

		Node<std::shared_ptr<SceneNode>>* current = node->children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				drawToShadowMap(current->data, cam);
			}

			current = current->next;
		}
	}

	void GameplayState::drawShadows(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam)
	{
		if (node->mesh)
		{
			// Set the shader
			std::shared_ptr<Shader> shadowGenShader = Shader::getStandardShader(StandardShaders::ShadowGenerator);
			shadowGenShader->bind();

			glUniformMatrix4fv(shadowGenShader->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(shadowGenShader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(shadowGenShader->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			glUniform1f(shadowGenShader->blendLoc, node->mesh->animationBlend);

			glm::mat4 bias(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
			glm::mat4 worldToShadowMap = bias * (currentLevel->shadowCamera->projection * currentLevel->shadowCamera->view);
			glUniformMatrix4fv(shadowGenShader->shadowLocs.worldToShadowMap, 1, GL_FALSE, glm::value_ptr(worldToShadowMap));
			glUniform1i(shadowGenShader->shadowLocs.drawShadow, (GLint)node->mesh->acceptShadow);

			glBindTexture(GL_TEXTURE_2D, shadowMapBuffer.getDepthHandle());

			glBindVertexArray(node->mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, node->mesh->getNumberOfVertices());
			glBindVertexArray(GL_NONE);

			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			// Unbind the shader.
			shadowGenShader->unbind();
		}

		Node<std::shared_ptr<SceneNode>>* current = node->children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				drawShadows(current->data, cam);
			}

			current = current->next;
		}
	}

	void GameplayState::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		case sf::Keyboard::Comma:
			/*wireframe = !wireframe;
			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}*/
			break;
		case sf::Keyboard::Space:
			player->jump();
			break;
		case sf::Keyboard::Num1:
			if (placingTower && currentTower != TowerType::Arrow)
			{
				player->detach(tempTower);
				tempTower = std::make_shared<Tower>(Tower::createTower(TowerType::Arrow, glm::vec3(0.f, 0.f, 200.f)));
				player->attach(tempTower);
			}
			currentTower = TowerType::Arrow; //ArrowTower
			break;
		case sf::Keyboard::Num2:
			if (placingTower && currentTower != TowerType::Frost)
			{
				player->detach(tempTower);
				tempTower = std::make_shared<Tower>(Tower::createTower(TowerType::Frost, glm::vec3(0.f, 0.f, 200.f)));
				player->attach(tempTower);
			}
			currentTower = TowerType::Frost; //FrostTower
			break;
		case sf::Keyboard::Num3:
			if (placingTower && currentTower != TowerType::Fire)
			{
				player->detach(tempTower);
				tempTower = std::make_shared<Tower>(Tower::createTower(TowerType::Fire, glm::vec3(0.f, 0.f, 200.f)));
				player->attach(tempTower);
			}
			currentTower = TowerType::Fire; //FireTower
			break;
		case sf::Keyboard::Num4:
			if (placingTower && currentTower != TowerType::Catapult)
			{
				player->detach(tempTower);
				tempTower = std::make_shared<Tower>(Tower::createTower(TowerType::Catapult, glm::vec3(0.f, 0.f, 200.f)));
				player->attach(tempTower);
			}
			currentTower = TowerType::Catapult; //
			break;
		//case sf::Keyboard::Num5:
		//	currentTower = TowerType::Barricade;
		//	break;
		case sf::Keyboard::E:
		{
			if (!placingTower)
			{
				if (!player->jumping && player->gold >= 50)
				{
					placingTower = true;

					tempTower = std::make_shared<Tower>(Tower::createTower(currentTower, glm::vec3(0.f, 0.f, 200.f)));
					this->player->attach(tempTower);
				}
				else
				{
					// TODO play error sound, flash gold on ui
				}
			}
		}
			break;
		default:
			break;
		}

		hud.keyPressed(e);
	}

	void GameplayState::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
		{
			if (placingTower)
			{
				if (tempTower->mesh->validPlacement)
				{
					player->gold -= 50;

					//auto t = std::make_shared<Tower>(Tower::createTower(TowerType::Arrow, player->getGlobalPosition() + glm::vec3(front.x, 0.f, front.z)  * 30.f));
					glm::vec3 pos = tempTower->getGlobalPosition();
					player->detach(tempTower);

					tempTower->activate();
					tempTower->localTransform.setPosition(pos);
					currentLevel->attach(tempTower);
					tempTower->boundingBox.position = pos;

					currentLevel->towerBlockers.push_back(tempTower->boundingBox);
				
					SoundManager::playSoundAt(DefaultSounds::Clank, pos, false);

					placingTower = false;
				}
				else
				{
					// TODO play error sound
				}
			}
			else
			{
				glm::vec3 pos = player->getGlobalPosition() + glm::vec3(0.f, 3.f * player->mesh->getHeight() / 4.f, 0.f);
				auto p = std::make_shared<Projectile>(pos, cam->getGlobalPosition() + player->getAimDirection() * 2000.f, 5);
				this->currentLevel->attach(p);
				SoundManager::playSoundAt(DefaultSounds::Ping, pos, false);
			}
		}
			break;
		case sf::Mouse::Button::Right:
			if (placingTower)
			{
				player->detach(tempTower);
				tempTower.reset();
				placingTower = false;
			}
			break;
		default:
			break;
		}

		hud.mouseButtonPressed(e);
	}

	void GameplayState::mouseButtonReleased(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		default:
			break;
		}
	}

	void GameplayState::mouseMoved(sf::Event::MouseMoveEvent e)
	{
	}

	void GameplayState::setFieldOfView(float degrees)
	{
		cam->projection = glm::perspective(degrees, 1024.f / 768.f, 0.1f, 100000.0f);
	}

	void GameplayState::nextLevel()
	{
		levelNumber++;
		if (levelNumber > 5)
		{
			Game::getGame()->newGame();
			Game::getGame()->setMainMenuState();
		}
		else
		{
			currentLevel = Level::createLevel(levelNumber, player);
			root = currentLevel;
		}
	}
}