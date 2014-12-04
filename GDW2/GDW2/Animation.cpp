#include "Animation.h"
#include "Utilities.h"
#include <iostream>

namespace flopse
{
	Animation::Animation(const std::vector<Keyframe> frames) : keyframes(frames)
	{
		if (keyframes.size() > 0)
		{
			currentMesh = new Mesh(*keyframes[0].mesh);
		}
	}

	void Animation::update(const sf::Time &dt)
	{
		if (keyframes.size() > 1)
		{
			elapsed += dt;

			while (elapsed >= keyframes[currentKeyframe].duration)
			{
				elapsed -= keyframes[currentKeyframe].duration;
				currentKeyframe = (currentKeyframe + 1) % keyframes.size();
			}

			float interpParam = elapsed.asSeconds() / keyframes[currentKeyframe].duration.asSeconds();
			Mesh* target;

			// Set currentMesh by interpolating between this and next mesh
			if (currentKeyframe < keyframes.size() - 1)
			{
				target = keyframes[currentKeyframe + 1].mesh;
			}
			else // Interpolate between last frame and first frame
			{
				target = keyframes[0].mesh;
			}

			//currentMesh = keyframes[currentKeyframe].mesh;
			for (int i = 0; i < currentMesh->objData.size(); i += 8)
			{
				currentMesh->objData[i] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i], target->objData[i]);
				currentMesh->objData[i + 1] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i + 1], target->objData[i + 1]);
				currentMesh->objData[i + 2] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i + 2], target->objData[i + 2]);
				currentMesh->objData[i + 5] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i + 5], target->objData[i + 5]);
				currentMesh->objData[i + 6] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i + 6], target->objData[i + 6]);
				currentMesh->objData[i + 7] = lerp(interpParam, keyframes[currentKeyframe].mesh->objData[i + 7], target->objData[i + 7]);
			}

			currentMesh->refreshArrays();
		}
	}

	Mesh* Animation::getCurrentMesh()
	{
		return currentMesh;
	}
}