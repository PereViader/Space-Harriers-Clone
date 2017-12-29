#pragma once

#include "Module.h"
#include "ICollidable.h"

#include "Texture.h"

#include "json.hpp"
using json = nlohmann::json;

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
	list<Enemy*> enemies;
	map<string, Enemy*> enemyPrototypes;
	
	typedef Enemy*(ModuleEnemy::*PrototypeCreationFunction)(const json&) const;
	map<string, PrototypeCreationFunction> prototypeCreationFunctionMap;
	
	
private:
	Enemy * CreateEnemyPrototype(string type, const json& enemyData) const;
	Enemy * CreateObstaclePrototype(const json&) const;
	Enemy * CreateOvniPrototype(const json&) const;
	Enemy * CreateShieldedOvniBrainPrototype(const json&) const;
	Enemy * CreateShieldedOvni(const json&) const;
	Enemy * CreateExplosion(const json&) const;
};