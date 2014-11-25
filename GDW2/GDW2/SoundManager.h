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
	};
}