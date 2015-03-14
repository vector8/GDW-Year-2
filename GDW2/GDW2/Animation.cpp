#include "Animation.h"
#include "Utilities.h"
#include <iostream>

namespace flopse
{
	Animation::Animation(const std::vector<Keyframe> frames) : keyframes(frames)
	{
		if (keyframes.size() > 0)
		{
			initKeyframes();
			currentMesh = keyframes[0].mesh;
		}
	}

	Animation::~Animation()
	{
	}

	void Animation::initKeyframes()
	{
		for (int i = 0; i < keyframes.size() - 1; i++)
		{
			glBindVertexArray(keyframes[i].mesh->VAO);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glBindBuffer(GL_ARRAY_BUFFER, keyframes[i + 1].mesh->VBO);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
			glBindVertexArray(0);
		}

		glBindVertexArray(keyframes[keyframes.size() - 1].mesh->VAO);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, keyframes[0].mesh->VBO);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glBindVertexArray(0);
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

			interpParam = elapsed.asSeconds() / keyframes[currentKeyframe].duration.asSeconds();
		}
	}

	std::shared_ptr<Mesh> Animation::getCurrentMesh()
	{
		keyframes[currentKeyframe].mesh->animationBlend = interpParam;
		return keyframes[currentKeyframe].mesh;
	}
}