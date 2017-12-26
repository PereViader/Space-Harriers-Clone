#ifndef _SFX_H_
#define _SFX_H_

struct Mix_Chunk;

class SFX
{
public:
	SFX() : SFX(nullptr) {}
	SFX(Mix_Chunk* sfx) : sfx(sfx) {}

	Mix_Chunk* GetSfx() const { return sfx; }

private:
	Mix_Chunk* sfx;
};

#endif // !_SFX_H_
