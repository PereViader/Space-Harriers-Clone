#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(bool active) : 
	Module(active)
{
}

ModulePlayer::~ModulePlayer()
{
}

bool ModulePlayer::Init()
{
	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	player = new Player();
	return true;
}

update_status ModulePlayer::Update()
{
	player->Update();
	player->Render();
	return update_status::UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	RELEASE(player);
	return true;
}

Player & ModulePlayer::GetPlayer() const
{
	assert(player);
	return *player;
}