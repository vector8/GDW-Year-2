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
#include "PointCollection.h"

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

		void createPath(const std::vector<std::string> &files);
		void createColliders(const std::vector<std::string> &files);
		void createEnemies(const std::string &filename);
		void createTowerBlockers(const std::vector<std::string> &files);

		std::vector<glm::vec3> createDijkstrasPath(PointCollection collection);

	public:
		Level(const std::shared_ptr<Player> &p);
		virtual ~Level();

		static std::shared_ptr<Level> createLevel(int levelNumber, const std::shared_ptr<Player> &p)
		{
			std::shared_ptr<Level> lvl = std::make_shared<Level>(p);

			lvl->levelNumber = levelNumber;
			switch (levelNumber)
			{
			case 1:

			{
				lvl->bgMusic = new Sound("sounds/Level Music/Nothing Broken.wav", true, false);

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

				std::shared_ptr<Mesh> skyboxMesh = std::make_shared<Mesh>("meshes/Skybox.bmf", Shader::getStandardShader(StandardShaders::AmbientTexture));
				std::shared_ptr<Entity> skybox = std::make_shared<Entity>(skyboxMesh);
				skybox->mesh->setDiffuseMap("textures/Skybox.png");
				skybox->mesh->setSpecularMap("textures/BlankSpecular.png");
				skybox->mesh->acceptShadow = false;
				skybox->mesh->castShadow = false;

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
				lvl->dirLight->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
				lvl->dirLight->diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
				lvl->dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);

				lvl->attach(lvl->pointLights[0]);
				lvl->attach(lvl->dirLight);
				lvl->attach(trees);
				lvl->attach(castle);
				lvl->attach(fences);
				lvl->attach(skybox);

				std::vector<std::string> pathFiles;
				pathFiles.push_back("levels/Level1Path.txt");
				lvl->createPath(pathFiles);
				std::vector<std::string> colliders;
				colliders.push_back("levels/Level1Colliders.txt");
				lvl->createColliders(colliders);
				std::vector<std::string> towerBlockers;
				towerBlockers.push_back("levels/Level1Colliders.txt");
				towerBlockers.push_back("levels/Level1Colliders(Path).txt");
				lvl->createTowerBlockers(towerBlockers);
				lvl->createEnemies("levels/Level1Enemies.txt");

				p->setPosition(glm::vec3(0.f, 145.f, 100.f));
				p->gold = 100;
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->minimapCamera = std::make_shared<Camera>();
				lvl->minimapCamera->localTransform.rotate(90.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->minimapCamera->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
				float lvlWidth = lvl->mesh->getWidth() / 2.f, lvlDepth = lvl->mesh->getDepth() / 2.f;
				lvl->minimapCamera->projection = glm::ortho(-lvlWidth, lvlWidth, -lvlDepth, lvlDepth, -castleMesh->getHeight(), castleMesh->getHeight());
				lvl->attach(lvl->minimapCamera);
			}
				break;
			case 2:
			{
				//lvl 2
				lvl->bgMusic = new Sound("sounds/Level Music/Anguish.wav", true, false);

				lvl->mesh = std::make_shared<Mesh>("meshes/Level 2/Ground.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level2Ground.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> castleMesh = std::make_shared<Mesh>("meshes/Level 2/Castle.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> castle = std::make_shared<Entity>(castleMesh);
				castle->mesh->setDiffuseMap("textures/castleTexture.png");
				castle->mesh->setSpecularMap("textures/BlankSpecular.png");
				castle->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> fenceMesh = std::make_shared<Mesh>("meshes/Level 2/fences.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> fence = std::make_shared<Entity>(fenceMesh);
				fence->mesh->setDiffuseMap("textures/fenceTexture.png");
				fence->mesh->setSpecularMap("textures/BlankSpecular.png");
				fence->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> smallTreesMesh = std::make_shared<Mesh>("meshes/Level 2/smallTrees.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> smallTrees = std::make_shared<Entity>(smallTreesMesh);
				smallTrees->mesh->setDiffuseMap("textures/treeTexture.png");
				smallTrees->mesh->setSpecularMap("textures/BlankSpecular.png");
				smallTrees->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> largeTreesMesh = std::make_shared<Mesh>("meshes/Level 2/largeTrees.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> largeTrees = std::make_shared<Entity>(largeTreesMesh);
				largeTrees->mesh->setDiffuseMap("textures/largeTreeTexture.png");
				largeTrees->mesh->setSpecularMap("textures/BlankSpecular.png");
				largeTrees->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> skyboxMesh = std::make_shared<Mesh>("meshes/Skybox.bmf", Shader::getStandardShader(StandardShaders::AmbientTexture));
				std::shared_ptr<Entity> skybox = std::make_shared<Entity>(skyboxMesh);
				skybox->mesh->setDiffuseMap("textures/L2Skybox.png");
				skybox->mesh->setSpecularMap("textures/BlankSpecular.png");
				skybox->mesh->acceptShadow = false;
				skybox->mesh->castShadow = false;

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
				lvl->attach(fence);
				lvl->attach(smallTrees);
				lvl->attach(largeTrees);
				lvl->attach(skybox);

				std::vector<std::string> pathFiles;
				pathFiles.push_back("levels/Level2Path.txt");
				lvl->createPath(pathFiles);
				std::vector<std::string> colliders;
				colliders.push_back("levels/Level2Colliders.txt");
				lvl->createColliders(colliders);
				std::vector<std::string> towerBlockers;
				towerBlockers.push_back("levels/Level2Colliders.txt");
				towerBlockers.push_back("levels/Level2Colliders(Path).txt");
				lvl->createTowerBlockers(towerBlockers);
				lvl->createEnemies("levels/Level2Enemies.txt");

				p->setPosition(glm::vec3(0.f, 145.f, 100.f));
				p->gold = 150;
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->minimapCamera = std::make_shared<Camera>();
				lvl->minimapCamera->localTransform.rotate(90.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->minimapCamera->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
				float lvlWidth = lvl->mesh->getWidth() / 2.f, lvlDepth = lvl->mesh->getDepth() / 2.f;
				lvl->minimapCamera->projection = glm::ortho(-lvlWidth, lvlWidth, -lvlDepth, lvlDepth, -castleMesh->getHeight(), castleMesh->getHeight());
				lvl->attach(lvl->minimapCamera);
			}
			break;
			case 3:
			{
				lvl->bgMusic = new Sound("sounds/Level Music/Stoneworld Battle.wav", true, false);

				lvl->pointGraph.createCollection("levels/Level3Points.txt");

				lvl->mesh = std::make_shared<Mesh>("meshes/Level3.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level3Texture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> castleMesh = std::make_shared<Mesh>("meshes/Castle3.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> castle = std::make_shared<Entity>(castleMesh);
				castle->mesh->setDiffuseMap("textures/castleTexture.png");
				castle->mesh->setSpecularMap("textures/BlankSpecular.png");
				castle->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> skyboxMesh = std::make_shared<Mesh>("meshes/Skybox.bmf", Shader::getStandardShader(StandardShaders::AmbientTexture));
				std::shared_ptr<Entity> skybox = std::make_shared<Entity>(skyboxMesh);
				skybox->mesh->setDiffuseMap("textures/L3Skybox.png");
				skybox->mesh->setSpecularMap("textures/BlankSpecular.png");
				skybox->mesh->acceptShadow = false;
				skybox->mesh->castShadow = false;

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
				lvl->attach(skybox);

				std::vector<std::string> pathFiles;
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path1Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path2Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path3Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path4Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path5Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path6Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path7Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path8Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path9Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path10Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path11Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path12Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path13Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path14Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path15Test.txt");
				pathFiles.push_back("levels/Level3Paths/Test/Level3Path16Test.txt");
				lvl->createPath(pathFiles);
				std::vector<std::string> colliders;
				colliders.push_back("levels/Level3Colliders.txt");
				lvl->createColliders(colliders);
				std::vector<std::string> towerBlockers;
				towerBlockers.push_back("levels/Level3Colliders.txt");
				towerBlockers.push_back("levels/Level3Colliders(Path).txt");
				lvl->createTowerBlockers(towerBlockers);
				lvl->createEnemies("levels/Level3Enemies.txt");

				lvl->groundLevel = 100.0;

				p->setPosition(glm::vec3(0.f, 300.f, 100.f));
				p->gold = 200;
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->minimapCamera = std::make_shared<Camera>();
				lvl->minimapCamera->localTransform.rotate(90.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->minimapCamera->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
				float lvlWidth = lvl->mesh->getWidth() / 2.f, lvlDepth = lvl->mesh->getDepth() / 2.f;
				lvl->minimapCamera->projection = glm::ortho(-lvlWidth, lvlWidth, -lvlDepth, lvlDepth, -castleMesh->getHeight(), castleMesh->getHeight());
				lvl->attach(lvl->minimapCamera);
			}
			break;
			case 4:
			{
				lvl->bgMusic = new Sound("sounds/Level Music/Music for Manatees.wav", true, false);

				lvl->mesh = std::make_shared<Mesh>("meshes/Level4.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level4Texture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> castleMesh = std::make_shared<Mesh>("meshes/level 4 castle.bmf", Shader::getStandardShader(StandardShaders::Phong));
				std::shared_ptr<Entity> castle = std::make_shared<Entity>(castleMesh);
				castle->mesh->setDiffuseMap("textures/castleTexture.png");
				castle->mesh->setSpecularMap("textures/BlankSpecular.png");
				castle->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> skyboxMesh = std::make_shared<Mesh>("meshes/Skybox.bmf", Shader::getStandardShader(StandardShaders::AmbientTexture));
				std::shared_ptr<Entity> skybox = std::make_shared<Entity>(skyboxMesh);
				skybox->mesh->setDiffuseMap("textures/L4Skybox.png");
				skybox->mesh->setSpecularMap("textures/BlankSpecular.png");
				skybox->mesh->acceptShadow = false;
				skybox->mesh->castShadow = false;

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
				lvl->attach(skybox);
				lvl->attach(castle);

				std::vector<std::string> pathFiles;
				pathFiles.push_back("levels/Level4Path1Test.txt");
				pathFiles.push_back("levels/Level4Path2Test.txt");
				lvl->createPath(pathFiles);
				std::vector<std::string> colliders;
				colliders.push_back("levels/Level4Colliders.txt");
				lvl->createColliders(colliders);
				std::vector<std::string> towerBlockers;
				towerBlockers.push_back("levels/Level3Colliders.txt");
				lvl->createTowerBlockers(towerBlockers);
				lvl->createEnemies("levels/Level4Enemies.txt");

				lvl->groundLevel = 155.0;

				p->setPosition(glm::vec3(0.f, 155.f, -2500.f));
				p->gold = 400;
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->minimapCamera = std::make_shared<Camera>();
				lvl->minimapCamera->localTransform.rotate(90.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->minimapCamera->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
				float lvlWidth = lvl->mesh->getWidth() / 2.f, lvlDepth = lvl->mesh->getDepth() / 2.f;
				lvl->minimapCamera->projection = glm::ortho(-lvlWidth, lvlWidth, -lvlDepth, lvlDepth, -castleMesh->getHeight(), castleMesh->getHeight());
				lvl->attach(lvl->minimapCamera);

				lvl->startBackgroundMusic();
			}
				break;
			case 5:
			{
				//lvl 5
				lvl->bgMusic = new Sound("sounds/Level Music/Oppressive Gloom.wav", true, false);

				lvl->mesh = std::make_shared<Mesh>("meshes/LEVEL5.bmf", Shader::getStandardShader(StandardShaders::Phong));
				lvl->mesh->setDiffuseMap("textures/level5Texture.png");
				lvl->mesh->setSpecularMap("textures/BlankSpecular.png");
				lvl->mesh->acceptShadow = true;

				std::shared_ptr<Mesh> skyboxMesh = std::make_shared<Mesh>("meshes/Skybox.bmf", Shader::getStandardShader(StandardShaders::AmbientTexture));
				std::shared_ptr<Entity> skybox = std::make_shared<Entity>(skyboxMesh);
				skybox->mesh->setDiffuseMap("textures/L5Skybox.png");
				skybox->mesh->setSpecularMap("textures/BlankSpecular.png");
				skybox->mesh->acceptShadow = false;
				skybox->mesh->castShadow = false;

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
				lvl->attach(skybox);

				std::vector<std::string> pathFiles;
				pathFiles.push_back("levels/Level5Path1Test.txt");
				pathFiles.push_back("levels/Level5Path2Test.txt");
				pathFiles.push_back("levels/Level5Path3Test.txt");
				lvl->createPath(pathFiles);
				std::vector<std::string> colliders;
				colliders.push_back("levels/Level5Colliders.txt");
				lvl->createColliders(colliders);
				std::vector<std::string> towerBlockers;
				towerBlockers.push_back("levels/Level5Colliders.txt");
				lvl->createTowerBlockers(towerBlockers);
				lvl->createEnemies("levels/Level5Enemies.txt");

				p->setPosition(glm::vec3(0.f, 100.f, 0.f));
				p->gold = 600;
				lvl->attach(p);

				lvl->shadowCamera = std::make_shared<Camera>();
				lvl->shadowCamera->localTransform.rotate(45.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->shadowCamera->localTransform.rotate(225.f, glm::vec3(0.f, 1.f, 0.f));
				lvl->shadowCamera->projection = glm::ortho(-4500.f, 4500.f, -4500.f, 4500.f, -3300.0f, 4600.f);
				lvl->shadowCamera->globalTransform = lvl->shadowCamera->localTransform.getTransformMatrix();
				lvl->shadowCamera->recalculateView();

				lvl->minimapCamera = std::make_shared<Camera>();
				lvl->minimapCamera->localTransform.rotate(90.f, glm::vec3(1.f, 0.f, 0.f));
				lvl->minimapCamera->localTransform.rotate(180.f, glm::vec3(0.f, 1.f, 0.f));
				float lvlWidth = lvl->mesh->getWidth() / 2.f, lvlDepth = lvl->mesh->getDepth() / 2.f, lvlHeight = lvl->mesh->getHeight() / 2.f;
				lvl->minimapCamera->projection = glm::ortho(-lvlWidth, lvlWidth, -lvlDepth, lvlDepth, -lvlHeight, lvlHeight);
				lvl->attach(lvl->minimapCamera);

				lvl->startBackgroundMusic();
			}
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
		std::shared_ptr<Camera> minimapCamera = nullptr;
		float fogFactor = 0.0f;

		std::vector<std::shared_ptr<Path>> paths;

		void startBackgroundMusic();
		void stopBackgroundMusic();

		void killEnemy();

		int gateHealth = 200;
		int maxGateHealth = 200;
		int enemyCount = 0;
		float groundLevel = 0.0;
		
		int levelNumber = 0;

		PointCollection pointGraph;

		std::vector<BoundingBox> colliders;
		std::vector<BoundingBox> towerBlockers;
		std::vector<std::shared_ptr<Enemy>> enemies;
		std::list<EnemySpawn> enemySpawns;

		virtual void updateLocalTransform(const sf::RenderWindow &window, const sf::Time &dt);
	};
}