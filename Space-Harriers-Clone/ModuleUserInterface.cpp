#include "ModuleUserInterface.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"


ModuleUserInterface::ModuleUserInterface(bool enabled)
{
}

ModuleUserInterface::~ModuleUserInterface()
{
}

bool ModuleUserInterface::Start()
{
	return false;
}

update_status ModuleUserInterface::Update()
{
	return update_status::UPDATE_CONTINUE;
}

void ModuleUserInterface::RenderPlayerHealthPoints()
{
	int currentHP = App->player->GetHealthPoints();
	LOG("%i", currentHP);
}
