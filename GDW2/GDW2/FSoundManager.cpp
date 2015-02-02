#include "FSoundManager.h"
#include <iostream>
#include "fmod_errors.h"

namespace flopse
{
	FSoundManager::FSoundManager()
	{
		initializeSystem(); 
		loadSounds();
	}

	FSoundManager::~FSoundManager()
	{
		errorCheck(footSteps->release());
		errorCheck(pewPew->release());
		errorCheck(clankSound->release());
		errorCheck(gateSmackSound->release());

		errorCheck(system->close());
		errorCheck(system->release());
	}

	void FSoundManager::initializeSystem()
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

		result = system->init(100, FMOD_INIT_NORMAL, 0);
		if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
		{
			errorCheck(system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO));

			errorCheck(system->init(100, FMOD_INIT_NORMAL, 0));/* ... and re-init. */
		}


		/*
		Set the distance units. (meters/feet etc).
		*/
		errorCheck(system->set3DSettings(1.0f, 1.0f, 1.0f));

		// create the global channel group
		errorCheck(system->createChannelGroup(NULL, &channelGroup));
		setMasterVolume(0.5f);
	}

	void FSoundManager::loadSounds()
	{
		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

		errorCheck(system->createSound("sounds/footstep.wav", FMOD_3D, 0, &footSteps));
		errorCheck(footSteps->set3DMinMaxDistance(0.5f, 100.0f));
		errorCheck(footSteps->setMode(FMOD_LOOP_NORMAL));
		errorCheck(system->playSound(FMOD_CHANNEL_FREE, footSteps, true, &channel1));
		errorCheck(channel1->set3DAttributes(&pos, &vel));
		errorCheck(channel1->setChannelGroup(channelGroup));

		errorCheck(system->createSound("sounds/pew.wav", FMOD_3D, 0, &pewPew));
		errorCheck(pewPew->set3DMinMaxDistance(0.5f, 100.0f));
		errorCheck(pewPew->setMode(FMOD_LOOP_OFF));

		errorCheck(system->createSound("sounds/clank.wav", FMOD_3D, 0, &clankSound));
		errorCheck(clankSound->set3DMinMaxDistance(0.5f, 100.0f));
		errorCheck(clankSound->setMode(FMOD_LOOP_OFF));

		errorCheck(system->createSound("sounds/gateSmack.wav", FMOD_3D, 0, &gateSmackSound));
		errorCheck(gateSmackSound->set3DMinMaxDistance(0.5f, 100.0f));
		errorCheck(gateSmackSound->setMode(FMOD_LOOP_OFF));
	}

	void FSoundManager::errorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		}
	}

	void FSoundManager::update()
	{
		system->update();
	}

	void FSoundManager::startFootSteps()
	{
		channel1->setPaused(false);
	}

	void FSoundManager::stopFootSteps()
	{
		channel1->setPaused(true);
	}

	void FSoundManager::pew()
	{
		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		errorCheck(system->playSound(FMOD_CHANNEL_FREE, pewPew, true, &channel2));
		errorCheck(channel2->set3DAttributes(&pos, &vel));
		errorCheck(channel2->setChannelGroup(channelGroup));
		errorCheck(channel2->setPaused(false));
	}

	void FSoundManager::clank()
	{
		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		errorCheck(system->playSound(FMOD_CHANNEL_FREE, clankSound, true, &channel3));
		errorCheck(channel3->set3DAttributes(&pos, &vel));
		errorCheck(channel2->setChannelGroup(channelGroup));
		errorCheck(channel3->setPaused(false));
	}

	void FSoundManager::gateSmack()
	{
		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		errorCheck(system->playSound(FMOD_CHANNEL_FREE, gateSmackSound, true, &channel4));
		errorCheck(channel4->set3DAttributes(&pos, &vel));
		errorCheck(channel2->setChannelGroup(channelGroup));
		errorCheck(channel4->setPaused(false));
	}

	void FSoundManager::setMasterVolume(float v)
	{
		if (v > 1.f)
		{
			v = 1.f;
		}
		else if (v < 0.f)
		{
			v = 0.f;
		}

		channelGroup->setVolume(v);
	}

	float FSoundManager::getMasterVolume()
	{
		float v;
		channelGroup->getVolume(&v);
		return v;
	}
}