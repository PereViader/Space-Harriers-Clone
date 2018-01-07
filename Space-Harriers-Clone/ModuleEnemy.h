#ifndef _MODULE_ENEMY_H_
#define _MODULE_ENEMY_H_

#include "Module.h"

#include "json.hpp"
using json = nlohmann::json;

#include <map>
#include <string>

using namespace std;

class Enemy;

class ModuleEnemy :
	public Module
{
public:
	ModuleEnemy(bool enabled);
	~ModuleEnemy();

	bool Start() override;
	bool CleanUp() override;

	Enemy* InstantiateEnemyByName(const string& name, const json& parameters);
private:
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
	Enemy * CreateBossDragon(const json&) const;
	Enemy * CreateBossDragonHead(const json&) const;
	Enemy * CreateBossDragonBody(const json&) const;
};

#endif // !_MODULE_ENEMY_H_