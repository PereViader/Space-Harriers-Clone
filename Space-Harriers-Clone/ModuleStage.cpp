#include "ModuleStage.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"
#include "Enemy.h"
#include "Transform.h"

#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

ModuleStage::ModuleStage(bool enabled) : 
	Module(enabled),
	currentStage(0),
	previousSegmentCount(-1)
{
	LoadNextStage();
	json a;
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
	App->audio->PlayMusic(stageData["backgroundMusicPath"]);
	//App->enemies->InstantiateEnemyByName("ovni", map<string, void*>());
	return true;
}

update_status ModuleStage::Update()
{
	int currentSegmentCount = App->floor->GetCurrentSegmentCount();
	if (currentSegmentCount != previousSegmentCount) {
		previousSegmentCount = currentSegmentCount;

		for (auto jsonEnemy : stageData["enemies"]) {
			if (jsonEnemy["spawnId"] == currentSegmentCount) {
				Enemy * enemy = App->enemies->InstantiateEnemyByName(jsonEnemy["name"], map<string, void*>());
				Vector3 startingPositionDelta(jsonEnemy["position"].at(0), jsonEnemy["position"].at(1));
				enemy->GetTransform().Move(startingPositionDelta);
			}
		}
	}
	
	return update_status::UPDATE_CONTINUE;
}

string CreateStageJsonFilePath(int stage) {
	string jsonFilePath = "data/stages/";
	jsonFilePath.append(to_string(stage));
	jsonFilePath.append(".json");
	return jsonFilePath;
}

#include <iostream>
void ModuleStage::LoadNextStage()
{
	previousSegmentCount = -1;
	currentStage++;
	ifstream jsonFile(CreateStageJsonFilePath(currentStage));
	stageData.clear();
	jsonFile >> stageData;
}