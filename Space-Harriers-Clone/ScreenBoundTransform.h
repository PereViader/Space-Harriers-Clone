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
	virtual ~ScreenBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;

	virtual void Move(const Vector3& movement) override;

	virtual ScreenBoundTransform* Clone() const override;
	virtual float GetRenderingScale() const override;


	void SetScreenPosition(const Vector3& position);

private:
	Vector3 position;
};