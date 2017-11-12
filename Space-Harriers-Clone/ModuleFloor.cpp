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
	firstSegmentPositionPercentage = 0.0f;

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
	RenderVerticalLines();
	RenderHorizontalLines();
}

void ModuleFloor::RenderVerticalLines()
{
	//Number of pixels between each line from the texture to be blit to the screen
	float originalTextureVerticalPixelsPerScreenRow = (float)FLOOR_PIXEL_HEIGHT / (SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight);


	//Rects for blitting from the texture to the screen
	const int extraSpaceForCurves = 260;
	SDL_Rect floorTextureRect = { extraSpaceForCurves,0,FLOOR_PIXEL_WIDTH - extraSpaceForCurves * 2,1 };
	SDL_Rect screenBlitRect = { 0,horizonRenderHeight,SCREEN_WIDTH*SCREEN_SIZE,1 };


	//Move Floor Horizontally
	float horizonDisplayPercentage = (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (SCREEN_HEIGHT*SCREEN_SIZE - HORIZON_MAX_HEIGHT);
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
}

/*void ModuleFloor::RenderHorizontalLines()
{ 
	//float horizonDisplayPercentage = (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (SCREEN_HEIGHT*SCREEN_SIZE - HORIZON_MAX_HEIGHT);

	float startingRenderingPosition = SCREEN_HEIGHT*SCREEN_SIZE - FIRST_HORIZONTAL_SEGMENT_HEIGHT*(1.0f - firstSegmentPositionPercentage);
	float firstSegmentHeight = FIRST_HORIZONTAL_SEGMENT_HEIGHT * (1.0f - firstSegmentPositionPercentage) + FIRST_HORIZONTAL_SEGMENT_HEIGHT * (1.0f / SEGMENT_REDUCTION) * (firstSegmentPositionPercentage);
	
	float currentSegmentHeight = firstSegmentHeight;
	float currentRenderingPosition = startingRenderingPosition;

	int i = 0;
	//while (currentRenderingPosition > horizonRenderHeight) {
	while (i++ < 20){
		float currentSegmentPrintedHeight = currentSegmentHeight * 0.40f;
		SDL_Rect quadRect = { 0, (int)currentRenderingPosition, SCREEN_WIDTH*SCREEN_SIZE, (int)currentSegmentPrintedHeight };
		App->renderer->DrawQuad(quadRect, 0, 0, 0, 50, false);
		
		currentSegmentHeight = currentSegmentHeight * SEGMENT_REDUCTION;
		currentRenderingPosition -= currentSegmentHeight;
	}

	firstSegmentPositionPercentage = fmod(firstSegmentPositionPercentage + 0.05f, 1.0f);
}*/

void ModuleFloor::RenderHorizontalLines()
{
	//float horizonDisplayPercentage = (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (SCREEN_HEIGHT*SCREEN_SIZE - HORIZON_MAX_HEIGHT);

	float startingRenderingPosition = SCREEN_HEIGHT*SCREEN_SIZE - FIRST_HORIZONTAL_SEGMENT_HEIGHT*(1.0f - firstSegmentPositionPercentage);
	float firstSegmentHeight = FIRST_HORIZONTAL_SEGMENT_HEIGHT * (1.0f - firstSegmentPositionPercentage) + FIRST_HORIZONTAL_SEGMENT_HEIGHT * (1.0f / SEGMENT_REDUCTION) * (firstSegmentPositionPercentage);

	float currentSegmentHeight = firstSegmentHeight;
	float currentRenderingPosition = startingRenderingPosition;
	  
	int i = 0;
	//while (currentRenderingPosition > horizonRenderHeight) {
	while (i++ < 50) {
		float currentSegmentPrintedHeight = currentSegmentHeight * 0.40f;
		SDL_Rect quadRect = { 0, (int)currentRenderingPosition, SCREEN_WIDTH*SCREEN_SIZE, (int)currentSegmentPrintedHeight };
		App->renderer->DrawQuad(quadRect, 0, 0, 0, 50, false);

		currentSegmentHeight = currentSegmentHeight * SEGMENT_REDUCTION;
		currentRenderingPosition -= currentSegmentHeight;
	}

	firstSegmentPositionPercentage = fmod(firstSegmentPositionPercentage + 0.05f, 1.0f);
}