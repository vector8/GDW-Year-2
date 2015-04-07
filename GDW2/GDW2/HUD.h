#pragma once
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>
#include <vector>
#include "UIElement.h"
#include "SpriteWithAtlas.h"

namespace flopse
{
	class HUD
	{
	private:
		sf::RenderWindow* window;
		SpriteWithAtlas hudSprite;
		sf::Vector2f scale;

		std::vector<UIElement> staticElements;
		std::vector<UIElement> towerInfos;
		UIElement castleHealthBar;

		// Tower selectors
		std::vector<Button> buttons;
		sf::Vector2f selectorScale;

		// Tutorial animations
		AnimatedUIElement eKeyAnimation;
		AnimatedUIElement mouseButtonAnimation;

		sf::RectangleShape crossHairVert;
		sf::RectangleShape crossHairHoriz;

		sf::CircleShape enemyDot;
		sf::CircleShape playerDot;

		sf::Font arial;
		sf::Text text;

		int currentTower = 0;

		sf::Time eButtonDelay = sf::seconds(5.f);
		sf::Time mouseButtonDelay = sf::seconds(5.f);
		sf::Time tutorialTimer;

		bool eButtonPressed = false;
		bool lMouseButtonPressed = false;

		void createUIElements();

	public:
		HUD(sf::RenderWindow* window);
		virtual ~HUD();

		void update(const sf::Time &dt);
		void draw();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);

		void scaleChanged();
	};
}