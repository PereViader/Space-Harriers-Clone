#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL\include\SDL.h"

#include <iostream>

using namespace std;

ModuleScene::ModuleScene(bool enabled) :
	Module(enabled)
{
}


ModuleScene::~ModuleScene()
{
}
   


bool ModuleScene::Init()
{
	background = App->textures->Load("rtype/background.bmp");
	return true;
}

update_status ModuleScene::Update()
{
	SDL_Rect rect = { 0,0,SCREEN_WIDTH*SCREEN_SIZE,SCREEN_HEIGHT*SCREEN_SIZE };
	App->renderer->DirectBlit(background, NULL, &rect);
	/*for (int y = SCREEN_HEIGHT * SCREEN_SIZE -1; y >= HORIZON_HEIGHT; y--) {
		int normalizedY = -y + SCREEN_HEIGHT * SCREEN_SIZE-1;
		float worldZ = GetDepthOfPixelRow(normalizedY);
		float depthPercent = worldZ / HORIZON_DISTANCE;
		for (int x = 0; x < SCREEN_WIDTH * SCREEN_SIZE; x++) {
			const SDL_Color& color = GetColorForPixelWithDepth(x, worldZ, depthPercent);
			App->renderer->DrawPixel(x, y, color.r, color.g, color.b, color.a);
		}
	}*/

	
	/*for (int y = SCREEN_HEIGHT * SCREEN_SIZE - 1; y >= HORIZON_HEIGHT; y--) {
		int normalizedY = NormalizeYPixel(y);
		for (int x = 0; x < SCREEN_WIDTH * SCREEN_SIZE; x++) {
			int normalizedX = NormalizeXPixel(x);
			float worldX, worldZ;
			ProjectPixelToWorldPosition(normalizedX, normalizedY, worldX, worldZ);
			const SDL_Color& color = GetColorForPosition(worldX, worldZ);
			App->renderer->DrawPixel(x, y, color.r, color.g, color.b, color.a);
		}
	}*/




	return UPDATE_CONTINUE;
}

int ModuleScene::NormalizeXPixel(int x) const
{
	return -(SCREEN_WIDTH * SCREEN_SIZE)/2 + x;

}

int ModuleScene::NormalizeYPixel(int y)const
{
	return -y + SCREEN_HEIGHT * SCREEN_SIZE - 1;
}

void ModuleScene::ProjectPixelToWorldPosition(int normalizedX, int normalizedY, float & worldX, float & worldZ) const
{
	worldZ = (HORIZON_DISTANCE*((float)normalizedY)) / HORIZON_HEIGHT;
	worldX = (float)normalizedX*((float)(SCREEN_WIDTH*SCREEN_SIZE)/ (float)SCREEN_DISTANCE)*(worldZ/HORIZON_DISTANCE);
}

const SDL_Color & ModuleScene::GetColorForPosition(float worldX, float worldZ)
{
	worldX = worldX < 0 ? -worldX : worldX;
	int colorX = (int)(worldX / SEGMENT_WIDTH) % 2;
	int colorY = (int)(worldZ / SEGMENT_DEPTH) % 2;
	return colors[colorX][colorY];
}

const SDL_Color & ModuleScene::GetColorForPixelWithDepth(int pixel, float depth, float depthPercent) const
{
	int colorX = (int)((float)pixel*depthPercent / SEGMENT_WIDTH) % 2;
	int colorY = (int)((float)depth*depthPercent / SEGMENT_DEPTH) % 2;
	return colors[colorX][colorY];
}



float ModuleScene::GetDepthOfPixelRow(int row) const
{
	return (HORIZON_DISTANCE*((float)row))/HORIZON_HEIGHT;
}