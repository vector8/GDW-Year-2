#include "SpriteWithAtlas.h"
#include <fstream>
#include <iostream>

namespace flopse
{
	SpriteWithAtlas::SpriteWithAtlas(std::string name)
	{
		if (!texture.loadFromFile("textures/" + name + ".png"))
		{
			std::cout << "ERROR LOADING textures/" + name + ".png" << std::endl;

			return;
		}

		sprite = new sf::Sprite(texture);

		std::ifstream xmlIn("textures/" + name + ".xml");
		if (!xmlIn)
		{
			std::cout << "Cannot open textures/" + name + ".xml" << std::endl;

			return;
		}

		std::string line;
		while (std::getline(xmlIn, line))
		{
			if (line.substr(0, 11) == "    <sprite")
			{
				SpriteFrame frame;

				size_t start = 15;
				size_t end = line.find('\"', start);

				frame.name = line.substr(start, end - start);

				size_t end2 = line.find(" pX=", end);

				std::string parseString = line.substr(end + 2, end2 - (end + 2));

				int number = sscanf_s(parseString.c_str(), "x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\"", &(frame.rect.left), &(frame.rect.top), &(frame.rect.width), &(frame.rect.height));
				if (number != 4)
				{
					std::cout << "ERROR: Sprite Atlas formatting error: " << line << std::endl;
				}

				frames.push_back(frame);
			}
			else
			{
				// skip this line
			}
		}
	}

	SpriteWithAtlas::~SpriteWithAtlas()
	{
		delete sprite;
	}

	bool SpriteWithAtlas::setToFrame(std::string frameName)
	{
		bool success = false;

		for (auto it = frames.begin(); it != frames.end(); it++)
		{
			if (it->name == frameName)
			{
				sprite->setTextureRect(it->rect);
				success = true;
				break;
			}
		}
		
		return success;
	}

	SpriteFrame SpriteWithAtlas::getFrame(std::string frameName)
	{
		for (auto it = frames.begin(); it != frames.end(); it++)
		{
			if (it->name == frameName)
			{
				return (*it);
			}
		}

		std::cout << "ERROR: SpriteWithAtlas::getFrame(): Frame " << frameName << " not found." << std::endl;
		SpriteFrame s;
		return s;
	}

	sf::Sprite* SpriteWithAtlas::getSprite()
	{
		return sprite;
	}

	void SpriteWithAtlas::setScale(float x, float y)
	{
		sprite->setScale(sf::Vector2f(x, y));
	}

	void SpriteWithAtlas::setPosition(float x, float y)
	{
		sprite->setPosition(x, y);
	}
}