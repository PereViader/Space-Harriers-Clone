#pragma once

#include "SDL/include/SDL.h"

inline SDL_Rect GetRectInPositionWithPivot(int x, int y, float width, float height, float pivotX, float pivotY) {
	SDL_Rect rectForPivot;
	rectForPivot.w = (int)(width);
	rectForPivot.h = (int)(height);
	rectForPivot.x = (int)(x - (width*pivotX));
	rectForPivot.y = (int)(y - (height*pivotY));
	return rectForPivot;
}