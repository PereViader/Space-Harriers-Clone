#include "ModuleEnemy.h"
#include "Enemy.h"
#include "Assert.h"


ModuleEnemy::ModuleEnemy()
{
	//TODO finish method

}


ModuleEnemy::~ModuleEnemy()
{
	//TODO finish method

}

bool ModuleEnemy::Init()
{
	//TODO finish method

	return false;
}

update_status ModuleEnemy::Update()
{
	//TODO finish method

	return update_status();
}

bool ModuleEnemy::CleanUp()
{

	//TODO finish method
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
		RELEASE(*it)
	}
	enemies.clear();



	return false;
}

void ModuleEnemy::OnCollision(const Collider * own, const Collider * other)
{
	//TODO finish method


}

Enemy * ModuleEnemy::InstantiateEnemyByName(string name)
{
	Enemy* prototype = GetEnemyPrototypeByName(name);
	assert(prototype != nullptr);
	
	Enemy* instance = prototype->Clone();
	enemies.push_back(instance);

	return instance;
}

const Enemy * ModuleEnemy::GetEnemyPrototypeByName(string name)
{
	map<string, const Enemy*>::const_iterator res = enemyPrototypes.find(name);
	return res != enemyPrototype.cend() ? res->second : nullptr;
}
