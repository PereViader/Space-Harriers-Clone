#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include <algorithm>
#include <assert.h>

#include "Application.h"
#include "ModuleTime.h"

using namespace std;

class Animation
{
public:
	Animation();

	void UpdateFrame();
	const SDL_Rect& GetFrame() const;
	const SDL_Rect& UpdateAndGetFrame();

	bool Finished() const;
	void Reset();

public:
	bool loop;
	float speed;
	vector<SDL_Rect> frames;

private:
	float current_frame;
	int loops;
};

#endif // !_ANIMATION_H_