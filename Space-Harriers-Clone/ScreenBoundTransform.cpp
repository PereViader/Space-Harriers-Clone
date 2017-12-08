#include "ScreenBoundTransform.h"

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

void ScreenBoundTransform::Move(Vector3 movement)
{
	position = position + movement;
}