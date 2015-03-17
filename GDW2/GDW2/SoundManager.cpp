#include "SoundManager.h"
#include <iostream>
#include "fmod_errors.h"

namespace flopse
{
	const float DISTANCE_FACTOR = 100.f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

	SoundManager::SoundManager()
	{
		initializeSystem();
	}

	SoundManager::~SoundManager()
	{
		errorCheck(system->close());
		errorCheck(system->release());
		delete system;
		delete masterGroup;
	}

	void SoundManager::initializeSystem()
	{
		unsigned int version;
		int key, numDrivers;
		FMOD_CAPS caps;
		FMOD_SPEAKERMODE speakerMode;
		char driverName[256];

		/*
		Create a System object and initialize.
		*/
		errorCheck(FMOD::System_Create(&system));

		errorCheck(system->getVersion(&version));

		if (version < FMOD_VERSION)
		{
			std::cout << "Error!  You are using an old version of FMOD " << version << ".  This program requires " << FMOD_VERSION << std::endl;
		}

		errorCheck(system->getNumDrivers(&numDrivers));

		if (numDrivers == 0)
		{
			errorCheck(system->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
		}
		else
		{
			errorCheck(system->getDriverCaps(0, &caps, 0, &speakerMode));

			errorCheck(system->setSpeakerMode(speakerMode));       /* Set the user selected speaker mode. */

			if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
			{                                                   /* You might want to warn the user about this. */
				errorCheck(system->setDSPBufferSize(1024, 10));
			}

			errorCheck(system->getDriverInfo(0, driverName, 256, 0));

			if (strstr(driverName, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
			{
				errorCheck(system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR));
			}
		}

		// Set flag so that the coordinate system matches OpenGL's right-handed coordinate system.
		FMOD_RESULT result = system->init(100, FMOD_INIT_3D_RIGHTHANDED, 0);
		if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
		{
			errorCheck(system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));

			errorCheck(system->init(100, FMOD_INIT_NORMAL, 0));/* ... and re-init. */
		}

		/*
		Set the distance units. (meters/feet etc).
		*/
		errorCheck(system->set3DSettings(1.0f, DISTANCE_FACTOR, 1.0f));

		// Create the global channel group
		errorCheck(system->createChannelGroup(NULL, &masterGroup));
		setMasterVolume(0.5f);

		// Set listener attributes:
		FMOD_VECTOR pos = { 0.f, 0.f, 0.f }, vel = { 0.f, 0.f, 0.f }, forward = { 0.f, 0.f, 1.f }, up = { 0.f, 1.f, 0.f };
		if (listener)
		{
			glm::vec3 glmPos = listener->getGlobalPosition();
			glm::vec3 glmFront = listener->getGlobalFront();
			pos = { glmPos.x, glmPos.y, glmPos.z };
			forward = { glmFront.x, glmFront.y, glmFront.z };
		}
		errorCheck(system->set3DListenerAttributes(0, &pos, &vel, &forward, &up));
	}

	void SoundManager::errorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		}
	}

	void SoundManager::createSound(std::string path, FMOD::Sound** sound, bool threeDimensional)
	{
		if (threeDimensional)
		{
			errorCheck(system->createSound(path.c_str(), FMOD_3D, 0, sound));

			// Multiply by distance factor to convert meters to centimeters, which is the unit used by the rest of the engine.
			// Min distance is 50cm, max distance is 5m.
			(*sound)->set3DMinMaxDistance(0.5f * DISTANCE_FACTOR, 5.0f * DISTANCE_FACTOR);
		}
		else
		{
			errorCheck(system->createSound(path.c_str(), FMOD_2D, 0, sound));
		}
	}

	void SoundManager::playSound(FMOD::Sound* sound, FMOD::Channel** channel, FMOD_VECTOR pos, FMOD_VECTOR vel)
	{
		// Start the sound paused. This is so that looping sounds aren't forced to begin playing when they are created.
		errorCheck(system->playSound(FMOD_CHANNEL_FREE, sound, true, channel));
		FMOD_MODE mode;
		sound->getMode(&mode);
		if (mode == mode & FMOD_3D)
		{
			errorCheck((*channel)->set3DAttributes(&pos, &vel));
		}
		// Add this channel to the master channel group so that it will be affected by the master volume.
		errorCheck((*channel)->setChannelGroup(masterGroup));
	}

	void SoundManager::update()
	{
		FMOD_VECTOR pos = { 0.f, 0.f, 0.f }, vel = { 0.f, 0.f, 0.f }, forward = { 0.f, 0.f, 1.f }, up = { 0.f, 1.f, 0.f };
		if (listener)
		{
			glm::vec3 glmPos = listener->getGlobalPosition();
			glm::vec3 glmFront = listener->getGlobalFront();
			pos = { glmPos.x, glmPos.y, glmPos.z };
			forward = { glmFront.x, glmFront.y, glmFront.z };
		}
		errorCheck(system->set3DListenerAttributes(0, &pos, &vel, &forward, &up));

		system->update();
	}

	void SoundManager::setMasterVolume(float v)
	{
		// Limit v to between 0.0 and 1.0
		if (v > 1.f)
		{
			v = 1.f;
		}
		else if (v < 0.f)
		{
			v = 0.f;
		}

		masterGroup->setVolume(v);
	}

	float SoundManager::getMasterVolume()
	{
		float v;
		masterGroup->getVolume(&v);
		return v;
	}

	void SoundManager::setListener(std::shared_ptr<Entity> l)
	{
		listener = l;
	}
}