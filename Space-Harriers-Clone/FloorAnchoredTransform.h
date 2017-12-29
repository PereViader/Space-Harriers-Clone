#pragma once

#include "FloorRelatedTransform.h"

#include "Application.h"
#include "ModuleFloor.h"

#include "Vector2.h"

class FloorAnchoredTransform :
	public FloorRelatedTransform
{
public:
	FloorAnchoredTransform();
	FloorAnchoredTransform(float startingXPositionOffset, float yOffset, float percentageInsideSegment);
	FloorAnchoredTransform(const FloorAnchoredTransform&);
	FloorAnchoredTransform(const Transform&);

	~FloorAnchoredTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual float GetDepth() const override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual void Move(const Vector3& movement) override;

	virtual float GetRenderingScale() const override;

	virtual FloorAnchoredTransform * Clone() const override;

	void ResetPositionToTheHorizon();
	void SetPosition(const Vector3&);
	void SetPosition(const FloorAnchoredTransform&);
private:
	int renderingFloorId;
	int renderingFloorSegmentCount;
	float percentageInsideSegment;
	Vector2 position;

private:
	bool IsBoundSegmentPresentOnScreen() const;
};

