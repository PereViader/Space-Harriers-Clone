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
class ModuleParticles;
class ModuleScene;
class ModuleFloor;
class ModuleBackground;
class ModuleTime;

// Game modules ---
class ModulePlayer;
class ModuleSceneIntro;
class ModuleSceneSpace;

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
	ModuleParticles* particles;

	// Game modules ---
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModuleSceneSpace* scene_space;
	ModuleScene* gameScene;
	ModuleBackground* moduleBackground;
	ModuleFloor* moduleFloor;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__