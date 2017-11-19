#include "ModuleTime.h"
#include "SDL/include/SDL.h"


ModuleTime::ModuleTime(bool enabled) : Module(enabled)
{}


ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init()
{
	currentDeltaTime = 0;
	previousTicks = 0;
	return true;
}

update_status ModuleTime::PreUpdate()
{
	Uint32 currentTicks = SDL_GetTicks();
	currentDeltaTime = (currentTicks - previousTicks)/1000.0f;
	previousTicks = currentTicks;
	return UPDATE_CONTINUE;
}

float ModuleTime::GetDeltaTime() const
{
	return currentDeltaTime;
}
