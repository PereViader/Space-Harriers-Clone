#pragma once

#include "IClonable.h"

#include "Vector3.h"


class Transform : public IClonable<Transform*> {
public:
	virtual Vector3 GetScreenPositionAndDepth() const = 0;
	virtual void Move(Vector3 movement) = 0;
};