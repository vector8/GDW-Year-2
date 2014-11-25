#include "SoundManager.h"
#include <iostream>

namespace flopse
{
	SoundManager::SoundManager()
	{
		sf::SoundBuffer* stepBuffer = new sf::SoundBuffer();
		if (!stepBuffer->loadFromFile("sounds/footstep.wav"))
		{
			std::cout << "ERROR loading sound sounds/footstep.wav" << std::endl;
		}
		footSteps.setBuffer(*stepBuffer);
		footSteps.setLoop(true);

		sf::SoundBuffer* pewBuffer = new sf::SoundBuffer();
		if (!pewBuffer->loadFromFile("sounds/pew.flac"))
		{
			std::cout << "ERROR loading sound sounds/pew.flac" << std::endl;
		}
		pewPew.setBuffer(*pewBuffer);

		sf::SoundBuffer* clankBuffer = new sf::SoundBuffer();
		if (!clankBuffer->loadFromFile("sounds/clank.wav"))
		{
			std::cout << "ERROR loading sound sounds/clank.wav" << std::endl;
		}
		clankSound.setBuffer(*clankBuffer);
	}

	void SoundManager::startFootSteps()
	{
		if (footSteps.getStatus() != sf::Sound::Playing)
		{
			footSteps.play();
		}
	}

	void SoundManager::stopFootSteps()
	{
		footSteps.stop();
	}

	void SoundManager::pew()
	{
		pewPew.play();
	}

	void SoundManager::clank()
	{
		clankSound.play();
	}
}