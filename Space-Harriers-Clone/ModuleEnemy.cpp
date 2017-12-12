#include "ModuleEnemy.h"

#include "Enemy.h"
#include "Obstacle.h"
#include "Ovni.h"

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
	rock_bush = App->textures->Load("rtype/rock_bush.png");


	Animation treeAnimation;
	treeAnimation.frames.push_back({ 206,48,44,163 });
	const float treeScalingFactor = 3.5f;
	Obstacle * tree = new Obstacle(treeGraphic, treeAnimation, false,treeScalingFactor);
	Size2D treeSize(44 * treeScalingFactor, 163 * treeScalingFactor);
	tree->collider = App->collision->AddPrototypeCollider(ColliderType::Enemy,treeSize, Pivot2D::BOTTOM_CENTER,tree);
	enemyPrototypes["tree"] = tree;

	
	Animation rockAnimation;
	rockAnimation.frames.push_back({192,72,59,37});
	const float rockScalingFactor = 3;
	Obstacle * rock = new Obstacle(rock_bush, rockAnimation, true, rockScalingFactor);
	Size2D rockSize(59 * rockScalingFactor, 37 * rockScalingFactor);
	rock->collider = App->collision->AddPrototypeCollider(ColliderType::Enemy, rockSize , Pivot2D::BOTTOM_CENTER, rock);
	enemyPrototypes["rock"] = rock;


	Vector3 startOvni(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

	vector<Vector3> path;
	path.push_back(Vector3(0, SCREEN_HEIGHT / 2.0f));
	path.push_back(Vector3(0, SCREEN_HEIGHT,15));
	path.push_back(Vector3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 15));

	Ovni * ovni = new Ovni(startOvni, 700, rock_bush, rockAnimation, path, rockScalingFactor);
	ovni->collider = App->collision->AddPrototypeCollider(ColliderType::Enemy, rockSize, Pivot2D::BOTTOM_CENTER, ovni);
	enemyPrototypes["ovni"] = ovni;

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
	for (list<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
		RELEASE(*it);

	enemies.clear();

	
	for (map<string, Enemy*>::iterator it = enemyPrototypes.begin(); it != enemyPrototypes.end(); ++it)
		RELEASE(it->second);

	enemyPrototypes.clear();

	App->textures->Unload(treeGraphic);
	App->textures->Unload(rock_bush);

	return true;
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
	map<string, Enemy*>::const_iterator res = enemyPrototypes.find(name);
	return (res != enemyPrototypes.cend()) ? res->second : nullptr;
}
