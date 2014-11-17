#pragma once
#include "Entity.h"

namespace flopse
{
	class Player : public Entity
	{
	private:
		bool jumping;
		float dy;

	public:
		Player(Model* model);

		void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		void jump();
	};
}