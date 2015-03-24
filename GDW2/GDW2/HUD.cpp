#include "HUD.h"
#include <iostream>
#include <GL\glew.h>
#include "Game.h"
#include <sstream>

namespace flopse
{
	HUD::HUD(sf::RenderWindow* window) : window(window), crossHairVert(sf::Vector2f(4, 24)), crossHairHoriz(sf::Vector2f(25, 5)), enemyDot(3), playerDot(3), hudSprite("HUDSpriteSheet")
	{
		crossHairVert.setPosition(sf::Vector2f(((float)(window->getSize().x) / 2.f) - 14.f, ((float)(window->getSize().y) / 2.f) - 12.f));
		crossHairHoriz.setPosition(sf::Vector2f(((float)(window->getSize().x) / 2.f) - 24.f, ((float)(window->getSize().y) / 2.f) - 2.f));

		scaleChanged();

		enemyDot.setFillColor(sf::Color::Red);
		playerDot.setFillColor(sf::Color::Blue);

		if (!arial.loadFromFile("fonts/arial.ttf"))
		{
			std::cout << "ERROR Loading fonts/arial.ttf" << std::endl;
		}
		text.setFont(arial);
		text.setCharacterSize(24);
		text.setColor(sf::Color::Black);

		createUIElements();
	}
	
	HUD::~HUD()
	{
	}

