#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"
#include "Enemy.h"
#include "Transform.h"

ModuleStage::ModuleStage(bool enabled) : Module(enabled)
{
	currentTime = 0;
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

	//Enemy * enemy = App->enemies->InstantiateEnemyByName("tree",map<string,void*>());
	//enemy->transform->Move(Vector3(500, 0, 0));
	return true;
}

update_status ModuleStage::Update()
{
	if (currentTime >= 0) {
		currentTime += App->time->GetDeltaTime();

		if (currentTime > 2) {
			Enemy * enemy = App->enemies->InstantiateEnemyByName("rock", map<string, void*>());
			enemy->transform->Move(Vector3(0, 500, 0));
			currentTime = -1;
		}
	}
	
	return UPDATE_CONTINUE;
}
