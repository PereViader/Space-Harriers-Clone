#include "FloorBoundTransform.h"

#include <assert.h>

FloorBoundTransform::FloorBoundTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment) :
	xPositionOffset(startingXPositionOffset),
	percentageInsideSegment(percentageInsideSegment),
	renderingFloorId(App->floor->GetFurtherHorizontalStripeIndex()),
	renderingFloorSegmentCount(App->floor->segmentCount),
	yOffset(yOffset)
{
}

FloorBoundTransform::FloorBoundTransform(const FloorBoundTransform & o) :
	xPositionOffset(o.xPositionOffset),
	percentageInsideSegment(o.percentageInsideSegment),
	renderingFloorId(o.renderingFloorId),
	renderingFloorSegmentCount(o.renderingFloorSegmentCount),
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
	Vector3 position_scale = GetFloorPositionAndDepth();

	if (IsBoundSegmentPresentOnScreen()) {
		position_scale.y -= yOffset * CalculatePercentageOfPositionInFloor(position_scale.z);
	}
	return position_scale;
}

inline Vector3 FloorBoundTransform::GetFloorPositionAndDepth() const
{
	Vector3 position_scale;

	if (IsBoundSegmentPresentOnScreen()) {
		position_scale.y = App->floor->horizontalSegments[renderingFloorId].y + App->floor->horizontalSegments[renderingFloorId].h * percentageInsideSegment;
		position_scale.x = (SCREEN_WIDTH) / 2.0f + xPositionOffset * GetScaleForPosition(position_scale.y);
		position_scale.z = App->floor->GetHorizonDepthForPosition(position_scale.y);
	}
	else {
		position_scale.y = SCREEN_HEIGHT;
		position_scale.x = (SCREEN_WIDTH) / 2.0f + xPositionOffset * GetScaleForPosition(position_scale.y);
		position_scale.z = -1;
	}

	return position_scale;
}

FloorBoundTransform * FloorBoundTransform::Clone() const
{
	return new FloorBoundTransform(*this);
}

void FloorBoundTransform::ResetPositionToTheHorizon()
{
	renderingFloorId = App->floor->GetFurtherHorizontalStripeIndex();
	renderingFloorSegmentCount = App->floor->segmentCount;
}

bool FloorBoundTransform::IsBoundSegmentPresentOnScreen() const
{
	return renderingFloorSegmentCount + App->floor->nHorizonQuads > App->floor->segmentCount;
}

float FloorBoundTransform::GetRenderingScale() const
{
	float scale;
	if (IsBoundSegmentPresentOnScreen())
		scale = App->floor->GetHorizonPercentageOfPosition(GetFloorPositionAndDepth().y);
	else
		scale = 0;
	return scale;
}

void FloorBoundTransform::Move(const Vector3& movement)
{
	xPositionOffset += movement.x;
	yOffset += movement.y;
}
