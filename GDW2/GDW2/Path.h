#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "PointCollection.h"

namespace flopse
{
	struct DistanceTableEntry
	{
		DistanceTableEntry() : u(0.f), point(glm::vec3(0.f, 0.f, 0.f)), distance(0.f)
		{}

		DistanceTableEntry(float u, glm::vec3 point, float distance) : u(u), point(point), distance(distance)
		{}

		float u;
		glm::vec3 point;
		float distance;
	};

	class Path
	{
	private:
		std::vector<DistanceTableEntry> distanceTable;

	public:
		Path(std::vector<glm::vec3> points);
		virtual ~Path();

		glm::vec3 getPoint(float distance);

		float getMaxDistance();
	};
}