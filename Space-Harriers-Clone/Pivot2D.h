#pragma once

#include "Vector2.h"

class Pivot2D {
public:
	Pivot2D(float x, float y) : x(x), y(y) {}

	float x;
	float y;

	static const Pivot2D TOP_LEFT;
	static const Pivot2D TOP_RIGHT;
	static const Pivot2D TOP_CENTER;
	static const Pivot2D MIDDLE_LEFT;
	static const Pivot2D MIDDLE_CENTER;
	static const Pivot2D MIDDLE_RIGHT;
	static const Pivot2D BOTTOM_LEFT;
	static const Pivot2D BOTTOM_CENTER;
	static const Pivot2D BOTTOM_RIGHT;
};