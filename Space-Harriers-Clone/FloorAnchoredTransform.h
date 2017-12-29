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
	void SetPosition(const Transform& other);
	void SetPosition(const FloorAnchoredTransform&);
private:
	//Index in ModuleFloor of the horizontal stripe
	int renderingFloorId;

	// Percentual position of segment height
	float percentageInsideSegment;

	//Spawn ModuleFloor segment count
	int renderingFloorSegmentCount;

	//Horizontal and vertical offsets from the floor position
	Vector2 position;

private:
	bool IsBoundSegmentPresentOnScreen() const;
};

