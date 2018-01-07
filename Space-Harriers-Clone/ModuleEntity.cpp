#include "ModuleEntity.h"

#include "GameEntity.h"

ModuleEntity::ModuleEntity() :
	Module(true)
{
}

ModuleEntity::~ModuleEntity()
{
}

update_status ModuleEntity::PreUpdate()
{
	for (auto it = entities.begin(); it != entities.end();)
	{
		if ((*it)->ToDelete())
		{
			// if it has not yet been started we need to delete it as well from the list
			for (auto it1 = entitiesToBeStarted.begin(); it1 != entitiesToBeStarted.end(); ++it1) {
				if (*it1 == *it) {
					entitiesToBeStarted.erase(it1);
					break;
				}
			}

			RELEASE(*it);
			it = entities.erase(it);
		}
		else
			++it;
	}
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleEntity::Update()
{
	for (GameEntity *gameEntity : entitiesToBeStarted) {
		if (!gameEntity->ToDelete()) {
			gameEntity->Start();
		}
	}
	entitiesToBeStarted.clear();

	for (GameEntity *gameEntity : entities) {
		if (!gameEntity->ToDelete()) {
			gameEntity->Update();
		}
	}

	for (GameEntity *gameEntity : entities) {
		if (!gameEntity->ToDelete()) {
			gameEntity->Render();
		}
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleEntity::CleanUp()
{
	for (GameEntity *&gameEntity : entities) {
		RELEASE(gameEntity);
	}
	entities.clear();
	entitiesToBeStarted.clear();

	return false;
}

void ModuleEntity::AddGameEntity(GameEntity * gameEntity)
{
	entitiesToBeStarted.push_back(gameEntity);
	entities.push_back(gameEntity);
}
