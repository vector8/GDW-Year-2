#pragma once
#include "fmod.hpp"
#include "Entity.h"
#include "Sound.h"

namespace flopse
{
	enum class DefaultSounds
	{
		Ping,
		Arrow,
		Frost,
		Catapult,
		Clank,
		MetalSmack,
		Footstep,
		Error
	};

	class SoundManager
	{
	private:
		SoundManager();

		FMOD::System *system;

		FMOD::ChannelGroup *masterGroup;

		std::shared_ptr<Entity> listener = nullptr;

		// Initializes the FMOD system.
		void initializeSystem();

		// Basic function for checking FMOD error results.
		void errorCheck(FMOD_RESULT result);

	public:
		static SoundManager* getSoundManager()
		{
			static SoundManager* sm = new SoundManager();

			return sm;
		}

		virtual ~SoundManager();

		// Basic method to play a certain sound at a certain position in the world. The sound is returned so that it can be updated if required.
		static Sound* playSoundAt(DefaultSounds sound, glm::vec3 position, bool loop = false)
		{
			Sound* s = nullptr;

			switch (sound)
			{
			case flopse::DefaultSounds::Ping:
				s = new Sound("sounds/fireball.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Frost:
				s = new Sound("sounds/frostball.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Catapult:
				s = new Sound("sounds/arrow.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Arrow:
				s = new Sound("sounds/arrow.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Clank:
				s = new Sound("sounds/clank.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::MetalSmack:
				s = new Sound("sounds/gateSmack.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Footstep:
				s = new Sound("sounds/footstep.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			case flopse::DefaultSounds::Error:
				s = new Sound("sounds/fart.wav", loop);
				s->setPosition(position);
				s->play();
				break;
			default:
				break;
			}

			return s;
		}

		static Sound* playSound(DefaultSounds sound, bool loop = false)
		{
			Sound* s = nullptr;

			switch (sound)
			{
			case flopse::DefaultSounds::Ping:
				s = new Sound("sounds/fireball.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Frost:
				s = new Sound("sounds/frostball.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Catapult:
				s = new Sound("sounds/arrow.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Arrow:
				s = new Sound("sounds/arrow.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Clank:
				s = new Sound("sounds/clank.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::MetalSmack:
				s = new Sound("sounds/gateSmack.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Footstep:
				s = new Sound("sounds/footstep.wav", loop, false);
				s->play();
				break;
			case flopse::DefaultSounds::Error:
				s = new Sound("sounds/fart.wav", loop, false);
				s->play();
				break;
			default:
				break;
			}

			return s;
		}

		// Create a sound corresponding to the file at the given path, and store it in the provided sound variable.
		void createSound(std::string path, FMOD::Sound** sound, bool threeDimensional = true);
		// Play the given sound on the given channel, using the provided position and velocity parameters.
		void playSound(FMOD::Sound* sound, FMOD::Channel** channel, FMOD_VECTOR pos, FMOD_VECTOR vel);

		// Update the sound manager. This is called every frame.
		void update();

		// Set the volume for all sounds. The parameter should be a value between 0.0 and 1.0, and is multiplicative with the volume set on individual sounds.
		void setMasterVolume(float v);
		// Get the master volume.
		float getMasterVolume();

		void setListener(std::shared_ptr<Entity> l);
	};
}