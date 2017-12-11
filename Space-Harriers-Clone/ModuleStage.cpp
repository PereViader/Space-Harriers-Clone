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
	App->enemies->InstantiateEnemyByName("ovni", map<string, void*>());
	return true;
}

update_status ModuleStage::Update()
{
	/*currentTime += App->time->GetDeltaTime();

	if (currentTime > 0.5) {
		Enemy * enemy = App->enemies->InstantiateEnemyByName("rock", map<string, void*>());
		Vector3 startingPositionDelta((float)(rand() % 800 - 500), (float)(rand() % 300 + 300));
		enemy->transform->Move(startingPositionDelta);
		currentTime = 0.0f;
	}*/
	
	return UPDATE_CONTINUE;
}
