#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"

ModuleStage::ModuleStage(bool enabled) : Module(enabled)
{
	currentTime = 10;
}


ModuleStage::~ModuleStage()
{
}

bool ModuleStage::Start()
{
	App->player->Enable();
	App->background->Enable();
	App->floor->Enable();
	App->enemies->Enable();
	App->audio->PlayMusic("rtype/main_theme.wav");

	App->enemies->InstantiateEnemyByName("tree");
	return true;
}

update_status ModuleStage::Update()
{
	
	return UPDATE_CONTINUE;
}
