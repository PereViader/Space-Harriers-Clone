#pragma once

#include "SDL/include/SDL.h"
#include "Pivot2D.h"
#include "Size2D.h"

inline SDL_Rect GetRectInPositionWithPivot(const Vector2& position, const Size2D& size, const Pivot2D& pivot) {
	SDL_Rect rectForPivot;
	rectForPivot.w = static_cast<int>(size.width);
	rectForPivot.h = static_cast<int>(size.height);
	rectForPivot.x = static_cast<int>(position.x - (size.width*pivot.x));
	rectForPivot.y = static_cast<int>(position.y - (size.height*pivot.y));
	return rectForPivot;
}