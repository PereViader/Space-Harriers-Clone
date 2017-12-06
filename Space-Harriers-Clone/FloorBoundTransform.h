#pragma once

#include "Transform.h"

#include "Application.h"
#include "ModuleFloor.h"

class FloorBoundTransform :
	public Transform
{
public:
	FloorBoundTransform(float startingXPositionOffset, float percentageInsideSegment);
	FloorBoundTransform(const Transform&);


	~FloorBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
private:
	int renderingFloorId;
	float percentageInsideSegment;
	float xPositionOffset;
};

