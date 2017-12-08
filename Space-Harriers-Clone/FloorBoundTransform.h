#pragma once

#include "Transform.h"

#include "Application.h"
#include "ModuleFloor.h"

class FloorBoundTransform :
	public Transform
{
public:
	FloorBoundTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment);
	FloorBoundTransform(const FloorBoundTransform&);
	FloorBoundTransform(const Transform&);

	~FloorBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual void Move(Vector3 movement) override;

	virtual float GetRenderingScale() override;

	virtual FloorBoundTransform * Clone() const override;
private:
	int renderingFloorId;
	float percentageInsideSegment;
	float xPositionOffset;
	float yOffset;
};

