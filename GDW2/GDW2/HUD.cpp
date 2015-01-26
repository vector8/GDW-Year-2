#include "HUD.h"
#include <iostream>
#include <GL\glew.h>
#include "Game.h"
#include <sstream>

namespace flopse
{
	HUD::HUD(sf::RenderWindow* window) : window(window), crossHairVert(sf::Vector2f(4, 24)), crossHairHoriz(sf::Vector2f(25, 5)), enemyDot(3)
	{
		crossHairVert.setPosition(sf::Vector2f(((float)(window->getSize().x) / 2.f) - 14.f, ((float)(window->getSize().y) / 2.f) - 12.f));
		crossHairHoriz.setPosition(sf::Vector2f(((float)(window->getSize().x) / 2.f) - 24.f, ((float)(window->getSize().y) / 2.f) - 2.f));

		if (!hudTexture.loadFromFile("textures/HUD.png"))
		{
			std::cout << "ERROR Loading textures/HUD.png" << std::endl;
		}

		hudSprite = new sf::Sprite(hudTexture);
		scale.x = (float)(window->getSize().x) / (float)(hudTexture.getSize().x);
		scale.y = (float)(window->getSize().y) / (float)(hudTexture.getSize().y);
		hudSprite->setScale(scale);

		enemyDot.setFillColor(sf::Color::Red);

		if (!arial.loadFromFile("fonts/arial.ttf"))
		{
			std::cout << "ERROR Loading fonts/arial.ttf" << std::endl;
		}
		text.setFont(arial);
		text.setCharacterSize(14);
		text.setColor(sf::Color::Black);

		createUIElements();
	}

	void HUD::createUIElements()
	{
		// Resource bars
		UIElement healthBar(325, 183, 880, 88);
		resourceBars.push_back(healthBar);
		UIElement manaBar(325, 282, 880, 88);
		resourceBars.push_back(manaBar);
		UIElement structureBar(325, 528, 880, 96);
		resourceBars.push_back(structureBar);

		// Static UI elements
		UIElement healthIcon(192, 172, 79, 92);
		staticElements.push_back(healthIcon);
		UIElement healthBarBG;
		healthBarBG.pos = sf::Vector2i(325, 183);
		healthBarBG.rect = sf::IntRect(1481, 183, 883, 88);
		staticElements.push_back(healthBarBG);
		UIElement manaIcon(179, 269, 101, 119);
		staticElements.push_back(manaIcon);
		UIElement manaBarBG;
		manaBarBG.pos = sf::Vector2i(325, 282);
		manaBarBG.rect = sf::IntRect(1481, 183, 883, 88);
		staticElements.push_back(manaBarBG);
		UIElement structureIcon(124, 472, 157, 152);
		staticElements.push_back(structureIcon);
		UIElement structureBarBG;
		structureBarBG.pos = sf::Vector2i(325, 528);
		structureBarBG.rect = sf::IntRect(1481, 528, 880, 96);
		staticElements.push_back(structureBarBG);
		UIElement minimap(3208, 110, 801, 785);
		staticElements.push_back(minimap);
		UIElement enemyCountBG(3446, 962, 358, 85);
		staticElements.push_back(enemyCountBG);
		UIElement goldBG(3455, 1072, 356, 67);
		staticElements.push_back(goldBG);

		// Tutorial Animations
		UIElement pressEIcon(2500, 1424, 300, 282);
		eButtonAnimation.push_back(pressEIcon);
		UIElement pressEIconPressed;
		pressEIconPressed.pos = sf::Vector2i(2500, 1424);
		pressEIconPressed.rect = sf::IntRect(2800, 1424, 300, 282);
		eButtonAnimation.push_back(pressEIconPressed);

		UIElement mouseIcon(2507, 1728, 330, 480);
		mouseIcon.pos = sf::Vector2i(2500, 1424);
		mouseButtonAnimation.push_back(mouseIcon);
		UIElement mouseIconPressed;
		mouseIconPressed.pos = sf::Vector2i(2500, 1424);
		mouseIconPressed.rect = sf::IntRect(2837, 1728, 330, 480);
		mouseButtonAnimation.push_back(mouseIconPressed);
	}

	void HUD::update(const sf::Time &dt)
	{
		if (!eButtonPressed)
		{
			if (tutorialTimer > eButtonDelay)
			{
				eButtonFlashTimer += dt;
				if (eButtonFlashTimer >= flashDelay)
				{
					eButtonAnimIndex = (eButtonAnimIndex + 1) % eButtonAnimation.size();
					eButtonFlashTimer = sf::Time::Zero;
				}
			}
			else
			{
				tutorialTimer += dt;
			}

		}
		else if (!lMouseButtonPressed)
		{
			if (tutorialTimer > mouseButtonDelay)
			{
				mouseButtonFlashTimer += dt;
				if (mouseButtonFlashTimer >= flashDelay)
				{
					mouseButtonAnimIndex = (mouseButtonAnimIndex + 1) % mouseButtonAnimation.size();
					mouseButtonFlashTimer = sf::Time::Zero;
				}
			}
			else
			{
				tutorialTimer += dt;
			}
		}
	}

