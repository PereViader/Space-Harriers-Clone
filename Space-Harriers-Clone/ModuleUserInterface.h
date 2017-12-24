#pragma once
#include "Module.h"
class ModuleUserInterface :
	public Module
{
public:
	ModuleUserInterface(bool enabled);
	virtual ~ModuleUserInterface();

	virtual bool Start() override;
	virtual update_status Update() override;

private:


private:
	void RenderPlayerHealthPoints();
};

