#pragma once
#include <vector>
#include <SFML\System.hpp>
#include <memory>
#include "Mesh.h"

namespace flopse
{
	struct Keyframe
	{
		std::shared_ptr<Mesh> mesh;
		sf::Time duration;
	};

	class Animation
	{
	private:
		int currentKeyframe = 0;
		sf::Time elapsed;
		std::vector<Keyframe> keyframes;
		std::shared_ptr<Mesh> currentMesh;
		float interpParam = 0.f;

		void initKeyframes();

	public:
		Animation(const std::vector<Keyframe> frames);
		virtual ~Animation();

		void update(const sf::Time &dt);


		std::shared_ptr<Mesh> getCurrentMesh();
	};
}