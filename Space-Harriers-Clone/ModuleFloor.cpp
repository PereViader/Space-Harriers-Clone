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

const float ModuleFloor::HORIZONTAL_SPEED_MAX = 0.01f;
const float ModuleFloor::SEGMENT_REDUCTION = 0.60f;
const float ModuleFloor::HORIZONTAL_LINES_SPEED = 0.02f;

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

	for (int i = 0; i < nHorizonQuads; i++) {
		horizontalQuads[i].x = 0;
		horizontalQuads[i].w = SCREEN_SIZE*SCREEN_WIDTH;
	}

	firstQuadIndex = 0;


	positionPercentage = 0.0f;
	objectQuadIndex = nHorizonQuads -1;


	return true;
}

update_status ModuleFloor::Update()
{

	RenderFloor();

	fPoint playerPosition = App->player->GetPosition();
	
	horizontalSpeed = HORIZONTAL_SPEED_MAX * playerPosition.x;
	horizonRenderHeight = (int)(HORIZON_MAX_HEIGHT + (HORIZON_MIN_HEIGHT-HORIZON_MAX_HEIGHT) * ((playerPosition.y + 1.0f) / 2.0f));

	return UPDATE_CONTINUE;
}

int ModuleFloor::GetHorizonRenderHeight() const
{
	return horizonRenderHeight;
}

void ModuleFloor::RenderObject()
{
	const SDL_Rect& renderQuad = horizontalQuads[objectQuadIndex];

	SDL_Rect object;
	object.x = (int)(SCREEN_WIDTH*SCREEN_SIZE / 2.0f);
	object.y = (int)(renderQuad.y + renderQuad.h * positionPercentage);
	object.w = 10;
	object.h = 10;

	App->renderer->DrawQuad(object, 0, 0, 255, 255,false);
}

void ModuleFloor::RenderFloor() {
	RenderVerticalLines();
	RenderHorizontalLines();
	RenderObject();
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

void ModuleFloor::RenderHorizontalLines()
{
	float baseSegmentHeight = (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (1.0f+ SEGMENT_REDUCTION + 
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION + 
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION + 
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION + 
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION +
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION +
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION +
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION + 
																								SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION +
										 														SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION*SEGMENT_REDUCTION );

	float startingRenderingPosition = SCREEN_HEIGHT*SCREEN_SIZE - baseSegmentHeight*(1.0f - firstSegmentPositionPercentage);
	float firstSegmentHeight = baseSegmentHeight * (1.0f - firstSegmentPositionPercentage) + baseSegmentHeight * (1.0f / SEGMENT_REDUCTION) * (firstSegmentPositionPercentage);

	float currentSegmentHeight = firstSegmentHeight;
	float currentRenderingPosition = startingRenderingPosition;
	
	int currentQuad = firstQuadIndex;
	bool hasLoopedArray = false;
	do {
		float currentSegmentPrintedHeight = currentSegmentHeight * (1.0f - SEGMENT_REDUCTION);
		horizontalQuads[currentQuad].y = (int)currentRenderingPosition;
		horizontalQuads[currentQuad].h = (int)currentSegmentPrintedHeight;
		currentSegmentHeight = currentSegmentHeight * SEGMENT_REDUCTION;
		currentRenderingPosition -= currentSegmentHeight;

		currentQuad = (currentQuad + 1) % nHorizonQuads;
		hasLoopedArray = currentQuad == firstQuadIndex;
	} while (!hasLoopedArray);

	App->renderer->DrawQuads(horizontalQuads, nHorizonQuads, 0, 0, 0, 50);

	float nextfirstSegmentPositionPercentage = fmod(firstSegmentPositionPercentage + HORIZONTAL_LINES_SPEED, 1.0f);
	if (nextfirstSegmentPositionPercentage < firstSegmentPositionPercentage) {
		firstQuadIndex = (firstQuadIndex + 1) % nHorizonQuads;
	}
	firstSegmentPositionPercentage = nextfirstSegmentPositionPercentage;
}