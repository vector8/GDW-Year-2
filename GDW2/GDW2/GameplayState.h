#pragma once
#include "State.h"

#include <memory>
#include <glm\glm.hpp>

#include "Level.h"
#include "HUD.h"
#include "FrameBuffer.h"

#define BLOOM_THRESHOLD		0.25f
#define BLOOM_DOWNSCALE		2.0f
#define BLOOM_BLUR_PASSES	4

namespace flopse
{
	class GameplayState : public State
	{
	private:
		glm::mat4 projection;
		sf::RenderWindow* window;

	public:
		SceneNode* root;
		std::shared_ptr<Player> player = nullptr;
		Level* currentLevel;
		HUD hud;
		FrameBuffer mainBuffer;
		FrameBuffer workBuffer1;
		FrameBuffer workBuffer2;
		Shader grayscalePostShader;
		Shader bloomHighPassShader;
		Shader blurHorizontalShader;
		Shader blurVerticalShader;
		Shader bloomCompositeShader;

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