#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleFloor.h"
#include "RectUtil.h"
#include "ModuleCollision.h"
#include "FloorBoundTransform.h"

#include <assert.h>

Obstacle::Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow, float scalingFactor) :
	Enemy(new FloorBoundTransform(0,0,0), hasShadow),
	graphics(graphics),
	animation(animation),
	collider(nullptr),
	scalingFactor(scalingFactor)
{
}

Obstacle::Obstacle(const Obstacle & other) :
	Enemy(other.transform->Clone(), other.hasShadow, other.toDelete),
	scalingFactor(other.scalingFactor),
	graphics(other.graphics),
	animation(other.animation),
	collider(other.collider)
{
}


Obstacle::~Obstacle()
{
}

Obstacle * Obstacle::Clone() const
{
	return new Obstacle(*this);
}

void Obstacle::Init(map<string, void*> parameters)
{
	collider = App->collision->RegisterPrototypeInstance(collider, this);
}

void Obstacle::Update()
{
	//Move Obstacle
	Vector3 movement(App->floor->GetCurrentFloorMovement(), 0, 0);
	transform->Move(movement);

	//Update collider
	collider->UpdateValues(*transform);

	//Render
	Vector3 screen = transform->GetScreenPositionAndDepth();
	float scale = scalingFactor * transform->GetRenderingScale();

	SDL_Rect& animationRect = animation.GetCurrentFrame();
	App->renderer->BlitWithPivotScaledZBuffer(graphics, &animationRect, scale, 0.5f, 1.0f, static_cast<int>(screen.x), static_cast<int>(screen.y), screen.z);
}

void Obstacle::OnCollision(const Collider * own, const Collider * other)
{
	assert(own == collider);
	LOG("%s", "enemy collided");
	if (other->colliderType == ColliderType::PlayerParticle) {
		toDelete = true;
		collider->to_delete = true;
	}
}