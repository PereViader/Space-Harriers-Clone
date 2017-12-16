#pragma once

#include "FloorRelatedTransform.h"

#include "Application.h"
#include "ModuleFloor.h"

class FloorAnchoredTransform :
	public FloorRelatedTransform
{
public:
	FloorAnchoredTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment);
	FloorAnchoredTransform(const FloorAnchoredTransform&);
	FloorAnchoredTransform(const Transform&);

	~FloorAnchoredTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual void Move(const Vector3& movement) override;

	virtual float GetRenderingScale() const override;

	virtual FloorAnchoredTransform * Clone() const override;

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

