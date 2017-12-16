#include "FloorAnchoredTransform.h"

#include <assert.h>

FloorAnchoredTransform::FloorAnchoredTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment) :
	xPositionOffset(startingXPositionOffset),
	percentageInsideSegment(percentageInsideSegment),
	renderingFloorId(App->floor->GetFurtherHorizontalStripeIndex()),
	renderingFloorSegmentCount(App->floor->segmentCount),
	yOffset(yOffset)
{
}

FloorAnchoredTransform::FloorAnchoredTransform(const FloorAnchoredTransform & o) :
	xPositionOffset(o.xPositionOffset),
	percentageInsideSegment(o.percentageInsideSegment),
	renderingFloorId(o.renderingFloorId),
	renderingFloorSegmentCount(o.renderingFloorSegmentCount),
	yOffset(o.yOffset)
{
}

FloorAnchoredTransform::FloorAnchoredTransform(const Transform& t)
{
	assert(false); //TODO
}

FloorAnchoredTransform::~FloorAnchoredTransform()
{
}

inline float GetScaleForPosition(float screenY)
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}

Vector3 FloorAnchoredTransform::GetScreenPositionAndDepth() const
{
	Vector3 position_scale = GetFloorPositionAndDepth();

	if (IsBoundSegmentPresentOnScreen()) {
		position_scale.y -= yOffset * CalculatePercentageOfPositionInFloor(position_scale.z);
	}
	return position_scale;
}

inline Vector3 FloorAnchoredTransform::GetFloorPositionAndDepth() const
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

FloorAnchoredTransform * FloorAnchoredTransform::Clone() const
{
	return new FloorAnchoredTransform(*this);
}

void FloorAnchoredTransform::ResetPositionToTheHorizon()
{
	renderingFloorId = App->floor->GetFurtherHorizontalStripeIndex();
	renderingFloorSegmentCount = App->floor->segmentCount;
}

bool FloorAnchoredTransform::IsBoundSegmentPresentOnScreen() const
{
	return renderingFloorSegmentCount + App->floor->nHorizonQuads > App->floor->segmentCount;
}

float FloorAnchoredTransform::GetRenderingScale() const
{
	float scale;
	if (IsBoundSegmentPresentOnScreen())
		scale = App->floor->GetHorizonPercentageOfPosition(GetFloorPositionAndDepth().y);
	else
		scale = 0;
	return scale;
}

void FloorAnchoredTransform::Move(const Vector3& movement)
{
	xPositionOffset += movement.x;
	yOffset += movement.y;
}
