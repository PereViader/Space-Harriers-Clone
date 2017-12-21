#include "FloorBasedTransform.h"

#include "Application.h"
#include "ModuleFloor.h"

FloorBasedTransform::FloorBasedTransform()
{
}

FloorBasedTransform::FloorBasedTransform(const Vector3 & position) :
	position(position)
{
}

FloorBasedTransform::~FloorBasedTransform()
{
}

FloorBasedTransform* FloorBasedTransform::Clone() const
{
	return new FloorBasedTransform(*this);
}

Vector3 FloorBasedTransform::GetScreenPositionAndDepth() const
{
	Vector3 floor = GetFloorPositionAndDepth();
	return floor - Vector3(0, position.y, 0)*GetRenderingScale();
}

void FloorBasedTransform::Move(const Vector3 & movement)
{
	position = position + movement;
}

Vector3 FloorBasedTransform::GetFloorPositionAndDepth() const
{
	return Vector3(position.x, App->floor->GetHorizonPositionForDepth(position.z), position.z);
}

float FloorBasedTransform::GetDepth() const
{
	return position.z;
}

float FloorBasedTransform::GetRenderingScale() const
{
	return CalculatePercentageOfPositionInFloor(position.z);
}

void FloorBasedTransform::SetPosition(const Vector3 & newPosition)
{
	position = newPosition;
}
