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

#include "Boss_Dragon.h"
#include "Boss_Dragon_Head.h"
#include "Boss_Dragon_Body.h"

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
	prototypeCreationFunctionMap["bossDragon"] = &ModuleEnemy::CreateBossDragon;
	prototypeCreationFunctionMap["bossDragonHead"] = &ModuleEnemy::CreateBossDragonHead;
	prototypeCreationFunctionMap["bossDragonBody"] = &ModuleEnemy::CreateBossDragonBody;

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
	
	for (auto& enemyPrototypePair : enemyPrototypes) {
		Enemy* enemyPrototype = enemyPrototypePair.second;
		delete enemyPrototype;
	}
	enemyPrototypes.clear();

	return true;
}

Enemy * ModuleEnemy::InstantiateEnemyByName(const string& name, const json& parameters)
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

	float speed = data["speed"];
	float particleSpeed = data["particleSpeed"];
	SFX sfx = App->audio->LoadFx(data["sfxPath"]);

	return new Ovni(speed, particleSpeed, graphics, animation, size, sfx, scalingFactor);
}

Enemy * ModuleEnemy::CreateShieldedOvniBrainPrototype(const json & data) const
{
	vector<Vector3> topPath = data["topPath"];
	vector<Vector3> leftPath = data["leftPath"];
	vector<Vector3> rightPath = data["rightPath"];

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
	float timeOpen = data["timeOpen"];
	float timeClosed = data["timeClosed"];
	int stateSwitchesToLeave = data["stateSwitchesToLeave"];
	SFX sfx = App->audio->LoadFx(data["sfxPath"]);

	return new ShieldedOvni(speed, projectileSpeed, sfx, graphics, animation, size, scalingFactor, timeOpen, timeClosed, stateSwitchesToLeave);
}

Enemy * ModuleEnemy::CreateExplosion(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	Animation animation = data["animation"];
	float scalingFactor = data["scalingFactor"];
	SFX explosionSoundPath = App->audio->LoadFx(data["explosionSoundPath"]);

	return new Explosion(graphics, animation, scalingFactor, explosionSoundPath);
}

Enemy * ModuleEnemy::CreateBossDragon(const json & data) const
{
	string bossTheme = data["bossTheme"];

	return new Boss_Dragon(bossTheme);
}

Enemy * ModuleEnemy::CreateBossDragonHead(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	vector<Animation> forwardAnimations = data["forwardAnimations"];
	vector<Animation> backwardAnimations = data["backwardAnimations"];
	int healthPoints = data["healthPoints"];
	float timeBetweenRounds = data["timeBetweenRounds"];
	float timeBetweenBullets = data["timeBetweenBullets"];
	int bulletsForRound = data["bulletsForRound"];
	float bulletSpeed = data["bulletSpeed"];

	float scalingFactor = data["scalingFactor"];
	SFX sfx = App->audio->LoadFx(data["sfx"]);
	Size2D size = data["size"];
	Vector3 speed = data["speed"];

	return new Boss_Dragon_Head(graphics,forwardAnimations, backwardAnimations, healthPoints, sfx,size,scalingFactor, speed, timeBetweenRounds, timeBetweenBullets, bulletsForRound, bulletSpeed);
}

Enemy * ModuleEnemy::CreateBossDragonBody(const json & data) const
{
	Texture graphics = App->textures->Load(data["graphicsPath"]);
	vector<Animation> animations = data["animations"];
	float scalingFactor = data["scalingFactor"];
	SFX sfx = App->audio->LoadFx(data["sfx"]);
	Size2D size = data["size"];

	return new Boss_Dragon_Body(graphics, animations, sfx, size, scalingFactor);
}