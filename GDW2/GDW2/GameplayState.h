#pragma once
#include "State.h"

#include <memory>
#include <glm\glm.hpp>

#include "Level.h"
#include "Tower.h"
#include "HUD.h"
#include "FrameBuffer.h"

#define BLOOM_THRESHOLD		0.6f
#define BLOOM_DOWNSCALE		2.0f
#define BLOOM_BLUR_PASSES	10
#define SHADOW_RESOLUTION	2048

namespace flopse
{
	class GameplayState : public State
	{
	private:
		sf::RenderWindow* window;
		bool wireframe = false;
		bool placingTower = false;
		std::shared_ptr<Tower> tempTower = nullptr;
		std::shared_ptr<Entity> ballistaRange = nullptr, mageRange = nullptr, catapultRange = nullptr;

		TowerType currentTower = TowerType::Arrow;

		void applyBloomEffect(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer);
		void applyBlur(FrameBuffer &inputBuffer, FrameBuffer &outputBuffer, int passes);
		void applyGrayscaleEffect(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer);
		void applyShadowEffect(const FrameBuffer &sceneDepthBuffer, const FrameBuffer &shadowDepthBuffer, FrameBuffer &outputBuffer);
		void applyShadows(const FrameBuffer &sceneBuffer, const FrameBuffer &shadowBuffer, FrameBuffer &outputBuffer);
		void applyEdgeOutline(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer);
		void applyPixelationEffect(const FrameBuffer &inputBuffer, FrameBuffer &outputBuffer);

		void GameplayState::draw(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam, const std::shared_ptr<Level> &lvl);
		void GameplayState::drawToShadowMap(std::shared_ptr<SceneNode> root, const std::shared_ptr<Camera> &cam);
		void GameplayState::drawShadows(std::shared_ptr<SceneNode> node, const std::shared_ptr<Camera> &cam);

	public:
		std::shared_ptr<SceneNode> root;
		std::shared_ptr<Player> player = nullptr;
		std::shared_ptr<Camera> cam = nullptr;
		std::shared_ptr<Level> currentLevel = nullptr;
		HUD hud;
		FrameBuffer mainBuffer;
		FrameBuffer downscaleBuffer1;
		FrameBuffer downscaleBuffer2;
		FrameBuffer fullscaleBuffer1;
		FrameBuffer fullscaleBuffer2;
		FrameBuffer fullscaleBuffer3;
		FrameBuffer shadowMapBuffer;
		std::shared_ptr<Shader> grayscalePostShader = nullptr;
		std::shared_ptr<Shader> bloomHighPassShader = nullptr;
		std::shared_ptr<Shader> blurHorizontalShader = nullptr;
		std::shared_ptr<Shader> blurVerticalShader = nullptr;
		std::shared_ptr<Shader> bloomCompositeShader = nullptr;
		int levelNumber = 1;

		GameplayState(sf::RenderWindow* window);
		virtual ~GameplayState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);

		void setFieldOfView(float degrees);
	};
}