	void HUD::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		window->pushGLStates();

		// Static elements
		for (int i = 0; i < staticElements.size(); i++)
		{
			hudSprite->setTextureRect(staticElements[i].rect);
			hudSprite->setPosition(staticElements[i].pos.x * scale.x, staticElements[i].pos.y * scale.y);
			window->draw(*hudSprite);
		}

		Game* game = Game::getGame();

		// Enemies on minimap
		std::vector<Enemy*> enemies = game->getEnemies();
		sf::IntRect minimap(3208 * scale.x, 110 * scale.y, 801 * scale.x, 785 * scale.y);
		float mapWidth = game->getCurrentLevel()->mesh->getWidth();
		float mapDepth = game->getCurrentLevel()->mesh->getDepth();
		int enemyCount = 0;
		for (int i = 0; i < enemies.size(); i++)
		{
			if (!enemies[i]->toBeDeleted)
			{
				glm::vec3 pos(enemies[i]->localTransform.getPosition());

				pos.x += mapWidth / 2.f;
				pos.z += mapDepth / 2.f;

				enemyDot.setPosition(minimap.left + (pos.x / mapWidth) * minimap.width, minimap.top + (pos.z / mapDepth) * minimap.height);
				window->draw(enemyDot);
			}
		}

		// Health bar
		hudSprite->setTextureRect(resourceBars[0].rect);
		hudSprite->setPosition(resourceBars[0].pos.x * scale.x, resourceBars[0].pos.y * scale.y);
		float xScale = (float)game->getPlayer()->health / (float)game->getPlayer()->maxHealth;
		hudSprite->scale(sf::Vector2f(xScale, 1.f));
		window->draw(*hudSprite);
		hudSprite->setScale(scale);

		// Mana bar
		hudSprite->setTextureRect(resourceBars[1].rect);
		hudSprite->setPosition(resourceBars[1].pos.x * scale.x, resourceBars[1].pos.y * scale.y);
		window->draw(*hudSprite);

		// Structure bar
		hudSprite->setTextureRect(resourceBars[2].rect);
		hudSprite->setPosition(resourceBars[2].pos.x * scale.x, resourceBars[2].pos.y * scale.y);
		xScale = (float)game->getCurrentLevel()->gateHealth / (float)game->getCurrentLevel()->maxGateHealth;
		hudSprite->scale(sf::Vector2f(xScale, 1.f));
		window->draw(*hudSprite);
		hudSprite->setScale(scale);

		// Enemy count
		std::stringstream ss;
		ss << game->getCurrentLevel()->enemyCount;
		text.setString(ss.str());
		text.setPosition(3532 * scale.x, 975 * scale.y);
		window->draw(text);

		// Gold count
		ss.str("");
		ss.clear();
		ss << game->getPlayer()->gold;
		text.setString(ss.str());
		text.setPosition(3532 * scale.x, 1065 * scale.y);
		window->draw(text);
		
		if (!eButtonPressed && tutorialTimer > eButtonDelay)
		{
			hudSprite->setTextureRect(eButtonAnimation[eButtonAnimIndex].rect);
			hudSprite->setPosition(eButtonAnimation[eButtonAnimIndex].pos.x * scale.x, eButtonAnimation[eButtonAnimIndex].pos.y * scale.y);
			window->draw(*hudSprite);
		}
		else if (!lMouseButtonPressed && tutorialTimer > mouseButtonDelay)
		{
			// TODO have some elaborate code to detect if an enemy is nearby?
			hudSprite->setTextureRect(mouseButtonAnimation[mouseButtonAnimIndex].rect);
			hudSprite->setPosition(mouseButtonAnimation[mouseButtonAnimIndex].pos.x * scale.x, mouseButtonAnimation[mouseButtonAnimIndex].pos.y * scale.y);
			window->draw(*hudSprite);
		}

		window->draw(crossHairVert);
		window->draw(crossHairHoriz);

		window->popGLStates();
	}

	void HUD::scaleChanged()
	{
		scale.x = (float)(window->getSize().x) / (float)(hudTexture.getSize().x);
		scale.y = (float)(window->getSize().y) / (float)(hudTexture.getSize().y);
	}

	void HUD::keyPressed(sf::Event::KeyEvent e)
	{
		switch (e.code)
		{
		case sf::Keyboard::E:
			if (!eButtonPressed)
			{
				eButtonPressed = true;
				tutorialTimer = sf::Time::Zero;
			}
			break;
		default:
			break;
		}
	}

	void HUD::mouseButtonPressed(sf::Event::MouseButtonEvent e)
	{
		switch (e.button)
		{
		case sf::Mouse::Button::Left:
			if (eButtonPressed)
			{
				lMouseButtonPressed = true;
			}
			break;
		default:
			break;
		}
	}
}