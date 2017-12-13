#pragma once

#include "Module.h"
#include "ICollidable.h"

#include "Texture.h"

#include <map>
#include <string>
#include <list>

struct SDL_Texture;

using namespace std;

class Enemy;

class ModuleEnemy :
	public Module
{
public:
	ModuleEnemy(bool enabled);
	~ModuleEnemy();

	bool Start() override;

	update_status PreUpdate() override;
	update_status Update() override;
	bool CleanUp() override;

	Enemy* InstantiateEnemyByName(const string& name, map<string, void*> parameters);
private:
	const Enemy* GetEnemyPrototypeByName(const string& name);

	map<string, Enemy*> enemyPrototypes;

	Texture treeGraphic;
	Texture rock_bush;
	
	list<Enemy*> enemies;
};