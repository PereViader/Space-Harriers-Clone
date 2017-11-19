#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"

ModuleStage::ModuleStage(bool enabled) : Module(enabled)
{
}


ModuleStage::~ModuleStage()
{
}

bool ModuleStage::Start()
{
	App->player->Enable();
	App->moduleBackground->Enable();
	App->moduleFloor->Enable();
	App->audio->PlayMusic("rtype/main_theme.wav");

	return true;
}
