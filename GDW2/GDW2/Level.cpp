#include "Level.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm\gtc\matrix_transform.hpp>
#include "ThirdPersonCamera.h"
#include "Constants.h"
#include "Utilities.h"
#include "Game.h"
#include "ParticleSystem.h"

namespace flopse
{
	Level::Level(const std::shared_ptr<Player> &p) : Entity(std::make_shared<Mesh>("meshes/Level1.bmf", Shader::getStandardShader(StandardShaders::Phong))), player(p)
	{
		this->mesh->setDiffuseMap("textures/Level1.png");
		this->mesh->setSpecularMap("textures/Level1SpecMap.png");
		this->mesh->acceptShadow = true;

		pointLights[0].position = glm::vec3(0.f, 500.f, 0.f);
		pointLights[0].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		pointLights[0].diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
		pointLights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
		pointLights[0].constantAttenuation = 1.f;
		pointLights[0].linearAttenuation = 0.0001f;
		pointLights[0].quadraticAttenuation = 0.000001f;

		dirLight.direction = glm::vec3(-1.f, -1.f, -1.f);
		dirLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		dirLight.diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
		dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

		//fogFactor = 0.001;

		/*std::shared_ptr<ParticleSystem> fogEffect = std::make_shared<ParticleSystem>(10, 12000, "textures/Fog.png");
		fogEffect->lerpAlpha = glm::vec2(0.2f, 0.f);
		fogEffect->lerpSize = glm::vec2(0.f, 50.f);
		fogEffect->rangeLifetime = glm::vec2(8.f, 20.f);
		fogEffect->rangeVelocity = glm::vec2(0.33f, 0.4f);
		fogEffect->rangeX = glm::vec2(-2300.f, 2300.f);
		fogEffect->rangeY = glm::vec2(10.f, 15.f);
		fogEffect->rangeZ = glm::vec2(-2900.f, 3150.f);

		std::shared_ptr<ParticleSystem> sparkleEffect = std::make_shared<ParticleSystem>(1, 4, "textures/Sparkle.png");
		sparkleEffect->lerpAlpha = glm::vec2(0.75f, 0.f);
		sparkleEffect->lerpSize = glm::vec2(0.f, 10.f);
		sparkleEffect->rangeLifetime = glm::vec2(1.f, 1.5f);
		sparkleEffect->rangeVelocity = glm::vec2(0.25f, 0.33f);
		sparkleEffect->rangeX = glm::vec2(-15.f, 15.f);
		sparkleEffect->rangeY = glm::vec2(147.f, 157.f);
		sparkleEffect->rangeZ = glm::vec2(-15.f, 15.f);

		attach(fogEffect);
		attach(sparkleEffect);*/

		createPath();
		createColliders();
		createEnemies();

		initializeEntities();

		cam = std::make_shared<ThirdPersonCamera>();
		cam->localTransform.translate(glm::vec3(-30.f, (player->mesh->getHeight() / 2.f) + 30.f, -200.f));
		cam->projection = glm::perspective(Game::getGame()->getFieldOfView(), 1024.0f / 768.0f, 0.1f, 100000.0f);
		//cam->projection = glm::ortho(-512.0f, 512.0f, -384.0f, 384.0f, -1000.f, 1000.f);
		//cam->projection = glm::ortho(-350.f, 350.f, -350.f, 350.f, -10.f, 10000.f);
		player->attach(cam);
		//ParticleSystem* s = particleManager->createParticleSystem(ParticleSystemBehaviour::Emit, 4, 1000, glm::vec3(0.f, player->mesh->getHeight(), 0.f));
		//player->attach(s);

		shadowCamera = std::make_shared<Camera>();
		shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
		shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
		//shadowCamera->localTransform.translate(glm::vec3(1000.f, 1000.f, 1000.f));
		shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
		shadowCamera->globalTransform = shadowCamera->localTransform.getTransformMatrix();
		shadowCamera->recalculateView();

		this->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
	}

	Level::~Level()
	{
	}

	void Level::initializeEntities()
	{
		player->setPosition(glm::vec3(0.f, 145.f, 100.f));

		this->attach(player);
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