#pragma once
#include "Module.h"
class ModuleStage :
	public Module
{
public:
	ModuleStage(bool enabled);
	~ModuleStage();

	bool Start() override;
	update_status Update() override;

private:
	float currentTime;

};

