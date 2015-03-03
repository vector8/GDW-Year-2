#include "GameplayState.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Tower.h"
#include "Projectile.h"
#include "SoundManager.h"
#include "Game.h"
#include "Utilities.h"

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
		std::shared_ptr<Mesh> playerMesh = std::make_shared<Mesh>("meshes/Goblin.bmf", Shader::getStandardShader(StandardShaders::Phong));
		playerMesh->setDiffuseMap("textures/GoblinTexture.png");
		playerMesh->setSpecularMap("textures/GoblinSpecularMap.png");
		player = std::make_shared<Player>(playerMesh);

		currentLevel = std::make_shared<Level>(player);
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
			glUniform1f(blurHorizontalShader->pixelSizeLoc, 1.0f / window->getSize().x);

			downscaleBuffer2.bind();

			glBindTexture(GL_TEXTURE_2D, downscaleBuffer1.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			downscaleBuffer2.unbind();

			blurHorizontalShader->unbind();

			// Vertical
			blurVerticalShader->bind();
			glUniform1f(blurVerticalShader->pixelSizeLoc, 1.0f / window->getSize().y);

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
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainBuffer.clear();
		glClearColor(1.0f, 1.0f, 1.0f, 1.f);
		downscaleBuffer1.clear();
		downscaleBuffer2.clear();
		fullscaleBuffer1.clear();
		fullscaleBuffer2.clear();
		shadowMapBuffer.clear();

		// Render to the shadow map
		glViewport(0, 0, SHADOW_RESOLUTION, SHADOW_RESOLUTION);
		shadowMapBuffer.bind();
		drawToShadowMap(root, currentLevel->shadowCamera);
		shadowMapBuffer.unbind();

		// Draw shadows in the scene
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		fullscaleBuffer1.bind();
		drawShadows(root, currentLevel->cam);
		fullscaleBuffer1.unbind();

		// Blur shadows
		//applyBlur(fullscaleBuffer1, fullscaleBuffer2, 100);

		// Render the scene
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		//glViewport(0, 0, window->getSize().x / 2, window->getSize().y / 2);
		mainBuffer.bind();
		draw(root, currentLevel->cam, currentLevel->dirLight, currentLevel->pointLights, currentLevel->fogFactor);
		mainBuffer.unbind();

		// Composite shadows and scene.
		applyShadows(mainBuffer, fullscaleBuffer1, fullscaleBuffer2);

		//applyBloomEffect(fullscaleBuffer2, fullscaleBuffer1);
		//applyGrayscaleEffect(fullscaleBuffer1, fullscaleBuffer2);

		/*glViewport(window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);
		applyShadowEffect(mainBuffer, shadowMapBuffer, fullscaleBuffer1);*/

		//fullscaleBuffer1.moveToBackBuffer(window->getSize().x, window->getSize().y);

		/*Shader s("shaders/PosUVStraightPassThrough.vert", "shaders/DrawFullScreenQuad.frag");
		s.bind();
		glBindTexture(GL_TEXTURE_2D, mainBuffer.getDepthHandle());
		drawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		s.unbind();*/

		fullscaleBuffer2.moveToBackBuffer(window->getSize().x, window->getSize().y);
		//mainBuffer.moveToBackBuffer(0, 0, window->getSize().x / 2, window->getSize().y / 2, 0, 0, window->getSize().x / 2, window->getSize().y / 2);
		//shadowMapBuffer.moveToBackBuffer(SHADOW_RESOLUTION, SHADOW_RESOLUTION, window->getSize().x, window->getSize().y);
		//fullscaleBuffer1.moveToBackBuffer(window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y, window->getSize().x / 2, window->getSize().y / 2, window->getSize().x, window->getSize().y);

		hud.draw();
	}

	void GameplayState::draw(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam, const Light &dirLight, const Light* pointLights, float fogFactor)
	{
		if (node->mesh)
		{
			// Set the shader
			node->mesh->shader->bind();

			glUniformMatrix4fv(node->mesh->shader->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(node->mesh->shader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(node->mesh->shader->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			glUniform4f(node->mesh->shader->objectColorLoc, node->mesh->overlayColour.getR(), node->mesh->overlayColour.getG(), node->mesh->overlayColour.getB(), node->mesh->overlayColour.getA());
			glm::vec3 camPos = cam->getGlobalPosition();
			glUniform3f(node->mesh->shader->viewPosLoc, camPos.x, camPos.y, camPos.z);

			// Fog
			glUniform1f(node->mesh->shader->fogFactorLoc, fogFactor);

			// Material
			glUniform1i(node->mesh->shader->materialLocs.diffuse, 0);
			glUniform1i(node->mesh->shader->materialLocs.specular, 1);
			glUniform1f(node->mesh->shader->materialLocs.specularExponent, 32.f);

			// Point Lights
			for (int i = 0; i < NUM_POINT_LIGHTS; i++)
			{
				glUniform3f(node->mesh->shader->pointLightLocs[i].position, pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z);
				glUniform3f(node->mesh->shader->pointLightLocs[i].ambient, pointLights[i].ambient.r, pointLights[i].ambient.g, pointLights[i].ambient.b);
				glUniform3f(node->mesh->shader->pointLightLocs[i].diffuse, pointLights[i].diffuse.r, pointLights[i].diffuse.g, pointLights[i].diffuse.b);
				glUniform3f(node->mesh->shader->pointLightLocs[i].specular, pointLights[i].specular.r, pointLights[i].specular.g, pointLights[i].specular.b);
				glUniform1f(node->mesh->shader->pointLightLocs[i].constantAttenuation, pointLights[i].constantAttenuation);
				glUniform1f(node->mesh->shader->pointLightLocs[i].linearAttenuation, pointLights[i].linearAttenuation);
				glUniform1f(node->mesh->shader->pointLightLocs[i].quadraticAttenuation, pointLights[i].quadraticAttenuation);
			}

			// Directional Lights
			glUniform3f(node->mesh->shader->directionalLightLocs.direction, dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
			glUniform3f(node->mesh->shader->directionalLightLocs.ambient, dirLight.ambient.r, dirLight.ambient.g, dirLight.ambient.b);
			glUniform3f(node->mesh->shader->directionalLightLocs.diffuse, dirLight.diffuse.r, dirLight.diffuse.g, dirLight.diffuse.b);
			glUniform3f(node->mesh->shader->directionalLightLocs.specular, dirLight.specular.r, dirLight.specular.g, dirLight.specular.b);


			// Diffuse map
			glActiveTexture(GL_TEXTURE0);
			sf::Texture* t = node->mesh->getDiffuseMap();
			sf::Texture::bind(t);

			// Specular map
			glActiveTexture(GL_TEXTURE1);
			t = node->mesh->getSpecularMap();
			sf::Texture::bind(t);

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
			node->mesh->shader->unbind();
		}

		Node<std::shared_ptr<SceneNode>>* current = node->children.head;

		while (current)
		{
			if (!current->data->toBeDeleted)
			{
				draw(current->data, cam, dirLight, pointLights, fogFactor);
			}

			current = current->next;
		}
	}

	void GameplayState::drawToShadowMap(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam)
	{
		if (node->mesh)
		{
			// Set the shader
			//std::shared_ptr<Shader> shadowMapShader = Shader::getStandardShader(StandardShaders::PhongNoTexture);
			std::shared_ptr<Shader> shadowMapShader = Shader::getStandardShader(StandardShaders::ShadowMap);
			shadowMapShader->bind();

			glUniformMatrix4fv(shadowMapShader->modelLoc, 1, GL_FALSE, glm::value_ptr(node->globalTransform));
			glUniformMatrix4fv(shadowMapShader->viewLoc, 1, GL_FALSE, glm::value_ptr(cam->view));
			glUniformMatrix4fv(shadowMapShader->projectionLoc, 1, GL_FALSE, glm::value_ptr(cam->projection));
			glUniform4f(shadowMapShader->objectColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
			glm::vec3 camPos = cam->getGlobalPosition();
			glUniform3f(shadowMapShader->viewPosLoc, camPos.x, camPos.y, camPos.z);
			glUniform3f(shadowMapShader->lightPosLoc, 500.f, 500.f, 500.f);

			glBindVertexArray(node->mesh->VAO);
			glDrawArrays(GL_TRIANGLES, 0, node->mesh->getNumberOfVertices());
			glBindVertexArray(GL_NONE);

			// Unbind the shader.
			shadowMapShader->unbind();
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
		case sf::Keyboard::Space:
			player->jump();
			break;
		case sf::Keyboard::E:
		{
			if (!player->jumping && player->gold >= 50)
			{
				player->gold -= 50;

				glm::vec3 front = player->getGlobalFront();
				//auto t = std::make_shared<Tower>(Tower::createTower(TowerType::Arrow, player->getGlobalPosition() + glm::vec3(front.x, 0.f, front.z)  * 30.f));
				auto t = std::make_shared<Tower>(Tower::createTower(TowerType::Mage, player->getGlobalPosition() + glm::vec3(front.x, 0.f, front.z)  * 30.f));
				this->currentLevel->attach(t);
				SoundManager::playSoundAt(DefaultSounds::Clank, t->getGlobalPosition(), false);
			}
			else
			{
				// TODO play error sound, flash gold on ui
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
			glm::vec3 pos = player->getGlobalPosition() + glm::vec3(0.f, 3.f * player->mesh->getHeight() / 4.f, 0.f);
			auto p = std::make_shared<Projectile>(pos, pos + player->getGlobalFront() * 1500.f, 5);
			this->currentLevel->attach(p);
			SoundManager::playSoundAt(DefaultSounds::Ping, pos, false);
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
		currentLevel->cam->projection = glm::perspective(degrees, 1024.f / 768.f, 0.1f, 100000.0f);
	}
}