#include "ScreenBoundTransform.h"

#include "ModuleFloor.h"

ScreenBoundTransform::ScreenBoundTransform() 
{
}

ScreenBoundTransform::ScreenBoundTransform(Vector3 position) :
	position(position)
{
}

ScreenBoundTransform::~ScreenBoundTransform()
{
}

Vector3 ScreenBoundTransform::GetScreenPositionAndDepth() const
{
	return position;
}

ScreenBoundTransform* ScreenBoundTransform::Clone() const
{
	return new ScreenBoundTransform(*this);
}

void ScreenBoundTransform::SetScreenPosition(Vector3 position)
{
	this->position = position;
}

Vector3 ScreenBoundTransform::GetFloorPositionAndDepth() const
{
	return Vector3();
}

float ScreenBoundTransform::GetRenderingScale()
{
	return CalculatePercentageOfPositionInFloor(position.z);
}

void ScreenBoundTransform::Move(Vector3 movement)
{
	position = position + movement;
}
