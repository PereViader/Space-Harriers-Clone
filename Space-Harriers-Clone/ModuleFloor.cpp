#include "ModuleFloor.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include <algorithm>

#include <iostream>
#include <cmath>

using namespace std; 

ModuleFloor::ModuleFloor(bool enabled) :
	Module(enabled)
{
}


ModuleFloor::~ModuleFloor()
{
}



bool ModuleFloor::Init()
{
	App->player->Enable();

	floor = App->textures->Load("rtype/background.bmp");
	//vertical lines
	horizonRenderHeight = HORIZON_MIN_HEIGHT;
	horizontalSpeed = 0;
	previousHorizontalMovePercentage = 0.0f;

	SetAlphaLineParametersPercentual(1.0f);
	iterationOfAlphaLine = 0;
	return true;
}

update_status ModuleFloor::Update()
{

	RenderFloor();

	fPoint playerPosition = App->player->GetPosition();
	
	horizontalSpeed = HORIZONTAL_SPEED_MAX * playerPosition.x;
	horizonRenderHeight = HORIZON_MAX_HEIGHT + (HORIZON_MIN_HEIGHT-HORIZON_MAX_HEIGHT) * ((playerPosition.y + 1.0f) / 2.0f);

	return UPDATE_CONTINUE;
}

void ModuleFloor::RenderFloor() {
	//Number of pixels between each line from the texture to be blit to the screen
	float originalTextureVerticalPixelsPerScreenRow = (float)FLOOR_PIXEL_HEIGHT / (SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight);


	//Rects for blitting from the texture to the screen
	const int extraSpaceForCurves = 260;
	SDL_Rect floorTextureRect = {extraSpaceForCurves,0,FLOOR_PIXEL_WIDTH-extraSpaceForCurves*2,1 };
	SDL_Rect screenBlitRect = {0,horizonRenderHeight,SCREEN_WIDTH*SCREEN_SIZE,1};


	//Move Floor Horizontally
	float horizonDisplayPercentage = (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight)/(SCREEN_HEIGHT*SCREEN_SIZE - HORIZON_MAX_HEIGHT);
	float alphaIncrementForCurrentHorizon = 1.0f / horizonDisplayPercentage;
	float currentMaxHorizontalAlpha = 0.33f*alphaIncrementForCurrentHorizon;

	float horizontalAlpha = previousHorizontalMovePercentage*currentMaxHorizontalAlpha + horizontalSpeed*alphaIncrementForCurrentHorizon;
	horizontalAlpha = fmod(horizontalAlpha, currentMaxHorizontalAlpha);
	previousHorizontalMovePercentage = horizontalAlpha / currentMaxHorizontalAlpha;

	float iterationAlpha = (float)floorTextureRect.x;

	// Texture row selector
	float currentRenderPosition = 0;

	for (; screenBlitRect.y < SCREEN_HEIGHT*SCREEN_SIZE; screenBlitRect.y++) {
		App->renderer->DirectBlit(floor, &floorTextureRect, &screenBlitRect);

		//Get the next texture line
		currentRenderPosition += originalTextureVerticalPixelsPerScreenRow;
		floorTextureRect.y = (int)currentRenderPosition;

		//Position the texture line to move with the horizontal speed
		iterationAlpha += horizontalAlpha;
		floorTextureRect.x = (int)iterationAlpha;
	}
	SetAlphaLineParametersPercentual(horizonDisplayPercentage);
	AlphaVerticalLinesMove();
}

void ModuleFloor::AlphaVerticalLinesMove()
{
	distanceBetweenAlphaLines = startDistanceBetweenAlphaLines;
	sizeOfAlphaLines = startSizeOfAlphaLines;

	float coef = iterationOfAlphaLine / distanceBetweenAlphaLines;
	float offsetDif = 0;

	while (distanceBetweenAlphaLines <= SCREEN_HEIGHT * SCREEN_SIZE - horizonRenderHeight)
	{
		int yPos = (int)(SCREEN_HEIGHT * SCREEN_SIZE - (distanceBetweenAlphaLines - coef*sizeOfAlphaLines));
		int height = (int)(sizeOfAlphaLines + (offsetDif * (coef / 2)));
		const SDL_Rect quadRect = { 0, yPos, SCREEN_WIDTH * SCREEN_SIZE, height };

		App->renderer->DrawQuad(quadRect, 0, 0, 0, 50, false);

		offsetDif = sizeOfAlphaLines / 4.0f;
		//sizeOfAlphaLines -= offsetDif;
		sizeOfAlphaLines = max(1.0f, sizeOfAlphaLines - offsetDif);
		distanceBetweenAlphaLines += (sizeOfAlphaLines * 2.0f);
	}

	iterationOfAlphaLine = (iterationOfAlphaLine + 2) % (int)(startDistanceBetweenAlphaLines * 2);
}

void ModuleFloor::SetAlphaLineParametersPercentual(float percent) {
	startDistanceBetweenAlphaLines = ALPHA_DISTANCE_MIN + (percent*(ALPHA_DISTANCE_MAX - ALPHA_DISTANCE_MIN));
	startSizeOfAlphaLines = ALPHA_SIZE_MIN + (percent*(ALPHA_SIZE_MAX - ALPHA_SIZE_MIN));
}