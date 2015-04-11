#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "PointCollection.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Utilities.h"

namespace flopse
{
	PointCollection::PointCollection(std::vector<Point> points){}
	PointCollection::PointCollection(){}
	PointCollection::~PointCollection(){}

	glm::vec3 getPoint(float distance);

	float getMaxDistance();

	void PointCollection::createCollection(const std::string &filename)
	{
		std::vector<glm::vec3> points;
		std::ifstream in(filename, std::ios::in);
		std::string fullPath = "";

		begin.push_back(0); //TODO: make these values no longer hardcoded
		begin.push_back(1);
		begin.push_back(2);

		if (!in)
		{
			std::cout << "Cannot open " << filename << std::endl;
			assert(false);
		}

		std::string line;
		std::vector<std::vector<std::string>> tokens;
		int a = 0;
		while (std::getline(in, line))
		{
			std::vector<std::string> tempTokens = split(line, " ,");
			tokens.push_back(tempTokens);
			assert(tokens[a].size() % 2 == 0);
			a += 1;
		}

		for (int i = 0; i < tokens.size(); i++)
		{
			glm::vec3 tempLocation = { stof(tokens[i][0]), 0.0f, stof(tokens[i][1]) };
			Point* tempPoint = new Point;
			tempPoint->location = tempLocation;
			collection.push_back(tempPoint);
		}

		for (int i = 0; i < collection.size(); i++)
		{
			for (int j = 2; j < tokens[i].size(); j += 2)
			{
				glm::vec3 tempLocation = { stof(tokens[i][j]), 0.0f, stof(tokens[i][j+1]) };
				for (int k = 0; k < collection.size(); k++)
				{
					if (tempLocation.x == collection[k]->location.x && tempLocation.z == collection[k]->location.z)
					{
						collection[i]->neighbours.push_back(collection[k]);
					}
				}
			}
		}
	}
}