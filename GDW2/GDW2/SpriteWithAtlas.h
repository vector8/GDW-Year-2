#pragma once
#include <SFML\Graphics.hpp>

namespace flopse
{
	struct SpriteFrame
	{
		std::string name;
		sf::IntRect rect;
	};

	class SpriteWithAtlas
	{
	private:
		sf::Texture texture;
		sf::Sprite* sprite;
		std::vector<SpriteFrame> frames;
		
	public:
		SpriteWithAtlas(std::string name);

		bool setToFrame(std::string frameName);
		SpriteFrame getFrame(std::string frameName);

		sf::Sprite* getSprite();

		void setScale(float x, float y);
		void setPosition(float x, float y);
	};
}