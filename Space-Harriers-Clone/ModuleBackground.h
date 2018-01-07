#pragma once
#include "Module.h"
#include "SDL\include\SDL.h"
#include <list>

#include "Texture.h"

class BackgroundLayer;

class ModuleBackground :
	public Module
{
public:
	ModuleBackground(bool enabled);
	virtual ~ModuleBackground();

	virtual bool Start() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

private:
	list<BackgroundLayer*> backgroundLayers;

};

