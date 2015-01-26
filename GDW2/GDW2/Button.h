#pragma once
#include <SFML\Graphics.hpp>

namespace flopse
{
	enum class ButtonState
	{
		Pressed,
		Hover,
		Normal
	};

	struct Button
	{
		sf::Vector2i pos;
		sf::IntRect rect;
		std::string name;
		ButtonState state = ButtonState::Normal;
		bool visible = true;
	};
}