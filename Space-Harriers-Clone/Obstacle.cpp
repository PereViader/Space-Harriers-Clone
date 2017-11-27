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
	collider(nullptr),
	scalingFactor(scalingFactor),
	zSpeed(0),
	renderingFloorId(-1),
	xPositionOffset(0)
{
}

Obstacle::Obstacle(const Obstacle & other) :
	Enemy(other.hasShadow, other.toDelete),
	scalingFactor(other.scalingFactor),
	graphics(other.graphics),
	animation(other.animation),
	collider(nullptr),
	zSpeed(other.zSpeed),
	renderingFloorId(-1),
	xPositionOffset(other.xPositionOffset)
{
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
	collider = App->collision->AddCollider(animation.frames[0], *App->enemies);
	xPositionOffset = 0;
	renderingFloorId = App->floor->GetFurtherHorizontalStripeIndex();
}

void Obstacle::Update()
{
	xPositionOffset += App->floor->GetCurrentFloorMovement();
	iPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition((float)screen.y) * scalingFactor;

	SDL_Rect& animationRect = animation.GetCurrentFrame();
	
	// Move collider
	collider->rect = GetRectInPositionWithPivot(screen.x, screen.y, animationRect.w * scale, animationRect.h * scale, 0.5f, 1.0f);

	//Render
	float zValue = App->floor->GetHorizonDepthForPosition(screen.y);
	App->renderer->BlitWithPivotScaledZBuffer(graphics, &animationRect, scale, 0.5f, 1.0f, screen.x, screen.y, zValue);
}

void Obstacle::MoveObstacle()
{
	iPoint screen = GetScreenRenderPosition();
	collider->rect = GetRectInPositionWithPivot(screen.x, screen.y, (float)collider->rect.w, (float)collider->rect.h, 0.5f, 1.0f);
}

void Obstacle::RenderObstacle()
{
	iPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition((float)screen.y) * scalingFactor;
	App->renderer->BlitWithPivotScaled(graphics, &animation.GetCurrentFrame(), scale, 0.5f, 1.0f, screen.x, screen.y);
}

iPoint Obstacle::GetScreenRenderPosition() const
{
	iPoint screen;
	screen.y = App->floor->GetRenderHeightOfHorizontalStripe(renderingFloorId);
	screen.x = (SCREEN_WIDTH*SCREEN_SIZE) / 2.0f + xPositionOffset * GetScaleForPosition((float)screen.y);
	return screen;
}

float Obstacle::GetScaleForPosition(float screenY) const
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}


