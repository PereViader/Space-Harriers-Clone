#pragma once
#include <vector>
#include <algorithm>
#include "Application.h"
#include "ModuleTime.h"

using namespace std;

class Animation
{
public:
	bool loop;
	float speed;
	vector<SDL_Rect> frames;

private:
	float current_frame;
	int loops;

public:
	Animation() : 
		loop(true),
		speed(1.0f),
		current_frame(0.0f),
		loops(0)
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames)
	{}

	void UpdateFrame() {
		float last_frame = (float)frames.size();

		current_frame += speed*App->time->GetDeltaTime();
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : max(last_frame - 1.0f, 0.0f);
			loops++;
		}
	}

	const SDL_Rect& GetFrame() const
	{
		return frames[(int)current_frame];
	}

	const SDL_Rect& UpdateAndGetFrame()
	{
		UpdateFrame();
		return GetFrame();
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};