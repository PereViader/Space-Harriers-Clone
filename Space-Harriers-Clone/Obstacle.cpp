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

#include <iostream>

void Obstacle::Update()
{
	xPositionOffset += App->floor->GetCurrentFloorMovement();
	fPoint screen = GetScreenRenderPosition();
	float scale = GetScaleForPosition(screen.y) * scalingFactor;
	
	using namespace std;
	cout << scale << endl;

	SDL_Rect& animationRect = animation.GetCurrentFrame();
	
	// Move collider
	collider->rect = GetRectInPositionWithPivot(static_cast<int>(screen.x), static_cast<int>(screen.y), animationRect.w * scale, animationRect.h * scale, 0.5f, 1.0f);

	//Render
	float zValue = App->floor->GetHorizonDepthForPosition(screen.y);
	App->renderer->BlitWithPivotScaledZBuffer(graphics, &animationRect, scale, 0.5f, 1.0f, static_cast<int>(screen.x), static_cast<int>(screen.y), zValue);
}

fPoint Obstacle::GetScreenRenderPosition() const
{
	fPoint screen;
	screen.y = App->floor->GetRenderHeightOfHorizontalStripe(renderingFloorId);
	screen.x = (SCREEN_WIDTH*SCREEN_SIZE) / 2.0f + xPositionOffset * GetScaleForPosition(screen.y);
	return screen;
}

float Obstacle::GetScaleForPosition(float screenY) const
{
	return App->floor->GetHorizonPercentageOfPosition(screenY);
}


