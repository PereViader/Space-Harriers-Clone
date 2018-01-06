#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"
#include "ModuleUserInterface.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"

#include "Enemy.h"
#include "Transform.h"


#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

const float ModuleStage::GAME_RESET_TIME = 4.0f;

ModuleStage::ModuleStage(bool enabled) : 
	Module(enabled),
	currentStage(1),
	previousSegmentCount(-1),
	hasGameEnded(false),
	gameResetTimer(GAME_RESET_TIME)
{
	LoadNextStage();
}


ModuleStage::~ModuleStage()
{
}

bool ModuleStage::Start()
{
	StartGame();
	return true;
}

update_status ModuleStage::Update()
{
	SpawnEnemies();
	EndGame();
	
	return update_status::UPDATE_CONTINUE;
}

void ModuleStage::EndGame()
{
	hasGameEnded = hasGameEnded || App->player->GetHealthPoints() == 0;

	if (hasGameEnded) {
		gameResetTimer -= App->time->GetDeltaTime();
		if (gameResetTimer <= 0) {
			gameResetTimer = GAME_RESET_TIME;
			hasGameEnded = false;
			RestartGame();
		}
	}
}

void ModuleStage::SpawnEnemies()
{
	int currentSegmentCount = App->floor->GetCurrentSegmentCount();
	if (currentSegmentCount != previousSegmentCount) {
		previousSegmentCount = currentSegmentCount;

		for (auto jsonEnemy : stageData["enemies"]) {
			if (jsonEnemy["spawnId"] == currentSegmentCount) {
				Enemy * enemy = App->enemies->InstantiateEnemyByName(jsonEnemy["name"], jsonEnemy);
			}
		}
	}
}

void ModuleStage::RestartGame()
{
	StopGame();
	StartGame();
}

void ModuleStage::OnStageBossDied()
{
	hasGameEnded = true;
}

string CreateStageJsonFilePath(int stage) {
	string jsonFilePath = "data/stages/";
	jsonFilePath.append(to_string(stage));
	jsonFilePath.append(".json");
	return jsonFilePath;
}

void ModuleStage::LoadNextStage()
{
	previousSegmentCount = -1;
	currentStage++;
	ifstream jsonFile(CreateStageJsonFilePath(currentStage));
	stageData.clear();
	jsonFile >> stageData;
}

void ModuleStage::StartGame()
{
	App->player->Enable();
	App->background->Enable();
	App->floor->Enable();
	App->enemies->Enable();
	App->userInterface->Enable();
	App->particles->Enable();
	App->audio->PlayMusic(stageData["backgroundMusicPath"]);
}

void ModuleStage::StopGame()
{
	App->particles->Disable();
	App->userInterface->Disable();
	App->enemies->Disable();
	App->floor->Disable();
	App->background->Disable();
	App->player->Disable();
}
