#pragma once

#include "Transform.h"

class ScreenBoundTransform :
	public Transform
{
public:
	ScreenBoundTransform();
	ScreenBoundTransform(Vector3 position);
	~ScreenBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual ScreenBoundTransform* Clone() const override;

	Vector3 position;
};

