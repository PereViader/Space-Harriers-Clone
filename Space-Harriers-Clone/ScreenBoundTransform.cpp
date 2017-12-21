#include "ScreenBoundTransform.h"

#include "ModuleFloor.h"

ScreenBoundTransform::ScreenBoundTransform() 
{
}

ScreenBoundTransform::ScreenBoundTransform(const ScreenBoundTransform & o) :
	position(o.position)
{
}

ScreenBoundTransform::ScreenBoundTransform(const Transform & o) :
	position(o.GetScreenPositionAndDepth())
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

float ScreenBoundTransform::GetDepth() const
{
	return position.z;
}

ScreenBoundTransform* ScreenBoundTransform::Clone() const
{
	return new ScreenBoundTransform(*this);
}

void ScreenBoundTransform::SetScreenPosition(const Vector3& position)
{
	this->position = position;
}

float ScreenBoundTransform::GetRenderingScale() const
{
	return CalculatePercentageOfPositionInFloor(position.z);
}

void ScreenBoundTransform::Move(const Vector3& movement)
{
	position = position + movement;
}