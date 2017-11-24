#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFloor.h"

Obstacle::Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow, float scalingFactor) :
	graphics(graphics),
	animation(animation),
	hasShadow(hasShadow),
	scalingFactor(scalingFactor)
{
}


Obstacle::~Obstacle()
{
}

Enemy * Obstacle::Clone() const
{
	Obstacle * o = new Obstacle(*this);
	o->renderingFloorId = App->floor->GetFurtherHorizontalStripeIndex();
	return o;
}

void Obstacle::Update()
{
	RenderObstacle();
}

void Obstacle::MoveObstacle()
{
}

void Obstacle::RenderObstacle()
{
	iPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition(screen.y) * scalingFactor;
	App->renderer->BlitWithPivotScaled(graphics, &animation.GetCurrentFrame(), scale, 0.5f, 1.0f, screen.x, screen.y);
}

iPoint Obstacle::GetScreenRenderPosition() const
{
	iPoint screen;
	screen.x = int((SCREEN_WIDTH*SCREEN_SIZE) / 2.0f); //TODO
	screen.y = App->floor->GetRenderHeightOfHorizontalStripe(renderingFloorId);
	App->renderer->DrawQuad({ screen.x, screen.y, 20,20}, 0, 0, 0, 255,false);
	return screen;
}

float Obstacle::GetScaleForPosition(int screenY) const
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}
