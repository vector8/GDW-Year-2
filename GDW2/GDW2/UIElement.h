#pragma once
#include <SFML\Graphics.hpp>

namespace flopse
{
	// Static UI Element
	struct UIElement
	{
		sf::Vector2i pos;
		sf::IntRect rect;
		std::string name;
		bool visible = true;
	};

	enum class ButtonState
	{
		Pressed,
		Hover,
		Normal
	};

	// Clickable button. Has separate frames for normal, hover, and pressed.
	struct Button
	{
		sf::Vector2i pos;
		sf::IntRect rect, hoverRect, pressedRect;
		std::string name, hoverName, pressedName;
		ButtonState state = ButtonState::Normal;
		bool visible = true;
	};

	// Animated UI Element.
	struct AnimatedUIElement
	{
		std::vector<std::pair<UIElement, sf::Time>> frames; // Pair.first is frame, Pair.second is time for that frame.
		int frameIndex = 0;
		sf::Time animationTimer;
		bool visible = true;
	};
}