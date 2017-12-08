#include "ModuleEnemy.h"

#include "Enemy.h"
#include "Obstacle.h"

#include "Assert.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"



ModuleEnemy::ModuleEnemy(bool enabled) :
	Module(enabled)
{
	//TODO finish method

}


ModuleEnemy::~ModuleEnemy()
{
}

bool ModuleEnemy::Start()
{
	//TODO finish method
	treeGraphic = App->textures->Load("rtype/tree.png");


	Animation treeAnimation;
	treeAnimation.frames.push_back({ 206,48,44,163 });
	Obstacle * tree = new Obstacle(treeGraphic, treeAnimation, false,3.5f);
	tree->collider = App->collision->AddPrototypeCollider(ColliderType::Enemy,44*3.5f,163*3.5f,0.5f,1.0f,tree);
	enemyPrototypes["tree"] = tree;

	return true;
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
	return UPDATE_CONTINUE;
}

update_status ModuleEnemy::Update()
{
	for (Enemy* enemy : enemies) {
		enemy->Update();
	}

	return UPDATE_CONTINUE;
}

bool ModuleEnemy::CleanUp()
{
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		RELEASE(*it);

	enemies.clear();

	
	for (map<string, const Enemy*>::iterator it = enemyPrototypes.begin(); it != enemyPrototypes.end(); ++it)
		RELEASE(it->second);

	enemyPrototypes.clear();

	App->textures->Unload(treeGraphic);

	return true;
}

void ModuleEnemy::OnCollision(const Collider * own, const Collider * other)
{
	//TODO finish  


}

Enemy * ModuleEnemy::InstantiateEnemyByName(string name, map<string, void*> parameters)
{
	const Enemy* prototype = GetEnemyPrototypeByName(name);
	assert(prototype != nullptr);
	
	Enemy* instance = prototype->Clone();
	instance->Init(parameters);
	enemies.push_back(instance);

	return instance;
}

const Enemy* ModuleEnemy::GetEnemyPrototypeByName(string name)
{
	map<string, const Enemy*>::const_iterator res = enemyPrototypes.find(name);
	return (res != enemyPrototypes.cend()) ? res->second : nullptr;
}
