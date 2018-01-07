#ifndef _MODULE_BACKGROUND_H_
#define _MODULE_BACKGROUND_H_

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

#endif // !_MODULE_BACKGROUND_H_