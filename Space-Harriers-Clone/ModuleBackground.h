#pragma once
#include "Module.h"
#include "SDL\include\SDL.h"

class ModuleBackground :
	public Module
{
public:
	ModuleBackground(bool enabled);
	~ModuleBackground();

	bool Start() override;
	update_status Update() override;

	void DrawBackground();

	void MoveBackground();


	void NextLevel();

	void SetLevel(int level);

private:
	static const float BACKGROUND_HORIZONTAL_SPEED;
	static const float HORIZON_DECAL_HORIZONTAL_SPEED;

	static const int BACKGROUND_TEXTURE_HORIZONTAL_RENDERING_WINDOW;

	static const int NUMBER_OF_LEVELS = 1;
	SDL_Texture * horizonDecal[NUMBER_OF_LEVELS];
	SDL_Texture * background[NUMBER_OF_LEVELS];

	int currentLevel;
	int currentLevelBackgroundTextureWidth;
	int currentLevelBackgroundTextureHeight;

	int currentLevelHorizonDecalTextureWidth;
	int currentLevelHorizonDecalTextureHeight;


	int backgroundTextureOffset;

private:
	bool DoesBackgroundNeed2Blits() const;

};

