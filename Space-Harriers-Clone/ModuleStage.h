#ifndef  _MODULE_STAGE_H_
#define _MODULE_STAGE_H_

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

	void EndGame();


	void OnStageBossDied();

private:
	int currentStage;
	int previousSegmentCount;
	json stageData;

	bool hasGameEnded;
	float gameResetTimer;
	static const float GAME_RESET_TIME;
private:
	void LoadNextStage();
	void StartGame();
	void StopGame();
	void RestartGame();

	void SpawnEnemies();

};

#endif // ! _MODULE_STAGE_H_