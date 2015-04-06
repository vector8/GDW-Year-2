#include "Path.h"
#include "Utilities.h"
#include <vector>

namespace flopse
{
	Path::Path(std::vector<glm::vec3> points)
	{
		if (points.size() < 2)
		{
			if (points.size() == 1)
			{
				this->distanceTable.push_back(DistanceTableEntry(0.f, points[0], 0.f));
			}
			return;
		}

		float currentDistance = 0.f;

		// first segment
		glm::vec3 p4;
		if (points.size() == 2)
		{
			p4 = points[1];
		}
		else
		{
			p4 = points[2];
		}

		this->distanceTable.push_back(DistanceTableEntry(0.f, points[0], currentDistance));

		for (float u = 0.01f; u < 1.001f; u += 0.01f)
		{
			glm::vec3 p = catmull(u, points[0], points[0], points[1], p4);

			currentDistance += glm::length(p - this->distanceTable.back().point);
			this->distanceTable.push_back(DistanceTableEntry(u, p, currentDistance));
		}

		for (int i = 1; i < points.size() - 2; i++)
		{
			this->distanceTable.push_back(DistanceTableEntry(0.f, points[i], currentDistance));

			for (float u = 0.01f; u < 1.001f; u += 0.01f)
			{
				glm::vec3 p = catmull(u, points[i - 1], points[i], points[i + 1], points[i + 2]);

				currentDistance += glm::length(p - this->distanceTable.back().point);
				this->distanceTable.push_back(DistanceTableEntry(u, p, currentDistance));
			}
		}

		// last segment
		if (points.size() > 2)
		{
			this->distanceTable.push_back(DistanceTableEntry(0.f, points[points.size() - 2], currentDistance));

			for (float u = 0.01f; u < 1.001f; u += 0.01f)
			{
				glm::vec3 p = catmull(u, points[points.size() - 3], points[points.size() - 2], points[points.size() - 1], points[points.size() - 1]);

				currentDistance += glm::length(p - this->distanceTable.back().point);
				this->distanceTable.push_back(DistanceTableEntry(u, p, currentDistance));
			}
		}
	}

	Path::~Path()
	{

	}

	glm::vec3 Path::getPoint(float distance)
	{
		glm::vec3 result;

		if (this->distanceTable.size() > 0)
		{
			result = this->distanceTable.back().point;
		}

		if (distance >= 0.f)
		{
			for (int i = 1; i < this->distanceTable.size(); i++)
			{
				if (distance < this->distanceTable[i].distance)
				{
					float u = (distance - this->distanceTable[i - 1].distance) / (this->distanceTable[i].distance - this->distanceTable[i - 1].distance);
					result = lerp(u, this->distanceTable[i - 1].point, this->distanceTable[i].point);
					break;
				}
			}
		}

		return result;
	}

	float Path::getMaxDistance()
	{
		if (this->distanceTable.size() > 0)
		{
			return distanceTable.back().distance;
		}
		else
		{
			return 0.f;
		}
	}
}