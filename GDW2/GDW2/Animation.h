#pragma once
#include <vector>
#include <SFML\System.hpp>
#include "Mesh.h"

namespace flopse
{
	struct Keyframe
	{
		Mesh* mesh;
		sf::Time duration;
	};

	class Animation
	{
	private:
		int currentKeyframe = 0;
		sf::Time elapsed;
		std::vector<Keyframe> keyframes;

		Mesh* currentMesh;

	public:
		Animation(const std::vector<Keyframe> frames);

		void update(const sf::Time &dt);

		Mesh* getCurrentMesh();
	};
}