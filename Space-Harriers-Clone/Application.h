#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleEntity;
class ModuleParticles;
class ModuleStage;
class ModuleFloor;
class ModuleBackground;
class ModuleTime;
class ModuleEnemy;
class ModuleShadow;
class ModuleUserInterface;

// Game modules ---
class ModulePlayer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleTime* time;
	ModuleFadeToBlack* fade;
	ModuleCollision* collision;
	ModuleEntity* entity;

	// Game modules ---
	ModuleParticles* particles;
	ModuleShadow* shadow;
	ModuleStage* moduleStage;
	ModulePlayer* player;
	ModuleBackground* background;
	ModuleFloor* floor;
	ModuleEnemy* enemies;
	ModuleUserInterface* userInterface;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__