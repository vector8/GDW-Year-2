#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Sound.h"

namespace flopse
{
	class Player : public Entity
	{
	private:
		float dy = 0.f;
		Animation* runAnimation;
		Animation* idleAnimation;
		Sound* footsteps;
		std::shared_ptr<SceneNode> camJoint = nullptr;

		void initAnimations();

	public:
		Player();
		virtual ~Player();

		bool jumping = false;
		int gold = 150;
		int health = 100;
		int maxHealth = 100;

		void attachCam(const std::shared_ptr<SceneNode> &n);
		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);

		void jump();

		glm::vec3 getAimDirection();
	};
}