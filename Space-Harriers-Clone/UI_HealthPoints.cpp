#include "UI_HealthPoints.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Pivot2D.h"

#include "SDL\include\SDL.h"
#include "RectUtil.h"

const Vector3 UI_HealthPoints::RENDERING_POSITION(20,SCREEN_HEIGHT - 15,-1);

const Size2D UI_HealthPoints::HEALTH_SIZE(60, 90);

UI_HealthPoints::UI_HealthPoints() :
	previousHealthPoints(App->player->GetHealthPoints()),
	baseGraphics(App->textures->Load("data/sprites/GUI.png"))
	//healthPointsCachedGraphic(BuildTextureForHealthPoints(previousHealthPoints))
{
	baseGraphics.UpdateTexture({ 20,30,10,17 });
}

UI_HealthPoints::~UI_HealthPoints()
{
}

void UI_HealthPoints::Render() {
	Vector3 position = RENDERING_POSITION;
	for (int i = 0; i < App->player->GetHealthPoints(); i++)
	{
		App->renderer->BlitWithPivotScaledZBuffer(baseGraphics, 4, Pivot2D::BOTTOM_LEFT, position);
		position.x += HEALTH_SIZE.width;
	}
}

/*void UI_HealthPoints::Render()
{
	int currentHealthPoints = App->player->GetHealthPoints();
	if (currentHealthPoints != previousHealthPoints) {
		previousHealthPoints = currentHealthPoints;

		UnloadCachedTexture(healthPointsCachedGraphic);
		healthPointsCachedGraphic = BuildTextureForHealthPoints(currentHealthPoints);
	}
	App->renderer->BlitWithPivotScaledZBuffer(healthPointsCachedGraphic, 1, Pivot2D::BOTTOM_LEFT, RENDERING_POSITION);
}*/

/*Texture UI_HealthPoints::BuildTextureForHealthPoints(int healthPoints) const
{
	SDL_Surface * surface = SDL_CreateRGBSurface(0, static_cast<int>(HEALTH_SIZE.width*healthPoints), static_cast<int>(HEALTH_SIZE.height), 32, 0, 0, 0, 255);
	assert(surface != nullptr);
	//SDL_FillRect(surface, NULL, 0xFF00FF);
	//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
	Vector2 position;
	for (int i = 0; i < healthPoints; i++)
	{
		SDL_Rect dstRect = GetRectInPositionWithPivot(position, HEALTH_SIZE, Pivot2D::TOP_LEFT);
		SDL_BlitSurface(baseGraphics.GetTexture(), &baseGraphics.GetSection(), &dstRect);
		position.x += HEALTH_SIZE.width;
	}
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_DestroyRenderer(renderer);
	assert(texture != nullptr);
	SDL_FreeSurface(surface);
	return Texture(texture);
}*/

/*void UI_HealthPoints::UnloadCachedTexture(Texture & texture)
{
	SDL_DestroyTexture(texture.GetTexture());
	texture = Texture();
}*/