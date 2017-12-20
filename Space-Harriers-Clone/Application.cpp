#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleTime.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleStage.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"

#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleShadow.h"


using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(time = new ModuleTime());
	modules.push_back(shadow = new ModuleShadow());

	// Game Modules
	modules.push_back(moduleStage = new ModuleStage(false));
	modules.push_back(floor = new ModuleFloor(false));
	modules.push_back(background = new ModuleBackground(false));
	modules.push_back(enemies = new ModuleEnemy(false));
	modules.push_back(player = new ModulePlayer(false));


	// Modules to draw on top of game logic
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(collision = new ModuleCollision());

	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application()
{
	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; it++)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; it++)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	fade->FadeToBlack(moduleStage, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == update_status::UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == update_status::UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == update_status::UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

