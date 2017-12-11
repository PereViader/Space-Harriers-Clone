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
#include "Pivot2D.h"

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

	Vector2 position;
	position.y = App->floor->GetHorizonRenderHeight();
	if (SCREEN_SIZE*SCREEN_WIDTH > (currentLevelBackgroundTextureWidth - backgroundTextureOffset)*BACKGROUND_SCALE) {
		position.x = (currentLevelBackgroundTextureWidth - backgroundTextureOffset)*BACKGROUND_SCALE;

		App->renderer->BlitWithPivotScaled(background[currentLevel], BACKGROUND_SCALE, Pivot2D::BOTTOM_LEFT, position);
		App->renderer->BlitWithPivotScaled(background[currentLevel], BACKGROUND_SCALE, Pivot2D::BOTTOM_RIGHT, position);
	}
	else {
		Pivot2D pivot(static_cast<float>(backgroundTextureOffset) / currentLevelBackgroundTextureWidth, 1);

		App->renderer->BlitWithPivotScaled(background[currentLevel], BACKGROUND_SCALE, pivot, position);
	}
}

void ModuleBackground::DrawDecal() {
	MoveDecal();

	Vector2 position;
	position.y = App->floor->GetHorizonRenderHeight();
	if (SCREEN_SIZE*SCREEN_WIDTH > (currentLevelHorizonDecalTextureWidth - decalTextureOffset)*DECAL_SCALE) {
		position.x = (currentLevelHorizonDecalTextureWidth - decalTextureOffset)*DECAL_SCALE;

		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], DECAL_SCALE, Pivot2D::BOTTOM_LEFT, position);
		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], DECAL_SCALE, Pivot2D::BOTTOM_RIGHT, position);
	}
	else {
		Pivot2D pivot(static_cast<float>(decalTextureOffset) / currentLevelHorizonDecalTextureWidth, 1);

		App->renderer->BlitWithPivotScaled(horizonDecal[currentLevel], DECAL_SCALE, pivot, position);
	}
}

void ModuleBackground::MoveBackground()
{
	backgroundTextureOffset = (int)(backgroundTextureOffset + App->player->GetNormalizedPosition().x * BACKGROUND_HORIZONTAL_SPEED) % currentLevelBackgroundTextureWidth;
	if (backgroundTextureOffset < 0)
		backgroundTextureOffset = currentLevelBackgroundTextureWidth - backgroundTextureOffset - 1;
}

void ModuleBackground::MoveDecal()
{
	decalTextureOffset = (int)(decalTextureOffset + App->player->GetNormalizedPosition().x * HORIZON_DECAL_HORIZONTAL_SPEED) % currentLevelHorizonDecalTextureWidth;
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
	currentLevelHorizonDecalTextureWidth = horizonDecal[currentLevel].GetSection().w;
	currentLevelHorizonDecalTextureHeight = horizonDecal[currentLevel].GetSection().h;

	currentLevelBackgroundTextureWidth = background[currentLevel].GetSection().w;
	currentLevelBackgroundTextureHeight = background[currentLevel].GetSection().h;
	//SDL_QueryTexture(horizonDecal[currentLevel].GetTexture(), NULL, NULL, &currentLevelHorizonDecalTextureWidth, &currentLevelHorizonDecalTextureHeight);
	//SDL_QueryTexture(background[currentLevel].GetTexture(), NULL, NULL, &currentLevelBackgroundTextureWidth, &currentLevelBackgroundTextureHeight);
}
