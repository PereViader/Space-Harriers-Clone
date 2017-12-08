#include "FloorBoundTransform.h"

#include <assert.h>

FloorBoundTransform::FloorBoundTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment) :
	xPositionOffset(startingXPositionOffset),
	percentageInsideSegment(percentageInsideSegment),
	renderingFloorId(App->floor->GetFurtherHorizontalStripeIndex()),
	yOffset(yOffset)
{
}

FloorBoundTransform::FloorBoundTransform(const FloorBoundTransform & o) :
	xPositionOffset(o.xPositionOffset),
	percentageInsideSegment(o.percentageInsideSegment),
	renderingFloorId(o.renderingFloorId),
	yOffset(o.yOffset)
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

	position_scale.y += yOffset * CalculatePercentageOfPositionInFloor(position_scale.z);

	return position_scale;
}

FloorBoundTransform * FloorBoundTransform::Clone() const
{
	return new FloorBoundTransform(*this);
}

void FloorBoundTransform::Move(Vector3 movement)
{
	xPositionOffset += movement.x;
}
