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
		sf::IntRect rect, hoverRect, pressedRect;
		std::string name, hoverName, pressedName;
		ButtonState state = ButtonState::Normal;
		bool visible = true;
	};
}