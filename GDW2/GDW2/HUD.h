#pragma once
#include <SFML\Graphics.hpp>
#include <glm\glm.hpp>
#include <vector>

namespace flopse
{
	struct UIElement
	{
		UIElement()
		{
		}

		UIElement(int left, int top, int width, int height) : pos(sf::Vector2i(left, top)), rect(sf::IntRect(left, top, width, height))
		{
		}

		sf::Vector2i pos;
		sf::IntRect rect;
	};

	class HUD
	{
	private:
		sf::RenderWindow* window;
		sf::Texture hudTexture;
		sf::Sprite* hudSprite;
		sf::Vector2f scale;
		std::vector<UIElement> staticElements;
		std::vector<UIElement> resourceBars;
		std::vector<UIElement> eButtonAnimation;
		std::vector<UIElement> mouseButtonAnimation;

		sf::RectangleShape crossHairVert;
		sf::RectangleShape crossHairHoriz;

		sf::CircleShape enemyDot;

		sf::Font arial;
		sf::Text text;

		int eButtonAnimIndex = 0;
		int mouseButtonAnimIndex = 0;

		sf::Time eButtonFlashTimer;
		sf::Time mouseButtonFlashTimer;
		sf::Time flashDelay = sf::seconds(1.f);
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

		void scaleChanged();

		void keyPressed(sf::Event::KeyEvent e);
		void mouseButtonPressed(sf::Event::MouseButtonEvent e);
	};
}