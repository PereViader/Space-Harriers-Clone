#pragma once

#include "SDL/include/SDL.h"
#include "Pivot2D.h"
#include "Size2D.h"

inline SDL_Rect GetRectInPositionWithPivot(const Vector2& position, const Size2D& size, const Pivot2D& pivot) {
	SDL_Rect rectForPivot;
	rectForPivot.w = (int)(size.width);
	rectForPivot.h = (int)(size.height);
	rectForPivot.x = (int)(position.x - (size.width*pivot.x));
	rectForPivot.y = (int)(position.y - (size.height*pivot.y));
	return rectForPivot;
}