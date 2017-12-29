#include "ModuleEnemy.h"

#include "Enemy.h"
#include "Obstacle.h"
#include "Ovni.h"
#include "ShieldedOvniBrain.h"
#include "ShieldedOvni.h"
#include "Explosion.h"

#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "SFX.h"

#include <fstream>
#include <assert.h>

#include "json.hpp"
#include "json_serializer.h"

ModuleEnemy::ModuleEnemy(bool enabled) :
	Module(enabled)
{
	prototypeCreationFunctionMap["ovni"] = &ModuleEnemy::CreateOvniPrototype;
	prototypeCreationFunctionMap["shieldedOvniBrain"] = &ModuleEnemy::CreateShieldedOvniBrainPrototype;
	prototypeCreationFunctionMap["shieldedOvni"] = &ModuleEnemy::CreateShieldedOvni;
	prototypeCreationFunctionMap["obstacle"] = &ModuleEnemy::CreateObstaclePrototype;
	prototypeCreationFunctionMap["explosion"] = &ModuleEnemy::CreateExplosion;
}

ModuleEnemy::~ModuleEnemy()
{
}

bool ModuleEnemy::Start()
{
	ifstream enemyPrototypeJsonFile("data/enemyPrototypes.json");
	json enemyPrototypesData;
	enemyPrototypeJsonFile >> enemyPrototypesData;

	for (auto prototype : enemyPrototypesData["prototypes"]) {
		enemyPrototypes[prototype["name"]] = CreateEnemyPrototype(prototype["type"], prototype["data"]);
	}

	return true;
}

Enemy* ModuleEnemy::CreateEnemyPrototype(string type, const nlohmann::json& enemyData) const {
	PrototypeCreationFunction prototypeCreationFunction = prototypeCreationFunctionMap.at(type);
	return invoke(prototypeCreationFunction,*this,enemyData);
}

update_status ModuleEnemy::PreUpdate()
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
	{
		if ((*it)->ToDelete())
		{
			RELEASE(*it);
			it = enemies.erase(it);
		}
		else
			++it;
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEnemy::Update()
{
	for (Enemy* enemy : enemies) {
		enemy->Update();
	}

	for (Enemy* enemy : enemies) {
		enemy->Render();
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleEnemy::CleanUp()
{
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
	
	for (auto enemyPrototypePair : enemyPrototypes) {
		Enemy* enemyPrototype = enemyPrototypePair.second;
		delete enemyPrototype;
	}
	enemyPrototypes.clear();

	return true;
}

Enemy * ModuleEnemy::InstantiateEnemyByName(const string& name, map<string, void*> parameters)
{
	const Enemy* prototype = enemyPrototypes.at(name);
	
	Enemy* instance = prototype->Clone();
	instance->Init(parameters);
	enemies.push_back(instance);

	return instance;
}

Enemy * ModuleEnemy::CreateObstaclePrototype(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	Animation animation = data["animation"];
	float scalingFactor = data["scalingFactor"];
	Size2D size = data["size"];
	bool hasShadow = data["hasShadow"];
	bool isNonDamaging = data["isNonDamaging"];

	return new Obstacle(graphics, animation, hasShadow, size, scalingFactor, isNonDamaging);
}

Enemy * ModuleEnemy::CreateOvniPrototype(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	Animation animation = data["animation"];
	float scalingFactor = data["scalingFactor"];
	Size2D size = data["size"];

	vector<Vector3> path = data["path"];
	set<unsigned int> particleSpawns = data["particleSpawns"];
	float speed = data["speed"];
	float particleSpeed = data["particleSpeed"];

	return new Ovni(speed, particleSpeed, graphics, animation, size, path, particleSpawns, scalingFactor);
}

Enemy * ModuleEnemy::CreateShieldedOvniBrainPrototype(const json & data) const
{
	list<Vector3> topPath = data["topPath"];
	list<Vector3> leftPath = data["leftPath"];
	list<Vector3> rightPath = data["rightPath"];

	return new ShieldedOvniBrain(leftPath, rightPath, topPath);
}

Enemy * ModuleEnemy::CreateShieldedOvni(const json & data) const
{
	float speed = data["speed"];
	float projectileSpeed = data["projectileSpeed"];
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	Animation animation = data["animation"];
	Size2D size = data["size"];
	float scalingFactor = data["scalingFactor"];

	return new ShieldedOvni(speed, projectileSpeed, graphics, animation, size, scalingFactor);
}

Enemy * ModuleEnemy::CreateExplosion(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	Animation animation = data["animation"];
	float scalingFactor = data["scalingFactor"];
	SFX explosionSoundPath = App->audio->LoadFx(data["explosionSoundPath"]);

	return new Explosion(graphics, animation, scalingFactor, explosionSoundPath);
}
