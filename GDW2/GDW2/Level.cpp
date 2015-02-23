#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "ThirdPersonCamera.h"
#include "Constants.h"
#include "Utilities.h"

namespace flopse
{
	Level::Level(const std::shared_ptr<Player> &p) : Entity(std::make_shared<Mesh>("meshes/Level1.bmf", new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag"))), player(p)
	{
		this->mesh->setTexture("textures/Level1.png");
		
		light.ambientComponent = 0.2f;
		light.diffuseComponent = 0.7f;
		light.specularComponent = 0.3f;
		light.specularExponent = 32.f;
		light.constantAttenuation = 1.f;
		light.linearAttenuation = 0.0001f;
		light.quadraticAttenuation = 0.000001f;
		light.position = glm::vec3(0.f, 500.f, 0.f);

		particleManager = ParticleManager::getInstance();

		createPath();
		createColliders();
		createEnemies();

		initializeEntities();

		cam = std::make_shared<ThirdPersonCamera>();
		cam->localTransform.translate(glm::vec3(-30.f, (player->mesh->getHeight() / 2.f) + 30.f, -200.f));
		//cam->localTransform.translate(glm::vec3(-30.f, (player->mesh->getHeight()/2.f) + 30.f, -200.f));
		//cam->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
		//cam->localTransform.rotate(10.f, glm::vec3(0.f, 1.f, 0.f));
		player->attach(cam);
		//ParticleSystem* s = particleManager->createParticleSystem(ParticleSystemBehaviour::Emit, 4, 1000, glm::vec3(0.f, player->mesh->getHeight(), 0.f));
		//player->attach(s);

		this->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
	}

	Level::~Level()
	{
	}

	void Level::initializeEntities()
	{
		//std::shared_ptr<Mesh> crateMesh = new Mesh(vertices3, 36, new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag"));
		//crateMesh->setTexture("textures/container.jpg");
		//Entity *crate = new Entity(crateMesh);
		//crate->scale(glm::vec3(60.f, 60.f, 60.f));
		/*Shader *texShader = new Shader("shaders/StaticGeometry.vert", "shaders/Phong.frag");

		std::shared_ptr<Mesh> gobMesh = new Mesh("meshes/TreasureGoblin.bmf", texShader);
		gobMesh->setTexture("textures/TreasureGoblin.png");
		Entity *goblin = new Entity(gobMesh);
		goblin->setPosition(glm::vec3(0.f, 145.f, 0.f));

		std::shared_ptr<Mesh> bossMesh = new Mesh("meshes/FinalBoss.bmf", texShader);
		bossMesh->setTexture("textures/BossModelTexture.png");
		Entity *boss = new Entity(bossMesh);
		boss->setPosition(glm::vec3(0.f, 145.f, -200.f));

		std::shared_ptr<Mesh> p2mesh = new Mesh("meshes/Player.bmf", texShader);
		p2mesh->setTexture("textures/PlayerTexture.png");
		Entity *p2 = new Entity(p2mesh);
		p2->setPosition(glm::vec3(0.f, 145.f, -400.f));

		std::shared_ptr<Mesh> knifeHandsMesh = new Mesh("meshes/KnifeHands.bmf", texShader);
		knifeHandsMesh->setTexture("textures/KnifeHands.png");
		Entity *knifeHands = new Entity(knifeHandsMesh);
		knifeHands->setPosition(glm::vec3(0.f, 145.f, -600.f));*/

		player->setPosition(glm::vec3(0.f, 145.f, 100.f));

		this->attach(player);
		/*this->attach(goblin);
		this->attach(boss);
		this->attach(p2);
		this->attach(knifeHands);*/
	}

	void Level::createPath()
	{
		std::vector<glm::vec3> points;
		std::ifstream in("levels/Level1Path.txt", std::ios::in);
		std::string fullPath = "";

		if (!in)
		{
			std::cout << "Cannot open " << "levels/Level1Path.txt" << std::endl;
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

	void Level::createColliders()
	{
		std::ifstream in("levels/Level1Colliders.txt", std::ios::in);

		if (!in)
		{
			std::cout << "Cannot open " << "levels/Level1Colliders.txt" << std::endl;
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

	void Level::createEnemies()
	{
		sf::Time spawnTime = sf::seconds(7.f);

		for (int i = 0; i < 5; i++)
		{
			spawnTime += sf::seconds(3.f);

			EnemySpawn es;
			es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path));
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		spawnTime += sf::seconds(7.f);

		for (int i = 0; i < 7; i++)
		{
			spawnTime += sf::seconds(1.8f);

			EnemySpawn es;
			es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path));
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		spawnTime += sf::seconds(5.f);

		for (int i = 0; i < 8; i++)
		{
			spawnTime += sf::seconds(1.2f);

			EnemySpawn es;
			es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path));
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		spawnTime += sf::seconds(5.f);

		for (int i = 0; i < 12; i++)
		{
			spawnTime += sf::seconds(0.8f);

			EnemySpawn es;
			es.enemy = std::make_shared<Enemy>(Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path));
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		enemyCount = 32;
	}

	void Level::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		/*if (enemySpawns.size() > 0)
		{
			elapsed += dt;

			if (elapsed >= enemySpawns.front().spawnTime)
			{
				this->attach(enemySpawns.front().enemy);
				this->enemies.push_back(enemySpawns.front().enemy);
				enemySpawns.pop_front();
			}
		}*/

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