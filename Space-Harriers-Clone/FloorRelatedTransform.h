#pragma once
#include "Transform.h"
class FloorRelatedTransform :
	public Transform
{
public:
	virtual Vector3 GetFloorPositionAndDepth() const = 0;
	virtual FloorRelatedTransform* Clone() const override = 0;
};