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

	//Horizon
	static const int HORIZON_MAX_HEIGHT = (int)((SCREEN_HEIGHT * SCREEN_SIZE) / 2.0f);
	static const int HORIZON_MIN_HEIGHT = SCREEN_HEIGHT * SCREEN_SIZE - 50;

	int horizonRenderHeight;

	//vertical lines
	SDL_Texture * floor;
	static const int FLOOR_PIXEL_HEIGHT = 400;
	static const int FLOOR_PIXEL_WIDTH = 800;

	static const float HORIZONTAL_SPEED_MAX;
	float horizontalSpeed;
	float previousHorizontalMovePercentage;

	//horizontal lines
	float firstSegmentPositionPercentage;
	static const float SEGMENT_REDUCTION;
	static const float HORIZONTAL_LINES_SPEED;

	static const int nHorizonQuads = 11;
	SDL_Rect horizontalQuads[nHorizonQuads];


private:
	void RenderFloor();
	void RenderVerticalLines();
	void RenderHorizontalLines();
};


