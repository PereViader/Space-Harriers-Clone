#pragma once

#include "Transform.h"

class ScreenBoundTransform :
	public Transform
{
public:
	ScreenBoundTransform();
	ScreenBoundTransform(const ScreenBoundTransform&);
	ScreenBoundTransform(const Transform&);
	ScreenBoundTransform(Vector3 position);
	~ScreenBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual Vector3 GetFloorPositionAndDepth() const override;

	virtual void Move(Vector3 movement) override;

	virtual float GetRenderingScale() override;

	virtual ScreenBoundTransform* Clone() const override;

	void SetScreenPosition(Vector3 position);

private:
	Vector3 position;
};