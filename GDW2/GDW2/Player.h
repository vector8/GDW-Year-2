#pragma once
#include "Entity.h"
#include "Animation.h"

namespace flopse
{
	class Player : public Entity
	{
	private:
		float dy = 0.f;
		Animation* runAnimation;
		Mesh* idleMesh;

	public:
		Player(Mesh *m);

		bool jumping = false;
		int gold = 150;
		int health = 100;
		int maxHealth = 100;

		void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		void jump();
	};
}