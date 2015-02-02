#pragma once
#include "fmod.hpp"

namespace flopse
{
	class FSoundManager
	{
	private:
		FSoundManager();

		FMOD::System *system;
		FMOD_RESULT result;

		FMOD::Sound *footSteps;
		FMOD::Sound *pewPew;
		FMOD::Sound *clankSound;
		FMOD::Sound *gateSmackSound;
		FMOD::ChannelGroup *channelGroup;

		FMOD::Channel *channel1 = 0, *channel2 = 0, *channel3 = 0, *channel4 = 0;

		void initializeSystem();
		void loadSounds();
		void errorCheck(FMOD_RESULT result);

	public:
		static FSoundManager* getSoundManager()
		{
			static FSoundManager* sm = new FSoundManager();

			return sm;
		}

		~FSoundManager();

		void update();

		void startFootSteps();
		void stopFootSteps();
		void pew();
		void clank();
		void gateSmack();

		void setMasterVolume(float v);
		float getMasterVolume();
	};
}