#pragma once

#include "Module.h"
#include "Texture.h"

#define Z_MAX 700

class ModuleFloor :
	public Module
{
	friend class FloorAnchoredTransform;

public:
	ModuleFloor(bool enabled);
	virtual ~ModuleFloor();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	float GetHorizonRenderHeight() const;
	float GetCurrentHorizonPercentage() const;	

	float GetHorizonPercentageOfPosition(float position) const;
	float GetHorizonDepthForPosition(float position) const;
	float GetHorizonPositionForDepth(float depth) const;

	float GetCurrentFloorMovement() const;
	float GetRenderYOfHorizontalStripe(int index) const;
	float GetRenderHOfHorizontalStripe(int index) const;

	int GetFurtherHorizontalStripeIndex() const;

	int GetCurrentSegmentCount() const;

public:
	//Horizon
	static const int HORIZON_MAX_HEIGHT = SCREEN_HEIGHT / 2;
	static const int HORIZON_MIN_HEIGHT = SCREEN_HEIGHT - 50;


private:
	static const float HORIZON_MOVEMENT_SMOOTHING;
	float horizonRenderHeight;

	//vertical lines
	Texture floor;
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

	static const int NUMBER_OF_HORIZON_QUADS = 11;
	int firstQuadIndex;
	SDL_Rect horizontalQuads[NUMBER_OF_HORIZON_QUADS];


	struct HorizontalSegmentContainer {
		float y;
		float h;
	};

	HorizontalSegmentContainer horizontalSegments[NUMBER_OF_HORIZON_QUADS];
	int segmentCount;

private:
	void RenderFloor();
	void RenderVerticalLines();
	void RenderHorizontalLines();
};

inline float CalculatePercentageOfPositionInFloor(float zPosition) {
	return 1.0f - zPosition / Z_MAX;
}