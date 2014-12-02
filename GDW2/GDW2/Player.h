#pragma once
#include "Entity.h"

namespace flopse
{
	class Player : public Entity
	{
	private:
		bool jumping = false;
		float dy = 0.f;

	public:
		Player(Mesh *m);

		int gold = 150;
		int health = 100;
		int maxHealth = 100;

		void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		void jump();
	};
}