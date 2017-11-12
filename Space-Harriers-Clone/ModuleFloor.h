#pragma once

#include "Module.h"

struct SDL_Texture;

class ModuleFloor :
	public Module
{
public:
	ModuleFloor(bool enabled);
	virtual ~ModuleFloor();

	bool Init() override;
	update_status Update() override;

private:
	static const int HORIZON_MAX_HEIGHT = (int)((SCREEN_HEIGHT * SCREEN_SIZE) / 2.0f);
	//static const int HORIZON_MIN_HEIGHT = HORIZON_MAX_HEIGHT + (int)((SCREEN_HEIGHT * SCREEN_SIZE) / 5.0f);
	static const int HORIZON_MIN_HEIGHT = SCREEN_HEIGHT * SCREEN_SIZE - 50;

	static const int FLOOR_PIXEL_HEIGHT = 400;
	static const int FLOOR_PIXEL_WIDTH = 800;

	SDL_Texture * floor;

	//vertical lines
	int horizonRenderHeight;
	const float HORIZONTAL_SPEED_MAX = 0.01f;
	float horizontalSpeed;
	float previousHorizontalMovePercentage;

	//horizontal lines
	float ALPHA_DISTANCE_MAX = 20.0f*SCREEN_SIZE;
	float  ALPHA_DISTANCE_MIN = 6.0f*SCREEN_SIZE;
	float  ALPHA_SIZE_MAX = 10.3f*SCREEN_SIZE;
	float  ALPHA_SIZE_MIN = 3.32f*SCREEN_SIZE;

	float startDistanceBetweenAlphaLines;
	float distanceBetweenAlphaLines;

	float startSizeOfAlphaLines;
	float sizeOfAlphaLines;

	int iterationOfAlphaLine;
	int horizonY;

private:
	void RenderFloor();
	void AlphaVerticalLinesMove();
	void SetAlphaLineParametersPercentual(float percent);


};