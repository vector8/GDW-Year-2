#pragma once
#include <vector>
#include "glm\glm.hpp"

namespace flopse
{
	struct Point
	{
		Point() : location(glm::vec3(0.f, 0.f, 0.f)), neighbours(std::vector < Point* > {})
		{}

		Point(glm::vec3 location, std::vector<Point*> neighbours) : location(location), neighbours(neighbours)
		{}

		glm::vec3 location;
		std::vector<Point*> neighbours;
	};

	class PointCollection
	{
	private:
		//std::vector<DistanceTableEntry> distanceTable;
		

	public:
		PointCollection(std::vector<Point> points);
		PointCollection();
		virtual ~PointCollection();

		std::vector<Point*> collection;

		glm::vec3 getPoint(float distance);

		std::vector<int> begin;

		void createCollection(const std::string &filename);

		float getMaxDistance();
	};
}