#pragma once

#include "Module.h"

#include "json.hpp"
using json = nlohmann::json;

class ModuleStage :
	public Module
{
public:
	ModuleStage(bool enabled);
	~ModuleStage();

	bool Start() override;
	update_status Update() override;

private:
	int currentStage;
	int previousSegmentCount;
	json stageData;

private:
	void LoadNextStage();
	void StartGame();
	void LoseGame();
};

