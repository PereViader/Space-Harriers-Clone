#include "FloorAnchoredTransform.h"

#include <assert.h>

FloorAnchoredTransform::FloorAnchoredTransform() :
	position(0,0),
	percentageInsideSegment(0),
	renderingFloorId(-1),
	renderingFloorSegmentCount(-1)
{
}

FloorAnchoredTransform::FloorAnchoredTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment) :
	position(startingXPositionOffset,yOffset),
	percentageInsideSegment(percentageInsideSegment),
	renderingFloorId(App->floor->GetFurtherHorizontalStripeIndex()),
	renderingFloorSegmentCount(App->floor->segmentCount)
{
}

FloorAnchoredTransform::FloorAnchoredTransform(const FloorAnchoredTransform & o) :
	percentageInsideSegment(o.percentageInsideSegment),
	renderingFloorId(o.renderingFloorId),
	renderingFloorSegmentCount(o.renderingFloorSegmentCount),
	position(o.position)
{
}

FloorAnchoredTransform::FloorAnchoredTransform(const Transform& t)
{
	SetPosition(t);
}

FloorAnchoredTransform::~FloorAnchoredTransform()
{
}

Vector3 FloorAnchoredTransform::GetScreenPositionAndDepth() const
{
	Vector3 position_scale = GetFloorPositionAndDepth();

	if (IsBoundSegmentPresentOnScreen()) {
		position_scale.y -= position.y * CalculatePercentageOfPositionInFloor(position_scale.z);
	}
	return position_scale;
}

float FloorAnchoredTransform::GetDepth() const
{
	return GetFloorPositionAndDepth().z;
}

inline float GetScaleForPosition(float screenY)
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}

inline Vector3 FloorAnchoredTransform::GetFloorPositionAndDepth() const
{
	Vector3 position_scale;

	if (IsBoundSegmentPresentOnScreen()) {
		position_scale.y = App->floor->horizontalSegments[renderingFloorId].y + App->floor->horizontalSegments[renderingFloorId].h * percentageInsideSegment;
		position_scale.x = (SCREEN_WIDTH) / 2.0f + position.x * GetScaleForPosition(position_scale.y);
		position_scale.z = App->floor->GetHorizonDepthForPosition(position_scale.y);
	}
	else {
		position_scale.y = SCREEN_HEIGHT;
		position_scale.x = (SCREEN_WIDTH) / 2.0f + position.x * GetScaleForPosition(position_scale.y);
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

void FloorAnchoredTransform::SetPosition(const Transform& other)
{
	Vector3 screenPosition = other.GetScreenPositionAndDepth();
	// Find Base Y 
	float yForCurrentZ = App->floor->GetHorizonPositionForDepth(screenPosition.z);

	// Find rendering floor id that has yForCurrentZ 
	int newRenderingFloorId = App->floor->NUMBER_OF_HORIZON_QUADS - 1;
	for (int i = 1; i < App->floor->NUMBER_OF_HORIZON_QUADS; ++i) {
		if (App->floor->horizontalSegments[i].y < yForCurrentZ) {
			newRenderingFloorId = i-1;
			break;
		}
	}

	auto horizontalSemgent = App->floor->horizontalSegments[newRenderingFloorId];
	float newPercentageInsideSegment = (yForCurrentZ - horizontalSemgent.y) / horizontalSemgent.h;
	int newRenderingFloorSegmentCount = App->floor->segmentCount;

	Vector2 newPosition;
	newPosition.x = (screenPosition.x - SCREEN_WIDTH / 2.0f) / CalculatePercentageOfPositionInFloor(screenPosition.z);
	newPosition.y = (yForCurrentZ - screenPosition.y) / CalculatePercentageOfPositionInFloor(screenPosition.z);

	this->renderingFloorId = newRenderingFloorId;
	this->percentageInsideSegment = newPercentageInsideSegment;
	this->renderingFloorSegmentCount = newRenderingFloorSegmentCount;
	this->position = newPosition;
}

void FloorAnchoredTransform::SetPosition(const FloorAnchoredTransform & other)
{
	this->renderingFloorId = other.renderingFloorId;
	this->renderingFloorSegmentCount = other.renderingFloorSegmentCount;
	this->percentageInsideSegment = other.percentageInsideSegment;
	this->position = other.position;
}

bool FloorAnchoredTransform::IsBoundSegmentPresentOnScreen() const
{
	return renderingFloorSegmentCount + App->floor->NUMBER_OF_HORIZON_QUADS > App->floor->segmentCount;
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
	position += movement;
	percentageInsideSegment += movement.z;
}
