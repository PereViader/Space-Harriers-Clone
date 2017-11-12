#pragma once

#include "Module.h"

struct SDL_Texture;

class ModuleScene :
	public Module
{
public:
	ModuleScene(bool enabled);
	~ModuleScene();
	 
	bool Init() override;
	update_status Update() override;
	
private:
	SDL_Texture * background;

	float currentHorizontalOffset;

	const float SEGMENT_WIDTH = 20;
	const float SEGMENT_DEPTH = 20;
	float HORIZON_DISTANCE = SEGMENT_DEPTH * 13;
	float SCREEN_DISTANCE = SEGMENT_WIDTH * 24;
	float HORIZON_HEIGHT = (float)(SCREEN_HEIGHT * SCREEN_SIZE)/2.0f;
	SDL_Color colors[2][2] = { {{255,26,26,255},{255,153,153,255} },{ {26,102,255,255},{153,187,255,255} } };


	float GetDepthOfPixelRow(int row) const;
	const SDL_Color& GetColorForPixelWithDepth(int pixel, float depth, float depthPercent) const;


	int NormalizeXPixel(int x) const;
	int NormalizeYPixel(int y) const;


	void ProjectPixelToWorldPosition(int normalizedX, int normalizedY, float& worldX, float& worldZ) const;
	const SDL_Color& GetColorForPosition(float worldX,float worldZ);
};

