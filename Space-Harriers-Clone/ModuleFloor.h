#pragma once

#include "Module.h"
#include "FloorBoundTransform.h"

struct SDL_Texture;

class ModuleFloor :
	public Module
{
	friend class FloorBoundTransform;

public:
	ModuleFloor(bool enabled);
	virtual ~ModuleFloor();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	float GetHorizonRenderHeight() const;
	float GetCurrentHorizonPercentage() const;	

	float GetHorizonPercentageOfPosition(float position) const;
	float GetHorizonDepthForPosition(float position) const;

	float GetCurrentFloorMovement() const;
	float GetRenderYOfHorizontalStripe(int index) const;
	float GetRenderHOfHorizontalStripe(int index) const;

	int GetFurtherHorizontalStripeIndex() const;

public:
	//Horizon
	static const int HORIZON_MAX_HEIGHT = (int)((SCREEN_HEIGHT * SCREEN_SIZE) / 2.0f);
	static const int HORIZON_MIN_HEIGHT = SCREEN_HEIGHT * SCREEN_SIZE - 50;


private:
	float horizonRenderHeight;

	//vertical lines
	SDL_Texture * floor;
	static const int FLOOR_PIXEL_HEIGHT = 400;
	static const int FLOOR_PIXEL_WIDTH = 800;

	static const float HORIZONTAL_SPEED_MAX;
	float horizontalSpeed;
	float horizontalSpeedForOutsideUse;

	float previousHorizontalMovePercentage;

	//horizontal lines
	float firstSegmentPositionPercentage;
	static const float SEGMENT_REDUCTION;
	static const float HORIZONTAL_LINES_SPEED;

	static const int nHorizonQuads = 11;
	int firstQuadIndex;
	SDL_Rect horizontalQuads[nHorizonQuads];


	struct HorizontalSegmentContainer {
		float y;
		float h;
	};

	HorizontalSegmentContainer horizontalSegments[nHorizonQuads];

	//temp
	float positionPercentage;
	int objectQuadIndex;

private:
	void RenderFloor();
	void RenderVerticalLines();
	void RenderHorizontalLines();
};

inline float CalculatePercentageOfPositionInFloor(float zPosition) {
	return 1.0f - zPosition / Z_MAX;
}