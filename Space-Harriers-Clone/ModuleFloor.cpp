#include "ModuleFloor.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL\include\SDL.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleBackground.h"
#include "ModuleAudio.h"
#include "ModuleTime.h"

#include <algorithm>
#include <cmath>

using namespace std; 

const float ModuleFloor::HORIZONTAL_SPEED_MAX = 2.0f;
const float ModuleFloor::SEGMENT_REDUCTION = 0.60f;
const float ModuleFloor::HORIZONTAL_LINES_SPEED = 2.0f;

ModuleFloor::ModuleFloor(bool enabled) :
	Module(enabled),
	horizontalSpeedForOutsideUse(0)
{
}


ModuleFloor::~ModuleFloor()
{
}



bool ModuleFloor::Init()
{
	segmentCount = 0;
	floor = App->textures->Load("rtype/floor.bmp");
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

#include <iostream>

update_status ModuleFloor::Update()
{
	Vector2 playerPosition = App->player->GetNormalizedPosition();

	horizontalSpeed = HORIZONTAL_SPEED_MAX * playerPosition.x *App->time->GetDeltaTime();
	horizontalSpeedForOutsideUse = -horizontalSpeed * (SCREEN_WIDTH * SCREEN_SIZE) * 1.2f;

	horizonRenderHeight = HORIZON_MAX_HEIGHT + static_cast<float>(HORIZON_MIN_HEIGHT - HORIZON_MAX_HEIGHT) * ((playerPosition.y + 1.0f) / 2.0f);

	RenderFloor();
	return update_status::UPDATE_CONTINUE;
}

bool ModuleFloor::CleanUp()
{
	App->textures->Unload(floor);
	return true;
}

float ModuleFloor::GetHorizonRenderHeight() const
{
	return horizonRenderHeight;
}

float ModuleFloor::GetCurrentHorizonPercentage() const
{
	return (float)(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (SCREEN_HEIGHT*SCREEN_SIZE - HORIZON_MAX_HEIGHT);
}

float ModuleFloor::GetHorizonPercentageOfPosition(float position) const
{
	position = max(horizonRenderHeight, min(static_cast<float>(SCREEN_HEIGHT*SCREEN_SIZE), position));
	return 1.0f - (SCREEN_HEIGHT*SCREEN_SIZE - position) / (SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight);
}

float ModuleFloor::GetHorizonDepthForPosition(float yPosition) const
{
	float percentage = 1.0f - GetHorizonPercentageOfPosition(yPosition);
	return Z_MAX * percentage;
}

float ModuleFloor::GetHorizonPositionForDepth(float depth) const
{
	float percentage = CalculatePercentageOfPositionInFloor(depth);
	return horizonRenderHeight + (SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) * percentage;
}

float ModuleFloor::GetCurrentFloorMovement() const
{
	return horizontalSpeedForOutsideUse;
}

float ModuleFloor::GetRenderYOfHorizontalStripe(int index) const
{
	return horizontalSegments[index].y;
}

float ModuleFloor::GetRenderHOfHorizontalStripe(int index) const
{
	return horizontalSegments[index].h;
}

int ModuleFloor::GetFurtherHorizontalStripeIndex() const
{
	return (firstQuadIndex-1) < 0 ? (nHorizonQuads-1) : (firstQuadIndex - 1);
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
	const float extraSpaceForCurves = 260;
	SDL_Rect floorTextureRect = { static_cast<int>(extraSpaceForCurves),0, static_cast<int>(FLOOR_PIXEL_WIDTH - extraSpaceForCurves * 2), 1 };
	SDL_Rect screenBlitRect = { 0,static_cast<int>(horizonRenderHeight),SCREEN_WIDTH*SCREEN_SIZE,1 };


	//Move Floor Horizontally
	float horizonDisplayPercentage = GetCurrentHorizonPercentage();
	float alphaIncrementForCurrentHorizon = 1.0f / horizonDisplayPercentage;
	float currentMaxHorizontalAlpha = 0.33f*alphaIncrementForCurrentHorizon;


	float horizontalAlpha = previousHorizontalMovePercentage*currentMaxHorizontalAlpha + horizontalSpeed*alphaIncrementForCurrentHorizon;
	horizontalAlpha = fmod(horizontalAlpha, currentMaxHorizontalAlpha);

	previousHorizontalMovePercentage = horizontalAlpha / currentMaxHorizontalAlpha;

	float iterationAlpha = extraSpaceForCurves;

	// Texture row selector
	float currentRenderPosition = 0;

	for (; screenBlitRect.y < SCREEN_HEIGHT*SCREEN_SIZE; screenBlitRect.y++) {
		floor.UpdateTexture(floorTextureRect);
		App->renderer->DirectBlit(floor, &screenBlitRect);

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
	float baseSegmentHeight = static_cast<float>(SCREEN_HEIGHT*SCREEN_SIZE - horizonRenderHeight) / (1.0f+ SEGMENT_REDUCTION + 
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
		horizontalSegments[currentQuad].y = currentRenderingPosition;
		horizontalQuads[currentQuad].h = (int)currentSegmentPrintedHeight;
		horizontalSegments[currentQuad].h = currentSegmentPrintedHeight;

		currentSegmentHeight = currentSegmentHeight * SEGMENT_REDUCTION;
		currentRenderingPosition -= currentSegmentHeight;

		currentQuad = (currentQuad + 1) % nHorizonQuads;
		hasLoopedArray = currentQuad == firstQuadIndex;
	} while (!hasLoopedArray);

	App->renderer->DrawQuads(horizontalQuads, nHorizonQuads, 0, 0, 0, 50);

	float nextfirstSegmentPositionPercentage = fmod(firstSegmentPositionPercentage + HORIZONTAL_LINES_SPEED*App->time->GetDeltaTime(), 1.0f);
	if (nextfirstSegmentPositionPercentage < firstSegmentPositionPercentage) {
		firstQuadIndex = (firstQuadIndex + 1) % nHorizonQuads;
		segmentCount++;
	}
	firstSegmentPositionPercentage = nextfirstSegmentPositionPercentage;
}