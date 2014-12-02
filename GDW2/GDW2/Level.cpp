#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "ThirdPersonCamera.h"
#include "Constants.h"
#include "Utilities.h"

namespace flopse
{
	Level::Level(Player* p) : Entity(new Mesh("obj/Level1.obj", new Shader("shaders/texShader.vs", "shaders/texShader.frag"))), player(p)
	{
		this->mesh->setTexture("textures/Level1.png");
		lightPos = glm::vec3(11000.f, 9000.f, 10000.f);

		particleManager = ParticleManager::getInstance();

		createPath();
		createColliders();
		createEnemies();

		initializeEntities();

		cam = new ThirdPersonCamera();
		cam->localTransform.translate(glm::vec3(-30.f, (player->mesh->getHeight()/2.f) + 30.f, -200.f));
		cam->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
		//cam->localTransform.rotate(10.f, glm::vec3(0.f, 1.f, 0.f));
		player->attach(cam);
		//ParticleSystem* s = particleManager->createParticleSystem(ParticleSystemBehaviour::Emit, 4, 1000, glm::vec3(0.f, player->mesh->getHeight(), 0.f));
		//player->attach(s);

		this->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
	}

	void Level::initializeEntities()
	{
		//Mesh *crateMesh = new Mesh(vertices3, 36, new Shader("shaders/texShader.vs", "shaders/texShader.frag"));
		//crateMesh->setTexture("textures/container.jpg");
		//Entity *crate = new Entity(crateMesh);
		//crate->scale(glm::vec3(60.f, 60.f, 60.f));
		Shader *texShader = new Shader("shaders/texShader.vs", "shaders/texShader.frag");

		Mesh *gobMesh = new Mesh("obj/TreasureGoblin.obj", texShader);
		gobMesh->setTexture("textures/TreasureGoblin.png");
		Entity *goblin = new Entity(gobMesh);
		goblin->setPosition(glm::vec3(0.f, 145.f, 0.f));

		Mesh* bossMesh = new Mesh("obj/FinalBoss.obj", texShader);
		bossMesh->setTexture("textures/BossModelTexture.png");
		Entity *boss = new Entity(bossMesh);
		boss->setPosition(glm::vec3(0.f, 145.f, -200.f));

		Mesh* p2mesh = new Mesh("obj/Player.obj", texShader);
		p2mesh->setTexture("textures/PlayerTexture.png");
		Entity *p2 = new Entity(p2mesh);
		p2->setPosition(glm::vec3(0.f, 145.f, -400.f));

		Mesh* knifeHandsMesh = new Mesh("obj/KnifeHands.obj", texShader);
		knifeHandsMesh->setTexture("textures/KnifeHands.png");
		Entity *knifeHands = new Entity(knifeHandsMesh);
		knifeHands->setPosition(glm::vec3(0.f, 145.f, -600.f));

		player->setPosition(glm::vec3(0.f, 145.f, 100.f));

		this->attach(player);
		this->attach(goblin);
		this->attach(boss);
		this->attach(p2);
		this->attach(knifeHands);
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

		path = new Path(points);
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

			this->colliders.push_back(new BoundingBox(pos, width, height, depth));
		}
	}

	void Level::createEnemies()
	{
		sf::Time spawnTime = sf::seconds(7.f);

		for (int i = 0; i < 5; i++)
		{
			spawnTime += sf::seconds(3.f);

			EnemySpawn es;
			es.enemy = Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path);
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		spawnTime += sf::seconds(7.f);

		for (int i = 0; i < 7; i++)
		{
			spawnTime += sf::seconds(2.f);

			EnemySpawn es;
			es.enemy = Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path);
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}

		spawnTime += sf::seconds(5.f);

		for (int i = 0; i < 8; i++)
		{
			spawnTime += sf::seconds(1.5f);

			EnemySpawn es;
			es.enemy = Enemy::createEnemy(EnemyType::Goblin, path->getPoint(0.f), path);
			es.spawnTime = spawnTime;
			enemySpawns.push_back(es);
		}
	}

	void Level::updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt)
	{
		if (spawnCounter < enemySpawns.size())
		{
			elapsed += dt;

			if (elapsed >= enemySpawns[spawnCounter].spawnTime)
			{
				this->attach(enemySpawns[spawnCounter].enemy);
				this->enemies.push_back(enemySpawns[spawnCounter].enemy);
				spawnCounter++;
			}
		}
	}
}