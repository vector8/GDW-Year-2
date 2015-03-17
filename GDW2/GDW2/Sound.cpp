#include "Sound.h"
#include <assert.h>

#include "SoundManager.h"

namespace flopse
{
	Sound::Sound(std::string filename, bool loop, bool threeDimensional)
	{
		pos = { 0.0f, 0.0f, 0.0f };
		vel = { 0.0f, 0.0f, 0.0f };

		SoundManager::getSoundManager()->createSound(filename, &sound, threeDimensional);
		setLoop(loop);
	}

	Sound::~Sound()
	{
		sound->release();
		sound = nullptr;
		channel = nullptr;
	}
	
	void Sound::play()
	{
		if (loopMode == FMOD_LOOP_OFF)
		{
			// If this sound does not loop, we must set it to play using the sound manager.
			SoundManager::getSoundManager()->playSound(sound, &channel, pos, vel);
		}

		setPaused(false);
	}

	void Sound::setPaused(bool p)
	{
		if (channel)	// Only set paused on this channel if it is currently in use.
		{
			channel->setPaused(p);
		}
	}

	void Sound::setLoop(bool loop)
	{
		assert(sound);

		if (loop)
		{
			loopMode = FMOD_LOOP_NORMAL;
			// Call playSound so that the channel gets set to this sound.
			SoundManager::getSoundManager()->playSound(sound, &channel, pos, vel);
			if (channel)
			{
				// If playSound was successful, set the channel's mode so that it updates immediately.
				channel->setMode(loopMode | rollOffModel);
			}
		}
		else
		{
			loopMode = FMOD_LOOP_OFF;
		}

		// Set the mode on the sound object so that the "default" will be set if it is played later.
		sound->setMode(loopMode | rollOffModel);
	}

	void Sound::setPosition(glm::vec3 pos)
	{
		this->pos = { pos.x, pos.y, pos.z };

		if (channel)
		{
			// If the channel is in use (i.e., the sound is currently playing), set the 3D attributes so that they update immediately.
			channel->set3DAttributes(&(this->pos), &vel);
		}
	}

	void Sound::setRollOffModel(FMOD_MODE mode)
	{
		if (mode == FMOD_3D_INVERSEROLLOFF || mode == FMOD_3D_LINEARROLLOFF)
		{
			rollOffModel = mode;

			// setMode is a function that sets many different flags, so we use a bitwise OR operator to set both loop and roll-off simultaneously.
			sound->setMode(loopMode | rollOffModel);

			if (channel)
			{
				// Update the channel if it is currently in use.
				channel->setMode(loopMode | rollOffModel);
			}
		}
	}

	void Sound::toggleRollOffModel()
	{
		// Switch between logarithmic roll-off and linear roll-off.
		if (rollOffModel == FMOD_3D_INVERSEROLLOFF)
		{
			rollOffModel = FMOD_3D_LINEARROLLOFF;
		}
		else
		{
			rollOffModel = FMOD_3D_INVERSEROLLOFF;
		}

		sound->setMode(loopMode | rollOffModel);

		if (channel)
		{
			// Set the mode on the channel if it is currently in use, so that it will update immediately.
			channel->setMode(loopMode | rollOffModel);
		}
	}
}