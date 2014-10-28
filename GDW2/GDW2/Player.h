#pragma once
#include "Entity.h"

class Player : public Entity
{
private:
	bool jumping;
	float dy;

public:
	Player(const glm::vec3 &pos, const Model &model);

	void update(const sf::Window &window, const sf::Time &dt);

	void jump();
};