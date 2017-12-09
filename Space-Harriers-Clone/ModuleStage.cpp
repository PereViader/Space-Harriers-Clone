#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"
#include "Enemy.h"
#include "Transform.h"
#include <cstdlib>

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

	return true;
}

update_status ModuleStage::Update()
{
	currentTime += App->time->GetDeltaTime();

	if (currentTime > 0.5) {
		Enemy * enemy = App->enemies->InstantiateEnemyByName("rock", map<string, void*>());
		float yPos = (float)(rand() % 300 + 300);
		float xPos = 0;
		enemy->transform->Move(Vector3(xPos, yPos, 0));
		currentTime = -999999999.0f;
	}
	
	return UPDATE_CONTINUE;
}
