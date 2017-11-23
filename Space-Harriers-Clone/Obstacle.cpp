#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleFloor.h"

#include "Animation.h"

Obstacle::Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow) :
	graphics(graphics),
	animation(animation),
	hasShadow(hasShadow)
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

void Obstacle::RenderObstacle() const
{
	iPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition(screen.y);
	App->renderer->BlitWithPivotScaled(graphics, nullptr, scale, 0.5f, 1.0f, screen.x, screen.y);
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
