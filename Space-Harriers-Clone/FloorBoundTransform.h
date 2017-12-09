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
	virtual void Move(const Vector3& movement) override;

	virtual float GetRenderingScale() const override;

	virtual FloorBoundTransform * Clone() const override;

	void ResetPositionToTheHorizon();
private:
	int renderingFloorId;
	int renderingFloorSegmentCount;
	float percentageInsideSegment;
	float xPositionOffset;
	float yOffset;

private:
	bool IsBoundSegmentPresentOnScreen() const;
};

