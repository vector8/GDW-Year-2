#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Constants.h"
#include "Utilities.h"
#include "Game.h"

namespace flopse
{
	Level::Level(const std::shared_ptr<Player> &p) : player(p)
	{
	}

	Level::~Level()
	{
		delete bgMusic;
	}

	void Level::createPath(const std::vector<std::string> &files)
	{
		for (int i = 0; i < files.size(); i++)
		{
			std::vector<glm::vec3> points;
			std::string filename = files[i];
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

			paths.push_back(std::make_shared<Path>(points));
		}
	}

	void Level::createColliders(const std::vector<std::string> &files)
	{
		for (int i = 0; i < files.size(); i++)
		{
			std::string filename = files[i];
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
	}

	void Level::createTowerBlockers(const std::vector<std::string> &files)
	{
		for (int i = 0; i < files.size(); i++)
		{
			std::string filename = files[i];
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
				
				int randomPath = rand() % paths.size();
				
				if (levelNumber == 3)
				{
					std::shared_ptr<Path> enemyPath = std::make_shared<Path>(createDijkstrasPath(pointGraph));
					es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(static_cast<EnemyType>(enemyType), enemyPath->getPoint(0.f), enemyPath));
				}
				else
				{
					es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(static_cast<EnemyType>(enemyType), paths[randomPath]->getPoint(0.f), paths[randomPath]));
				}

				es.spawnTime = sf::seconds(spawnTime);

				enemySpawns.push_back(es);

				enemyCount++;
			}
		}
	}

	void Level::startBackgroundMusic()
	{
		if (bgMusic != nullptr)
		{
			bgMusic->play();
		}
	}

	void Level::stopBackgroundMusic()
	{
		if (bgMusic != nullptr)
		{
			bgMusic->setPaused(true);
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
	std::vector<glm::vec3> Level::createDijkstrasPath(PointCollection graph)
	{
		//shuffles the order of the neighbours around so that it finds a random shortest path each time (since there are multiple paths of the same length)
		for (int i = 0; i < graph.collection.size(); i++)
		{
			std::random_shuffle(graph.collection[i]->neighbours.begin(), graph.collection[i]->neighbours.end());
		}
		std::random_shuffle(graph.collection.begin() + 3, graph.collection.end() - 1);

		int randomStart = rand() % graph.begin.size();
		int start = graph.begin[randomStart];			//picks a random entrace to start from
		std::vector<std::vector<Point*>> table;			//holds 
		std::vector<int> distance;						//distance of each path
		std::vector<bool> checked;						//hold whether or not the neighbors of the given point have been looked at
		bool allchecked = false;						//when the algorithm has finished
		int currentPath = 0;							//the current path in table we are looking at
		std::vector<Point*> finalPath;					//the shortest path from start to end
		
		//initialize all the vectors
		for (int i = 0; i < graph.collection.size(); i++)
		{
			checked.push_back(false);
			distance.push_back(1000000);
			std::vector<Point*> tempTableEntry;
			table.push_back(tempTableEntry);
		}
		table[0].push_back(graph.collection[start]);
		Point* tempPoint = graph.collection[start];
		distance[0] = 0;
		checked[0] = false;

		while (!allchecked)
		{
			/// Find the current path to look at ///
			int shortestDistance = INT_MAX;
			for (int i = 0; i < checked.size(); i++)
			{
				
				if (!checked[i] && distance[i] < shortestDistance)
				{
					currentPath = i;
					if (table[i].size() > 0)
					{
						tempPoint = table[i].back();
						shortestDistance = distance[i];
					}
					else
					{
						tempPoint = graph.collection[start];
						for (int k = 0; k < table[currentPath].size(); k++){}

					}
				}
			}

			/// Check all it's neighbours ///
			for (int i = 0; i < tempPoint->neighbours.size(); i++)
			{
				bool pathExists = false;
				int currentDistance = distance[currentPath] + std::abs(tempPoint->location.x - tempPoint->neighbours[i]->location.x) + std::abs(tempPoint->location.z - tempPoint->neighbours[i]->location.z);
				
				/// Check if there already exists a path to that neighbour ///
				for (int j = 0; j < table.size(); j++)
				{
					
					if (table[j].size() == 0)
					{
						break;
					}
					if (table[j].back()->location == tempPoint->neighbours[i]->location && distance[j] <= currentDistance)
					{
						pathExists = true; 
						break;
					}
					else if (table[j].back()->location == tempPoint->neighbours[i]->location && distance[j] > currentDistance) //if the new path is shorter replace the old one
					{
						if (table[j].back()->location == graph.collection.back()->location)
						{
							finalPath = table[j];
						}
						pathExists = true;
						table[j].swap(table[currentPath]);
						table[j].push_back(tempPoint->neighbours[i]);
						distance[j] = currentDistance;
						checked[j] = false;
						break;
					}

				}

				/// If a path does not exist to the current neighbour then create one ///
				if (!pathExists)
				{
					int nextEntry = 0;
					for (int j = 0; j < table.size(); j++)
					{
						if (table[j].size() == 0)
						{
							nextEntry = j;
							break;
						}
					}
					for (int k = 0; k < table[currentPath].size(); k++){
						table[nextEntry].push_back(table[currentPath][k]);
					}
					table[nextEntry].push_back(tempPoint->neighbours[i]);
					distance[nextEntry] = currentDistance;
					
					if (table[nextEntry].back()->location == graph.collection.back()->location) //if the path leads to the end, add it to the list of final paths
					{
						finalPath = table[nextEntry];
					}
				}
			}
			checked[currentPath] = true;
			for (int i = 0; i < checked.size(); i++)
			{
				allchecked = true;
				if (!checked[i])
				{
					allchecked = false;
					break;
				}
			}
		}

		/// Convert finalPath into a spline of vec3s to return ///
		int something = table[0][0]->location.x + distance[0];
		std::vector<glm::vec3> newPath;
		for (int i = 0; i < finalPath.size(); i++)
		{
			glm::vec3 newVec3;
			newVec3 = finalPath[i]->location;
			newPath.push_back(newVec3);
			if (i < finalPath.size() - 1) //adds additional points to the spline so the enemies walk in more of a straight line
			{ 
				glm::vec3 temp1 = finalPath[i]->location;
				glm::vec3 temp2 = finalPath[i + 1]->location;
				
				newVec3 = finalPath[i]->location + (finalPath[i + 1]->location - finalPath[i]->location)*0.25f;
				newPath.push_back(newVec3);
				newVec3 = finalPath[i]->location + (finalPath[i + 1]->location - finalPath[i]->location)*0.5f;
				newPath.push_back(newVec3);
				newVec3 = finalPath[i]->location + (finalPath[i + 1]->location - finalPath[i]->location)*0.75f;
				newPath.push_back(newVec3);
			}
		}
		return newPath;
	}
}