#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Constants.h"
#include "Utilities.h"

namespace flopse
{
	Level::Level(const std::shared_ptr<Player> &p) : player(p)
	{
	}

	Level::~Level()
	{
	}

	void Level::createPath(const std::string &filename)
	{
		std::vector<glm::vec3> points;
		std::ifstream in(filename, std::ios::in);
		std::string fullPath = "";

		if (!in)
		{
			std::cout << "Cannot open " << filename << std::endl;
			assert(false);
		}

		std::string line;
		while (std::getline(in, line))
		{
			fullPath.append(line);
		}

		std::vector<std::string> tokens = split(fullPath, " ,");
		assert(tokens.size() % 3 == 0);

		glm::vec3 currPoint;
		std::istringstream ss;
		for (int i = 0; i < tokens.size(); i += 3)
		{
			ss.str(tokens[i]);
			ss.clear();
			assert(ss >> currPoint.x);

			ss.str(tokens[i + 1]);
			ss.clear();
			assert(ss >> currPoint.y);

			ss.str(tokens[i + 2]);
			ss.clear();
			assert(ss >> currPoint.z);

			points.push_back(currPoint);
		}

		path = std::make_shared<Path>(points);
	}

	void Level::createColliders(const std::string &filename)
	{
		std::ifstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Cannot open " << filename << std::endl;
			assert(false);
		}

		std::string line;
		std::istringstream ss;
		while (std::getline(in, line))
		{
			std::vector<std::string> tokens = split(line, " ,");
			assert(tokens.size() == 6);

			glm::vec3 p1, p2;

			ss.str(tokens[0]);
			ss.clear();
			assert(ss >> p1.x);
			ss.str(tokens[1]);
			ss.clear();
			assert(ss >> p1.y);
			ss.str(tokens[2]);
			ss.clear();
			assert(ss >> p1.z);

			ss.str(tokens[3]);
			ss.clear();
			assert(ss >> p2.x);
			ss.str(tokens[4]);
			ss.clear();
			assert(ss >> p2.y);
			ss.str(tokens[5]);
			ss.clear();
			assert(ss >> p2.z);

			glm::vec3 pos;
			float width, height, depth;

			pos.x = (p1.x + p2.x) / 2.f;
			pos.y = p1.y;
			pos.z = (p1.z + p2.z) / 2.f;

			width = abs(p2.x - p1.x);
			height = abs(p2.y - p1.y);
			depth = abs(p2.z - p1.z);

			this->colliders.push_back(BoundingBox(pos, width, height, depth));
		}
	}

	void Level::createTowerBlockers(const std::string &filename)
	{
		std::ifstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Cannot open " << filename << std::endl;
			assert(false);
		}

		std::string line;
		std::istringstream ss;
		while (std::getline(in, line))
		{
			std::vector<std::string> tokens = split(line, " ,");
			assert(tokens.size() == 6);

			glm::vec3 p1, p2;

			ss.str(tokens[0]);
			ss.clear();
			assert(ss >> p1.x);
			ss.str(tokens[1]);
			ss.clear();
			assert(ss >> p1.y);
			ss.str(tokens[2]);
			ss.clear();
			assert(ss >> p1.z);

			ss.str(tokens[3]);
			ss.clear();
			assert(ss >> p2.x);
			ss.str(tokens[4]);
			ss.clear();
			assert(ss >> p2.y);
			ss.str(tokens[5]);
			ss.clear();
			assert(ss >> p2.z);

			glm::vec3 pos;
			float width, height, depth;

			pos.x = (p1.x + p2.x) / 2.f;
			pos.y = p1.y;
			pos.z = (p1.z + p2.z) / 2.f;

			width = abs(p2.x - p1.x);
			height = abs(p2.y - p1.y);
			depth = abs(p2.z - p1.z);

			this->towerBlockers.push_back(BoundingBox(pos, width, height, depth));
		}
	}

	void Level::createEnemies(const std::string &filename)
	{
		enemyCount = 0;

		std::ifstream in(filename, std::ios::in);

		if (!in)
		{
			std::cout << "Cannot open " << filename << std::endl;
			assert(false);
		}

		std::string line;
		std::istringstream ss;
		while (std::getline(in, line))
		{
			if (line.size() > 0)
			{
				EnemySpawn es;
				int enemyType;
				float spawnTime;

				std::vector<std::string> tokens = split(line, " ");
				assert(tokens.size() == 2);

				ss.str(tokens[0]);
				ss.clear();
				assert(ss >> enemyType);

				ss.str(tokens[1]);
				ss.clear();
				assert(ss >> spawnTime);

				es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(static_cast<EnemyType>(enemyType), path->getPoint(0.f), path));
				es.spawnTime = sf::seconds(spawnTime);

				enemySpawns.push_back(es);

				enemyCount++;
			}
		}
	}

	void Level::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		if (enemySpawns.size() > 0)
		{
			elapsed += dt;

			if (elapsed >= enemySpawns.front().spawnTime)
			{
				this->attach(enemySpawns.front().enemy);
				this->enemies.push_back(enemySpawns.front().enemy);
				enemySpawns.pop_front();
			}
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->toBeDeleted)
			{
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
	}
}