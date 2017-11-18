#include "ModuleBackground.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFloor.h"
#include "ModulePlayer.h"


#include <string>
#include <assert.h>
#include <iostream>

#include "Globals.h"

const float ModuleBackground::BACKGROUND_HORIZONTAL_SPEED = 1;
const float ModuleBackground::HORIZON_DECAL_HORIZONTAL_SPEED = 1.5f;

const float ModuleBackground::BACKGROUND_SCALE = 3.0f;
const float ModuleBackground::DECAL_SCALE = 1.0f;


ModuleBackground::ModuleBackground(bool enabled) : Module(enabled)
{
}


ModuleBackground::~ModuleBackground()
{
}

bool ModuleBackground::Start()
{
	//Initialize textures
	std::string path;
	for (int i = 0; i < NUMBER_OF_LEVELS; i++) {
		path = "rtype/horizonDecal";
		path.append(std::to_string(i));
		path.append(".png");
		horizonDecal[i] = App->textures->Load(path.c_str());
		path = "rtype/background";
		path.append(std::to_string(i));
		path.append(".png");
		background[i] = App->textures->Load(path.c_str());
	}

	SetLevel(0);


	//Initialize variables
	backgroundTextureOffset = 0;

	return true;
}

update_status ModuleBackground::Update()
{
	DrawBackground();
	DrawDecal();
	return UPDATE_CONTINUE;
}

void ModuleBackground::DrawBackground() {
	MoveBackground();

	int horizonRenderHeight = App->moduleFloor->GetHorizonRenderHeight();
	if (SCREEN_SIZE*SCREEN_WIDTH > (currentLevelBackgroundTextureWidth - backgroundTextureOffset)*BACKGROUND_SCALE) {
		int screenX = (int)((currentLevelBackgroundTextureWidth - backgroundTextureOffset)*BACKGROUND_SCALE);
		float pivotXRight = 0;
		float pivotYRight = 1;
		float pivotXLeft = 1;
		float pivotYLeft = 1;

		App->renderer->BlitWithPivotScaled(background[currentLevel], nullptr, BACKGROUND_SCALE, pivotXRight, pivotYRight, screenX, horizonRenderHeight);
		App->renderer->BlitWithPivotScaled(background[currentLevel], nullptr, BACKGROUND_SCALE, pivotXLeft, pivotYLeft, screenX, horizonRenderHeight);
	}
	else {
		float pivotX = (float)backgroundTextureOffset/currentLevelBackgroundTextureWidth;
		float pivotY = 1;
		App->renderer->BlitWithPivotScaled(background[currentLevel], nullptr, BACKGROUND_SCALE, pivotX, pivotY, 0, horizonRenderHeight);
	}
}

void ModuleBackground::DrawDecal() {
	MoveDecal();

	int horizonRenderHeight = App->moduleFloor->GetHorizonRenderHeight();
	if (SCREEN_SIZE*SCREEN_WIDTH > (currentLevelHorizonDecalTextureWidth - decalTextureOffset)*DECAL_SCALE) {
		int screenX = (int)((currentLevelHorizonDecalTextureWidth - decalTextureOffset)*DECAL_SCALE);
		float pivotXRight = 0;
		float pivotYRight = 1;
		float pivotXLeft = 1;
		float pivotYLeft = 1;

		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], nullptr, DECAL_SCALE, pivotXRight, pivotYRight, screenX, horizonRenderHeight);
		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], nullptr, DECAL_SCALE, pivotXLeft, pivotYLeft, screenX, horizonRenderHeight);
	}
	else {
		float pivotX = (float)decalTextureOffset / currentLevelHorizonDecalTextureWidth;
		float pivotY = 1;
		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], nullptr, DECAL_SCALE, pivotX, pivotY, 0, horizonRenderHeight);
	}
}

void ModuleBackground::MoveBackground()
{
	backgroundTextureOffset = (int)(backgroundTextureOffset + App->player->GetPosition().x * BACKGROUND_HORIZONTAL_SPEED) % currentLevelBackgroundTextureWidth;
	if (backgroundTextureOffset < 0)
		backgroundTextureOffset = currentLevelBackgroundTextureWidth - backgroundTextureOffset - 1;
}

void ModuleBackground::MoveDecal()
{
	decalTextureOffset = (int)(decalTextureOffset + App->player->GetPosition().x * HORIZON_DECAL_HORIZONTAL_SPEED) % currentLevelHorizonDecalTextureWidth;
	if (decalTextureOffset < 0)
		decalTextureOffset = currentLevelHorizonDecalTextureWidth - decalTextureOffset - 1;
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
	SDL_QueryTexture(horizonDecal[currentLevel], NULL, NULL, &currentLevelHorizonDecalTextureWidth, &currentLevelHorizonDecalTextureHeight);
	SDL_QueryTexture(background[currentLevel], NULL, NULL, &currentLevelBackgroundTextureWidth, &currentLevelBackgroundTextureHeight);
}
