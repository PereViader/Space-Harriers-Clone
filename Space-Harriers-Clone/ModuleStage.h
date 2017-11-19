#pragma once
#include "Module.h"
class ModuleStage :
	public Module
{
public:
	ModuleStage(bool enabled);
	~ModuleStage();

	bool Start() override;
};

