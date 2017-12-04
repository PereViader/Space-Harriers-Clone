#pragma once

#include "Vector3.h"

class Transform {
	virtual Vector3 GetScreenPositionAndDepth() const = 0;
};