	void HUD::createUIElements()
	{
		SpriteFrame currentFrame;
		UIElement currentElement;
		int prevDimension = 0, prevDimension2 = 0;

		/// Static UI elements ///
		// Enemy Count Icon
		currentFrame = hudSprite.getFrame("EnemyCounter.png");
		currentElement.rect = currentFrame.rect;
		prevDimension = currentElement.rect.height * scale.x;
		currentElement.name = currentFrame.name;
		currentElement.pos = sf::Vector2i(window->getSize().x - 245, 270);
		staticElements.push_back(currentElement);
		// Gold Icon
		currentFrame = hudSprite.getFrame("GoldCounter.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		currentElement.pos = sf::Vector2i(window->getSize().x - 245, prevDimension + 280);
		staticElements.push_back(currentElement);
		/*UIElement enemyCountBG(3446, 962, 358, 85);
		staticElements.push_back(enemyCountBG);
		UIElement goldBG(3455, 1072, 356, 67);
		staticElements.push_back(goldBG);*/
		// Castle Icon
		currentFrame = hudSprite.getFrame("CastleHealthIcon.png");
		currentElement.rect = currentFrame.rect;
		prevDimension = currentElement.rect.width * scale.x;
		prevDimension2 = currentElement.rect.height * scale.y;
		currentElement.name = currentFrame.name;
		currentElement.pos = sf::Vector2i(50, 50);
		staticElements.push_back(currentElement);
		// Castle Health BG
		currentFrame = hudSprite.getFrame("CastleHealthEmpty.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		currentElement.pos = sf::Vector2i(prevDimension + 70, 50 + (prevDimension2 / 2.f) - ((currentElement.rect.height * scale.y) / 2.f));
		staticElements.push_back(currentElement);

		/// Health bar ///
		currentFrame = hudSprite.getFrame("CastleHealthFull.png");
		castleHealthBar.rect = currentFrame.rect;
		castleHealthBar.name = currentFrame.name;
		castleHealthBar.pos = sf::Vector2i(prevDimension + 70, 50 + (prevDimension2 / 2.f) - ((castleHealthBar.rect.height * scale.y) / 2.f));

		/// Tutorial Animations///
		sf::Time frameTime = sf::seconds(1.f);
		// E
		currentFrame = hudSprite.getFrame("EKey.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		currentElement.pos = sf::Vector2i(window->getSize().x * 0.6f, window->getSize().y * 0.4f);
		eKeyAnimation.frames.push_back(std::pair<UIElement, sf::Time>(currentElement, frameTime));
		currentFrame = hudSprite.getFrame("EKeyHilighted.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		eKeyAnimation.frames.push_back(std::pair<UIElement, sf::Time>(currentElement, frameTime));
		// Left Mouse Button
		currentFrame = hudSprite.getFrame("Mouse.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		mouseButtonAnimation.frames.push_back(std::pair<UIElement, sf::Time>(currentElement, frameTime));
		currentFrame = hudSprite.getFrame("MouseHilighted.png");
		currentElement.rect = currentFrame.rect;
		currentElement.name = currentFrame.name;
		mouseButtonAnimation.frames.push_back(std::pair<UIElement, sf::Time>(currentElement, frameTime));
	}

	void HUD::update(const sf::Time &dt)
	{
		if (!eButtonPressed)
		{
			if (tutorialTimer > eButtonDelay)
			{
				eKeyAnimation.animationTimer += dt;
				if (eKeyAnimation.animationTimer >= eKeyAnimation.frames[eKeyAnimation.frameIndex].second)
				{
					eKeyAnimation.frameIndex = (eKeyAnimation.frameIndex + 1) % eKeyAnimation.frames.size();
					eKeyAnimation.animationTimer = sf::Time::Zero;
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
				mouseButtonAnimation.animationTimer += dt;
				if (mouseButtonAnimation.animationTimer >= mouseButtonAnimation.frames[mouseButtonAnimation.frameIndex].second)
				{
					mouseButtonAnimation.frameIndex = (mouseButtonAnimation.frameIndex + 1) % mouseButtonAnimation.frames.size();
					mouseButtonAnimation.animationTimer = sf::Time::Zero;
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
		sf::Sprite* s = hudSprite.getSprite();
		for (int i = 0; i < staticElements.size(); i++)
		{
			hudSprite.setToFrame(staticElements[i].name);
			hudSprite.setPosition(staticElements[i].pos);
			window->draw(*s);
		}

		Game* game = Game::getGame();

		// Enemies on minimap
		std::vector<std::shared_ptr<Enemy>> enemies = game->getEnemies();
		sf::IntRect minimap(window->getSize().x - 245, 50, 195, 195);
		float mapWidth = game->getCurrentLevel()->mesh->getWidth();
		float mapDepth = game->getCurrentLevel()->mesh->getDepth();
		for (int i = 0; i < enemies.size(); i++)
		{
			if (!enemies[i]->toBeDeleted)
			{
				glm::vec3 pos(enemies[i]->getGlobalPosition());

				pos.x += mapWidth / 2.f;
				pos.z += mapDepth / 2.f;

				enemyDot.setPosition(minimap.left + ((mapWidth - pos.x) / mapWidth) * minimap.width - enemyDot.getRadius(), minimap.top + ((mapDepth - pos.z) / mapDepth) * minimap.height - enemyDot.getRadius());
				window->draw(enemyDot);
			}
		}

		// Player on minimap
		glm::vec3 ppos = game->getPlayer()->getGlobalPosition();
		ppos.x += mapWidth / 2.f;
		ppos.z += mapDepth / 2.f;
		playerDot.setPosition(minimap.left + ((mapWidth - ppos.x) / mapWidth) * minimap.width - playerDot.getRadius(), minimap.top + ((mapDepth - ppos.z) / mapDepth) * minimap.height - playerDot.getRadius());
		window->draw(playerDot);

		// Structure bar
		hudSprite.setToFrame(castleHealthBar.name);
		hudSprite.setPosition(castleHealthBar.pos);
		hudSprite.setScale(scale.x * (float)game->getCurrentLevel()->gateHealth / (float)game->getCurrentLevel()->maxGateHealth, scale.y);
		window->draw(*s);
		hudSprite.setScale(scale.x, scale.y);

		// Enemy count
		std::stringstream ss;
		float x, y;
		x = staticElements[0].pos.x + staticElements[0].rect.width * scale.x + 10.f;
		y = staticElements[0].pos.y + staticElements[0].rect.height * scale.y * 0.25;
		ss << game->getCurrentLevel()->enemyCount;
		text.setString(ss.str());
		text.setPosition(x, y);
		window->draw(text);

		// Gold count
		x = staticElements[1].pos.x + staticElements[1].rect.width * scale.x + 10.f;
		y = staticElements[1].pos.y + staticElements[1].rect.height * scale.y * 0.25;
		ss.str("");
		ss.clear();
		ss << game->getPlayer()->gold;
		text.setString(ss.str());
		text.setPosition(x, y);
		window->draw(text);
		
		if (!eButtonPressed && tutorialTimer > eButtonDelay)
		{
			hudSprite.setToFrame(eKeyAnimation.frames[eKeyAnimation.frameIndex].first.name);
			hudSprite.setPosition(eKeyAnimation.frames[eKeyAnimation.frameIndex].first.pos);
			window->draw(*s);
		}
		else if (!lMouseButtonPressed && tutorialTimer > mouseButtonDelay)
		{
			// TODO have some elaborate code to detect if an enemy is nearby?
			hudSprite.setToFrame(mouseButtonAnimation.frames[mouseButtonAnimation.frameIndex].first.name);
			hudSprite.setPosition(mouseButtonAnimation.frames[mouseButtonAnimation.frameIndex].first.pos);
			window->draw(*s);
		}

		window->draw(crossHairVert);
		window->draw(crossHairHoriz);

		window->popGLStates();
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
		case sf::Keyboard::Num1:
			currentTower = 0; //ArrowTower
			break;
		case sf::Keyboard::Num2:
			currentTower = 1; //FrostTower
			break;
		case sf::Keyboard::Num3:
			currentTower = 2; //FireTower
			break;
		case sf::Keyboard::Num4:
			currentTower = 3; //Catapult
			break;
		//case sf::Keyboard::Num5:
		//	currentTower = 4; //Barricade
		//	break;
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

	void HUD::scaleChanged()
	{
		if (window->getSize().x <= window->getSize().y)
		{
			scale.x = 0.5f * (float)(window->getSize().x) / (float)(hudSprite.getSprite()->getTexture()->getSize().x);
			scale.y = scale.x;
		}
		else
		{
			scale.y = 0.5f * (float)(window->getSize().y) / (float)(hudSprite.getSprite()->getTexture()->getSize().y);
			scale.x = scale.y;
		}

		hudSprite.setScale(scale.x, scale.y);
	}
}