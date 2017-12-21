#include "ScreenBoundFloorProjectedTransform.h"

#include "ModuleFloor.h"

ScreenBoundFloorProjectedTransform::ScreenBoundFloorProjectedTransform() 
{
}

ScreenBoundFloorProjectedTransform::ScreenBoundFloorProjectedTransform(const Vector3 & position) :
	position(position)
{
}

ScreenBoundFloorProjectedTransform::~ScreenBoundFloorProjectedTransform()
{
}

Vector3 ScreenBoundFloorProjectedTransform::GetScreenPositionAndDepth() const
{
	return position;
}

float ScreenBoundFloorProjectedTransform::GetDepth() const
{
	return position.z;
}

float ScreenBoundFloorProjectedTransform::GetRenderingScale() const
{
	return CalculatePercentageOfPositionInFloor(position.z);
}

void ScreenBoundFloorProjectedTransform::Move(const Vector3 & movement)
{
	position += movement;
}

Vector3 ScreenBoundFloorProjectedTransform::GetFloorPositionAndDepth() const
{
	return Vector3(position.x, App->floor->GetHorizonPositionForDepth(position.z), position.z);
}

ScreenBoundFloorProjectedTransform * ScreenBoundFloorProjectedTransform::Clone() const
{
	return new ScreenBoundFloorProjectedTransform(*this);
}
