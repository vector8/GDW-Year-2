#include "GameplayState.h"
#include <glm\gtc\matrix_transform.hpp>

#include "Tower.h"
#include "Projectile.h"
#include "SoundManager.h"
#include "Game.h"
#include "Utilities.h"

namespace flopse
{
	GameplayState::GameplayState(sf::RenderWindow* window) : window(window), hud(window), mainBuffer(1), workBuffer1(1), workBuffer2(1),
		grayscalePostShader("shaders/PosUVStraightPassThrough.vert", "shaders/GrayscalePost.frag"),
		bloomHighPassShader("shaders/PosUVStraightPassThrough.vert", "shaders/Bloom/BloomHighPass.frag"),
		blurHorizontalShader("shaders/PosUVStraightPassThrough.vert", "shaders/Bloom/BlurHorizontal.frag"),
		blurVerticalShader("shaders/PosUVStraightPassThrough.vert", "shaders/Bloom/BlurVertical.frag"),
		bloomCompositeShader("shaders/PosUVStraightPassThrough.vert", "shaders/Bloom/BloomComposite.frag")
	{
		std::shared_ptr<Mesh> playerMesh = std::make_shared<Mesh>("meshes/Goblin.bmf", new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag"));
		playerMesh->setTexture("textures/GoblinTexture.png");
		player = std::make_shared<Player>(playerMesh);

		currentLevel = new Level(player);
		root = currentLevel;

		projection = glm::perspective(Game::getGame()->getFieldOfView(), 1024.0f / 768.0f, 0.1f, 100000.0f);

		SoundManager::getSoundManager()->setListener(player);

		initFullScreenQuad();

		mainBuffer.initDepthTexture(window->getSize().x, window->getSize().y);
		mainBuffer.initColorTexture(0, window->getSize().x, window->getSize().y, GL_RGBA8, GL_NEAREST, GL_CLAMP_TO_EDGE);
		assert(mainBuffer.checkFBO());

		workBuffer1.initColorTexture(0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
		assert(workBuffer1.checkFBO());

		workBuffer2.initColorTexture(0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE, GL_RGB8, GL_LINEAR, GL_CLAMP_TO_EDGE);
		assert(workBuffer2.checkFBO());

	}

	GameplayState::~GameplayState()
	{
		delete root;
		delete currentLevel;
	}

	void GameplayState::update(const sf::Time &dt)
	{
		root->update(*window, dt, glm::mat4());

		hud.update(dt);

		window->setMouseCursorVisible(false);
		// Reset mouse to middle of screen
		sf::Mouse::setPosition(sf::Vector2i((int)(window->getSize().x) / 2, (int)(window->getSize().y) / 2), *window);
	}

	void GameplayState::draw()
	{
		// Clear all buffers
		glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainBuffer.clear();
		workBuffer1.clear();
		workBuffer2.clear();

		// Render the scene
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		mainBuffer.bind();
		root->draw(currentLevel->cam->getGlobalPosition(), currentLevel->cam->view, projection, currentLevel->light);
		mainBuffer.unbind();

		// Compute high pass
		glViewport(0, 0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE);
		bloomHighPassShader.bind();
		glUniform1f(bloomHighPassShader.thresholdLoc, BLOOM_THRESHOLD);
		workBuffer1.bind();
		glBindTexture(GL_TEXTURE_2D, mainBuffer.getColorHandle(0));
		drawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		workBuffer1.unbind();
		bloomHighPassShader.unbind();

		// Compute blur
		glViewport(0, 0, window->getSize().x / BLOOM_DOWNSCALE, window->getSize().y / BLOOM_DOWNSCALE);
		for (int i = 0; i < BLOOM_BLUR_PASSES; i++)
		{
			// Horizontal
			blurHorizontalShader.bind();
			glUniform1f(blurHorizontalShader.pixelSizeLoc, 1.0f / window->getSize().x);

			workBuffer2.bind();

			glBindTexture(GL_TEXTURE_2D, workBuffer1.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			workBuffer2.unbind();

			blurHorizontalShader.unbind();

			// Vertical
			blurVerticalShader.bind();
			glUniform1f(blurVerticalShader.pixelSizeLoc, 1.0f / window->getSize().y);

			workBuffer1.bind();

			glBindTexture(GL_TEXTURE_2D, workBuffer2.getColorHandle(0));
			drawFullScreenQuad();
			glBindTexture(GL_TEXTURE_2D, GL_NONE);

			workBuffer1.unbind();

			blurVerticalShader.unbind();
		}

		// Composite to back buffer
		glViewport(0, 0, window->getSize().x, window->getSize().y);
		bloomCompositeShader.bind();
		glUniform1i(bloomCompositeShader.sceneLoc, 0);
		glUniform1i(bloomCompositeShader.bloomLoc, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainBuffer.getColorHandle(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, workBuffer1.getColorHandle(0));
		drawFullScreenQuad();
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		bloomCompositeShader.unbind();

		hud.draw();
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
				auto t = std::make_shared<Tower>(Tower::createTower(TowerType::Arrow, player->getGlobalPosition() + glm::vec3(front.x, 0.f, front.z)  * 30.f));
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
		projection = glm::perspective(degrees, 1024.f / 768.f, 0.1f, 100000.0f);
	}
}