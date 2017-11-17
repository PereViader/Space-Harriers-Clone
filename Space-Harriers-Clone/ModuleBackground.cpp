#include "ModuleBackground.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFloor.h"


#include <string>
#include <assert.h>

#include "Globals.h"

const ModuleBackground::float speed = 2.0f; // TODO SET SPEED

const float ModuleBackground::BACKGROUND_HORIZONTAL_SPEED = 1;
const float ModuleBackground::HORIZON_DECAL_HORIZONTAL_SPEED = 1.5f;

const int ModuleBackground::HORIZON_DECAL_WINDOW_WIDTH = SCREEN_WIDTH*SCREEN_SIZE;
const int ModuleBackground::HORIZON_DECAL_WINDOW_HEIGHT = 100;

ModuleBackground::ModuleBackground()
{
}


ModuleBackground::~ModuleBackground()
{
}

bool ModuleBackground::Start()
{
	SetLevel(0);

	//Initialize textures
	std::string path;
	for (int i = 0; i < NUMBER_OF_LEVELS; i++) {
		path = "horizonDecal";
		path += i;
		path += ".png";
		horizonDecal[i] = App->textures->Load(path.c_str()); // TODO: Encontrar horizonte bueno
		path = "background";
		path += i;
		path += ".png";
		background[i] = App->textures->Load(path.c_str()); // TODO: Igual
	}


	//Initialize variables
	currentRenderingBackgroundStartInTexture = 0;

	return true;
}

update_status ModuleBackground::Update()
{
	SDL_Rect backgroundScreenSection = { 0,0,SCREEN_WIDTH*SCREEN_SIZE,App->moduleFloor->GetHorizonRenderHeight() };

	SDL_Rect backgroundTextureSection;
	<.x = currentRenderingBackgroundStartInTexture;
	backgroundTextureSection.y = 0;
	backgroundTextureSection.h = currentLevelBackgroundTextureHeight;
	if (currentRenderingBackgroundStartInTexture + HORIZON_DECAL_WINDOW_WIDTH <= currentLevelBackgroundTextureWidth) {
		backgroundTextureSection.w = HORIZON_DECAL_WINDOW_WIDTH;
	}
	else {


		int leftWarpSize = (currentRenderingBackgroundStartInTexture + HORIZON_DECAL_WINDOW_WIDTH - currentLevelBackgroundTextureWidth);
		SDL_Rect backgroundWarpScreenSection = { 0,0,leftWarpSize,App->moduleFloor->GetHorizonRenderHeight() };
		backgroundScreenSection.x = leftWarpSize;
		backgroundTextureSection.w = HORIZON_DECAL_WINDOW_WIDTH - leftWarpSize;
		SDL_Rect warpBackgroundRect = backgroundTextureSection;
		warpBackgroundRect.w = leftWarpSize;
		App->renderer->DirectBlit(background[currentLevel],)
	}

	App->renderer->DirectBlit(background[currentLevel], )




}

void ModuleBackground::NextLevel()
{
	int nextLevel = currentLevel + 1;
	SetLevel(nextLevel);
}

void ModuleBackground::SetLevel(int level)
{
	assert(level >= 0 && level < NUMBER_OF_LEVELS);
	currentLevel = level;
	SDL_QueryTexture(horizonDecal[currentLevel], nullptr, nullptr, &currentLevelHorizonDecalTextureWidth, &currentLevelHorizonDecalTextureHeight);
	SDL_QueryTexture(background[currentLevel], nullptr, nullptr, &currentLevelBackgroundTextureWidth, &currentLevelBackgroundTextureHeight);
}
