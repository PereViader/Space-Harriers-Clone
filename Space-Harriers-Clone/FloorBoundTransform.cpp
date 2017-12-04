#include "FloorBoundTransform.h"

#include <assert.h>

FloorBoundTransform::FloorBoundTransform(float startingXPositionOffset, float percentageInsideSegment) :
	xPositionOffset(startingXPositionOffset),
	percentageInsideSegment(percentageInsideSegment),
	renderingFloorId(App->floor->GetFurtherHorizontalStripeIndex())
{
}

FloorBoundTransform::FloorBoundTransform(const Transform& t)
{
	assert(false); //TODO
}


FloorBoundTransform::~FloorBoundTransform()
{
}

inline float GetScaleForPosition(float screenY)
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}

Vector3 FloorBoundTransform::GetScreenPositionAndDepth() const
{
	Vector3 position_scale;
	position_scale.y = App->floor->horizontalSegments[renderingFloorId].y + App->floor->horizontalSegments[renderingFloorId].h * percentageInsideSegment;
	position_scale.x = (SCREEN_WIDTH*SCREEN_SIZE) / 2.0f + xPositionOffset * GetScaleForPosition(position_scale.y);
	position_scale.z = App->floor->GetHorizonDepthForPosition(position_scale.y);
	return position_scale;
}