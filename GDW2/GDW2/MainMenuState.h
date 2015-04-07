#pragma once
#include "State.h"
#include <vector>
#include <string>
#include "UIElement.h"
#include "SpriteWithAtlas.h"
#include "SoundManager.h"

namespace flopse
{
	class MainMenuState : public State
	{
	private:
		sf::RenderWindow* window;
		sf::Texture bgTexture;
		sf::Sprite* bgSprite;
		SpriteWithAtlas buttonSprite;
		std::vector<Button> buttons;
		Button contBtn;
		Button newBtn;
		Button loadBtn;
		Button saveBtn;
		Button optionsBtn;
		Button creditsBtn;
		Button exitBtn;

		Sound *bgMusic = nullptr;

		void createButtons();
		void buttonClicked(const std::string &name);

	public:
		MainMenuState(sf::RenderWindow* window);
		virtual ~MainMenuState();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
		void mouseButtonReleased(sf::Event::MouseButtonEvent e);
		void mouseMoved(sf::Event::MouseMoveEvent e);

		void scaleChanged();

		void startBackgroundMusic();
		void stopBackgroundMusic();
	};
}