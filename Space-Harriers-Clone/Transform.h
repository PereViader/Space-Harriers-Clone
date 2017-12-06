#pragma once

#include "Vector3.h"

class Transform {
	virtual Vector3 GetScreenPositionAndDepth() const = 0;
	virtual Vector3 Update() = 0;
};