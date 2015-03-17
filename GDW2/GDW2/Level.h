#pragma once
#include <memory>
#include <list>
#include <glm\gtc\matrix_transform.hpp>

#include "Entity.h"
#include "Player.h"
#include "Path.h"
#include "Enemy.h"
#include "Light.h"
#include "ThirdPersonCamera.h"
#include "Sound.h"

namespace flopse
{
	struct EnemySpawn
	{
		std::shared_ptr<Enemy> enemy = nullptr;
		sf::Time spawnTime;
	};

	class Level : public Entity
	{
	private:
		sf::Time elapsed;

		Sound *bgMusic = nullptr;

		void createPath(const std::string &filename);
		void createColliders(const std::string &filename);
		void createEnemies(const std::string &filename);
		void createTowerBlockers(const std::string &filename);

	public:
		Level(const std::shared_ptr<Player> &p);
		virtual ~Level();

		static std::shared_ptr<Level> createLevel(int levelNumber, const std::shared_ptr<Player> &p)
		{
			std::shared_ptr<Level> lvl = std::make_shared<Level>(p);

			//levelNumber = 4;
			switch (levelNumber)
			{
			case 1:

			{
				lvl->bgMusic = new Sound("sounds/Level Music/Nothing Broken.wav", true);

				//lvl 1
				lvl->mesh = std::make_shared<Mesh>("meshes/Ground.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level1GroundTexture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> fencesMesh = std::make_shared<Mesh>("meshes/Fences.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> fences = std::make_shared<Entity>(fencesMesh);
				fences->mesh->setDiffuseMap("textures/fenceTexture.png");
				fences->mesh->setSpecularMap("textures/BlankSpecular.png");
				fences->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> treesMesh = std::make_shared<Mesh>("meshes/Trees.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> trees = std::make_shared<Entity>(treesMesh);
				trees->mesh->setDiffuseMap("textures/treeTexture.png");
				trees->mesh->setSpecularMap("textures/BlankSpecular.png");
				trees->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> castleMesh = std::make_shared<Mesh>("meshes/Castle.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> castle = std::make_shared<Entity>(castleMesh);
				castle->mesh->setDiffuseMap("textures/castleTexture.png");
				castle->mesh->setSpecularMap("textures/BlankSpecular.png");
				castle->mesh->acceptShadow = true;

				lvl->pointLights[0] = std::make_shared<Light>();
				lvl->pointLights[0]->localTransform.translate(glm::vec3(0.f, 500.f, 0.f));
				lvl->pointLights[0]->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->pointLights[0]->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->pointLights[0]->specular = glm::vec3(1.0f, 1.0f, 1.0f);
				lvl->pointLights[0]->constantAttenuation = 1.f;
				lvl->pointLights[0]->linearAttenuation = 0.0001f;
				lvl->pointLights[0]->quadraticAttenuation = 0.000001f;

				lvl->dirLight = std::make_shared<Light>();
				lvl->dirLight->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->dirLight->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->dirLight->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->dirLight->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);


				lvl->attach(lvl->pointLights[0]);
				lvl->attach(lvl->dirLight);
				lvl->attach(trees);
				lvl->attach(castle);
				lvl->attach(fences);
				//lvl->fogFactor = 0.001;

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

				lvl->createPath("levels/Level1Path.txt");
				lvl->createColliders("levels/Level1Colliders.txt");
				lvl->createTowerBlockers("levels/Level1Colliders.txt");
				lvl->createEnemies("levels/Level1Enemies.txt");

				p->setPosition(glm::vec3(0.f, 145.f, 100.f));
				lvl->attach(p);

				//ParticleSystem* s = particleManager->createParticleSystem(ParticleSystemBehaviour::Emit, 4, 1000, glm::vec3(0.f, player->mesh->getHeight(), 0.f));
				//player->attach(s);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				//lvl->shadowCamera->localTransform.translate(glm::vec3(1000.f, 1000.f, 1000.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
			}
				break;
			case 2:
			{
				//lvl 2
				lvl->bgMusic = new Sound("sounds/Level Music/Music for Manatees.wav", true);

				lvl->mesh = std::make_shared<Mesh>("meshes/level 4.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level4Texture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> castleMesh = std::make_shared<Mesh>("meshes/level 4 castle.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> castle = std::make_shared<Entity>(castleMesh);
				castle->mesh->setDiffuseMap("textures/castleTexture.png");
				castle->mesh->setSpecularMap("textures/BlankSpecular.png");
				castle->mesh->acceptShadow = true;


				lvl->pointLights[0] = std::make_shared<Light>();
				lvl->pointLights[0]->localTransform.translate(glm::vec3(0.f, 500.f, 0.f));
				lvl->pointLights[0]->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->pointLights[0]->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->pointLights[0]->specular = glm::vec3(1.0f, 1.0f, 1.0f);
				lvl->pointLights[0]->constantAttenuation = 1.f;
				lvl->pointLights[0]->linearAttenuation = 0.0001f;
				lvl->pointLights[0]->quadraticAttenuation = 0.000001f;

				lvl->dirLight = std::make_shared<Light>();
				lvl->dirLight->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->dirLight->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->dirLight->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->dirLight->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);


				lvl->attach(lvl->pointLights[0]);
				lvl->attach(lvl->dirLight);
				lvl->attach(castle);

				lvl->createPath("levels/Level1Path.txt");
				lvl->createColliders("levels/Level1Colliders.txt");
				lvl->createTowerBlockers("levels/Level1Colliders.txt");
				lvl->createEnemies("levels/Leve4Enemies.txt");

				p->setPosition(glm::vec3(0.f, 145.f, 100.f));
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				//lvl->shadowCamera->localTransform.translate(glm::vec3(1000.f, 1000.f, 1000.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);

			}
			break;
			case 3:
			{
				lvl->bgMusic = new Sound("sounds/Level Music/Oppressive Gloom.wav", true);

				lvl->mesh = std::make_shared<Mesh>("meshes/LEVEL5.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level5Texture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				lvl->pointLights[0] = std::make_shared<Light>();
				lvl->pointLights[0]->localTransform.translate(glm::vec3(0.f, 500.f, 0.f));
				lvl->pointLights[0]->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->pointLights[0]->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->pointLights[0]->specular = glm::vec3(1.0f, 1.0f, 1.0f);
				lvl->pointLights[0]->constantAttenuation = 1.f;
				lvl->pointLights[0]->linearAttenuation = 0.0001f;
				lvl->pointLights[0]->quadraticAttenuation = 0.000001f;

				lvl->dirLight = std::make_shared<Light>();
				lvl->dirLight->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->dirLight->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->dirLight->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->dirLight->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
				lvl->dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);


				lvl->attach(lvl->pointLights[0]);
				lvl->attach(lvl->dirLight);

				lvl->createPath("levels/Level1Path.txt");
				lvl->createColliders("levels/Level1Colliders.txt");
				lvl->createTowerBlockers("levels/Level1Colliders.txt");
				lvl->createEnemies("levels/Level5Enemies.txt");

				p->setPosition(glm::vec3(0.f, 145.f, 100.f));
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				//lvl->shadowCamera->localTransform.translate(glm::vec3(1000.f, 1000.f, 1000.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
				lvl->startBackgroundMusic();

				//lvl 3

			}
			break;
			case 4:
			{

				lvl->mesh = std::make_shared<Mesh>("meshes/level1.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/Level1.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				lvl->pointLights[0] = std::make_shared<Light>();
				lvl->pointLights[0]->localTransform.translate(glm::vec3(0.f, 500.f, 0.f));
				lvl->pointLights[0]->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
				lvl->pointLights[0]->diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
				lvl->pointLights[0]->specular = glm::vec3(1.0f, 1.0f, 1.0f);
				lvl->pointLights[0]->constantAttenuation = 1.f;
				lvl->pointLights[0]->linearAttenuation = 0.0001f;
				lvl->pointLights[0]->quadraticAttenuation = 0.000001f;

				lvl->dirLight = std::make_shared<Light>();
				lvl->dirLight->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->dirLight->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->dirLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
				lvl->dirLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
				lvl->dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);


				lvl->attach(lvl->pointLights[0]);
				lvl->attach(lvl->dirLight);

				lvl->createPath("levels/Level1PathOld.txt");
				lvl->createColliders("levels/Level1CollidersOld.txt");
				lvl->createTowerBlockers("levels/Level1CollidersOld.txt");
				lvl->createEnemies("levels/Level1Enemies.txt");

				p->setPosition(glm::vec3(0.f, 148.f, 100.f));
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				//lvl->shadowCamera->localTransform.translate(glm::vec3(1000.f, 1000.f, 1000.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->mesh->overlayColour = Colour(0.2f, 0.2f, 0.2f, 1.f);
			}
				break;
			case 5:
				//lvl 5
				break;
			default:
				break;
			}

			return lvl;
		}

		std::shared_ptr<Light> dirLight;
		std::shared_ptr<Light> pointLights[NUM_POINT_LIGHTS];
		std::shared_ptr<Player> player = nullptr;
		std::shared_ptr<Camera> shadowCamera = nullptr;
		float fogFactor = 0.0f;

		std::shared_ptr<Path> path = nullptr;

		void startBackgroundMusic();
		void stopBackgroundMusic();

		void killEnemy();

		int gateHealth = 200;
		int maxGateHealth = 200;
		int enemyCount = 0;

		std::vector<BoundingBox> colliders;
		std::vector<BoundingBox> towerBlockers;
		std::vector<std::shared_ptr<Enemy>> enemies;
		std::list<EnemySpawn> enemySpawns;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}