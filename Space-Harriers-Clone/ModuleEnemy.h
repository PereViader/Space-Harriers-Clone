#pragma once

#include "Module.h"

#include <map>
#include <string>
#include <list>

using namespace std;

class Enemy;

class ModuleEnemy :
	public Module
{
public:
	ModuleEnemy();
	~ModuleEnemy();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void OnCollision(const Collider* own, const Collider* other) override;

	Enemy* InstantiateEnemyByName(string name);
private:
	const Enemy* GetEnemyPrototypeByName(string name);

	map<string, const Enemy *> enemyPrototypes;
	
	list<Enemy*> enemies;
};