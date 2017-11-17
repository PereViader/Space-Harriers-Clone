#include "ModuleBackground.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFloor.h"
#include "ModulePlayer.h"


#include <string>
#include <assert.h>

#include "Globals.h"

const float ModuleBackground::BACKGROUND_HORIZONTAL_SPEED = 1;
const float ModuleBackground::HORIZON_DECAL_HORIZONTAL_SPEED = 1.5f;

const int ModuleBackground::BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW = 400;


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
		horizonDecal[i] = App->textures->Load(path.c_str()); // TODO: Encontrar horizonte bueno
		path = "rtype/background";
		path.append(std::to_string(i));
		path.append(".png");
		background[i] = App->textures->Load(path.c_str()); // TODO: Igual
	}

	SetLevel(0);


	//Initialize variables
	backgroundTextureOffset = 1000;

	return true;
}

update_status ModuleBackground::Update()
{
	DrawBackground();
	return UPDATE_CONTINUE;
}

void ModuleBackground::DrawBackground() {
	MoveBackground();

	SDL_Rect leftScreenSection, leftTextureSection;

	// Shared attibutes initialization
	leftScreenSection.x = 0;
	leftScreenSection.y = 0;
	leftScreenSection.h = App->moduleFloor->GetHorizonRenderHeight();

	leftTextureSection.y = 0;
	leftTextureSection.h = currentLevelBackgroundTextureHeight;

	if (DoesBackgroundNeed2Blits()) {				
		SDL_Rect rightScreenSection, rightTextureSection;
		rightScreenSection = leftScreenSection;
		rightTextureSection = leftTextureSection;

		rightTextureSection.x = backgroundTextureOffset;
		rightTextureSection.w = currentLevelBackgroundTextureWidth - backgroundTextureOffset;

		leftTextureSection.x = 0;
		leftTextureSection.w = BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW - rightTextureSection.w;


		float rightTexturePercentage = (float)rightTextureSection.w / BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW;

		leftScreenSection.w = (int)(SCREEN_WIDTH*SCREEN_SIZE*rightTexturePercentage);


		rightScreenSection.w = SCREEN_WIDTH*SCREEN_SIZE- leftScreenSection.w;
		rightScreenSection.x = SCREEN_WIDTH*SCREEN_SIZE - rightScreenSection.w;


		App->renderer->DirectBlit(background[currentLevel], &rightTextureSection, &leftScreenSection);
		App->renderer->DirectBlit(background[currentLevel], &leftTextureSection, &rightScreenSection);
	}
	else {
		leftScreenSection.w = SCREEN_WIDTH*SCREEN_SIZE;

		leftTextureSection.x = backgroundTextureOffset;
		leftTextureSection.w = BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW;
		App->renderer->DirectBlit(background[currentLevel], &leftTextureSection, &leftScreenSection);
	}
}

void ModuleBackground::MoveBackground()
{
	backgroundTextureOffset = (int)(backgroundTextureOffset + App->player->GetPosition().x * BACKGROUND_HORIZONTAL_SPEED) % currentLevelBackgroundTextureWidth;
	if (backgroundTextureOffset < 0)
		backgroundTextureOffset = currentLevelBackgroundTextureWidth - backgroundTextureOffset - 1;
}

bool ModuleBackground::DoesBackgroundNeed2Blits() const
{
	return backgroundTextureOffset + BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW > currentLevelBackgroundTextureWidth;
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
