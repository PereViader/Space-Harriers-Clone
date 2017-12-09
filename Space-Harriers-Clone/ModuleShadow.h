#pragma once

#include "Module.h"

#include "SDL/include/SDL.h"

class Transform;

class ModuleShadow :
	public Module
{
public:
	ModuleShadow();
	~ModuleShadow();
	bool Start() override;
	void DrawShadow(const Transform&);

	bool CleanUp() override;
private:
	SDL_Texture* shadow;
	SDL_Rect shadowRect;
};