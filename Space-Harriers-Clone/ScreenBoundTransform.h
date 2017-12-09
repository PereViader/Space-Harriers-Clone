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

	virtual void Move(const Vector3& movement) override;

	virtual float GetRenderingScale() const override;

	virtual ScreenBoundTransform* Clone() const override;

	void SetScreenPosition(const Vector3& position);

private:
	Vector3 position;
};