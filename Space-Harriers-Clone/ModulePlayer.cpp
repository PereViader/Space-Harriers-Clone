#include "ModulePlayer.h"

#include "ModuleEntity.h"

ModulePlayer::ModulePlayer(bool active) : 
	Module(active)
{
}

ModulePlayer::~ModulePlayer()
{
}

// Load assets
bool ModulePlayer::Start()
{
	player = new Player();
	App->entity->AddGameEntity(player);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	player->MarkAsDeleted();
	player = nullptr;
	return true;
}

Player & ModulePlayer::GetPlayer() const
{
	assert(player);
	return *player;
}