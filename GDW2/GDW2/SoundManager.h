#pragma once
#include <SFML\Audio.hpp>

namespace flopse
{
	class SoundManager
	{
	private:
		SoundManager();

		sf::Sound footSteps;
		sf::Sound pewPew;
		sf::Sound clankSound;
		sf::Sound gateSmackSound;

	public:
		static SoundManager* getSoundManager()
		{
			static SoundManager* sm = new SoundManager();

			return sm;
		}

		void startFootSteps();
		void stopFootSteps();

		void pew();

		void clank();

		void gateSmack();
	};
}