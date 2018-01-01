#pragma once
#include "Module.h"
#include "SDL\include\SDL.h"

#include "Texture.h"

class ModuleBackground :
	public Module
{
public:
	ModuleBackground(bool enabled);
	~ModuleBackground();

	bool Start() override;
	update_status Update() override;

	void NextLevel();
	void SetLevel(int level);

private:
	static const float BACKGROUND_HORIZONTAL_SPEED;
	static const float HORIZON_DECAL_HORIZONTAL_SPEED;

	static const float BACKGROUND_SCALE;
	static const float DECAL_SCALE;


	static const int NUMBER_OF_LEVELS = 1;
	Texture horizonDecal[NUMBER_OF_LEVELS];
	Texture background[NUMBER_OF_LEVELS];

	int currentLevel;
	float currentLevelBackgroundTextureWidth;
	float currentLevelBackgroundTextureHeight;

	float currentLevelHorizonDecalTextureWidth;
	float currentLevelHorizonDecalTextureHeight;


	float backgroundTextureOffset;
	float decalTextureOffset;
private:

	void DrawBackground();
	void DrawDecal();

	void MoveBackground();
	void MoveDecal();
};

