#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleFloor.h"
#include "RectUtil.h"
#include "ModuleCollision.h"

Obstacle::Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow, float scalingFactor) :
	Enemy(hasShadow,false),
	graphics(graphics),
	animation(animation),
	scalingFactor(scalingFactor),
	collider()
{
	collider = App->collision->AddCollider(animation.frames[0], *App->enemies);
}


Obstacle::~Obstacle()
{
}

Enemy * Obstacle::Clone() const
{
	Obstacle * o = new Obstacle(*this);
	return o;
}

void Obstacle::Init(map<string, void*> parameters)
{
	renderingFloorId = App->floor->GetFurtherHorizontalStripeIndex();
}

void Obstacle::Update()
{
	iPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition(screen.y) * scalingFactor;
	App->renderer->BlitWithPivotScaled(graphics, &animation.GetCurrentFrame(), scale, 0.5f, 1.0f, screen.x, screen.y);

	collider->rect = GetRectInPositionWithPivot(screen.x, screen.y, animation.PeakCurrentFrame().w * scale, animation.PeakCurrentFrame().h * scale, 0.5f, 1.0f);
}

void Obstacle::MoveObstacle()
{
	iPoint screen = GetScreenRenderPosition();
	collider->rect = GetRectInPositionWithPivot(screen.x, screen.y, (float)collider->rect.w, (float)collider->rect.h, 0.5f, 1.0f);
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
	App->renderer->DrawQuad({ screen.x, screen.y, 20,20}, 0, 0, 0, 255);
	return screen;
}

float Obstacle::GetScaleForPosition(int screenY) const
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}


