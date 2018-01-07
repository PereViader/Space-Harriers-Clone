#ifndef _MODULE_ENTITY_H__
#define _MODULE_ENTITY_H__

#include "Module.h"

#include <list>

class GameEntity;

class ModuleEntity :
	public Module
{
public:
	ModuleEntity();
	virtual ~ModuleEntity();

	virtual update_status PreUpdate() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

	void AddGameEntity(GameEntity *);

private:
	std::list<GameEntity*> entitiesToBeStarted;
	std::list<GameEntity*> entities;

};

#endif