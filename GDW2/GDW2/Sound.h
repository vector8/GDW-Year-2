#pragma once
#include "fmod.hpp"
#include <string>
#include <glm\glm.hpp>

namespace flopse
{
	class Sound
	{
	private:
		FMOD::Channel *channel = nullptr;
		FMOD::Sound *sound = nullptr;
		FMOD_MODE loopMode;
		FMOD_MODE rollOffModel = FMOD_3D_INVERSEROLLOFF;

		FMOD_VECTOR pos;
		FMOD_VECTOR vel;

	public:
		Sound(std::string filename, bool loop = false, bool threeDimensional = true);
		virtual ~Sound();

		void play();

		// Pause or unpause this sound.
		void setPaused(bool p);

		// Set whether this sound should loop or not.
		void setLoop(bool loop);

		// Set the roll-off model for this sound. FMOD_3D_INVERSEROLLOFF will set a logarithmic roll-off, while FMOD_3D_LINEARROLLOFF will set a linear roll-off. These are the only two options allowed.
		void setRollOffModel(FMOD_MODE mode);
		// Switches the roll-off model between logarithmic (default) and linear.
		void toggleRollOffModel();

		// Sets the position of this sound. The parameter represents the position in world-space.
		void setPosition(glm::vec3 pos);

	};
}