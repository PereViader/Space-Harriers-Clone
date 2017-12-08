#pragma once

#include "Transform.h"

class ScreenBoundTransform :
	public Transform
{
public:
	ScreenBoundTransform();
	ScreenBoundTransform(Vector3 position);
	~ScreenBoundTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;

	virtual void Move(Vector3 movement) override;
	virtual ScreenBoundTransform* Clone() const override;

	void SetScreenPosition(Vector3 position);

private:
	Vector3 position;
};

