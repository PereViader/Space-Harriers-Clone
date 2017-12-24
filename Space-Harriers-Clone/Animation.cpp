#include "Animation.h"

Animation::Animation() :
	loop(true),
	speed(1.0f),
	current_frame(0.0f),
	loops(0)
{}

void Animation::UpdateFrame() {
	/*float last_frame = (float)frames.size();

	current_frame += speed*App->time->GetDeltaTime();
	if (current_frame >= last_frame)
	{
	current_frame = (loop) ? 0.0f : max(last_frame - 1.0f, 0.0f);
	loops++;
	}*/

	float maxFrameIndex = frames.size() - 1.0f;

	assert(current_frame >= 0 && current_frame <= maxFrameIndex);

	current_frame += speed * App->time->GetDeltaTime();

	if (current_frame < 0 || current_frame > maxFrameIndex) {
		if (loop) {
			float newFrame = fmod(current_frame, maxFrameIndex);
			if (newFrame != current_frame) 
				loops++;
			current_frame = newFrame;
			assert(current_frame >= 0 && current_frame <= maxFrameIndex);

		}
		else {
			current_frame = min(maxFrameIndex, max(0.0f, current_frame));
			assert(current_frame >= 0 && current_frame <= maxFrameIndex);

		}
	}

	assert(current_frame >= 0 && current_frame <= maxFrameIndex);

}

const SDL_Rect& Animation::GetFrame() const
{
	return frames[(int)current_frame];
}

const SDL_Rect& Animation::UpdateAndGetFrame()
{
	UpdateFrame();
	return GetFrame();
}

bool Animation::Finished() const
{
	return loop ? loops > 0 : current_frame == frames.size() - 1.0f;
}

void Animation::Reset()
{
	current_frame = 0.0f;
	loops = 0;
}