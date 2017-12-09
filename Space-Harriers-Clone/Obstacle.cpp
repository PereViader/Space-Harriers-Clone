#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleFloor.h"
#include "RectUtil.h"
#include "ModuleCollision.h"
#include "ModuleShadow.h"
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
	collider->to_delete = true;
}

Obstacle * Obstacle::Clone() const
{
	Obstacle* o = new Obstacle(*this);
	static_cast<FloorBoundTransform*>(o->transform)->ResetPositionToTheHorizon();
	return o;
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
	
	if (transform->GetFloorPositionAndDepth().z <= 0) {
		toDelete = true;
	}
}

void Obstacle::OnCollision(const Collider * own, const Collider * other)
{
	assert(own == collider);
	LOG("%s", "enemy collided");
	if (other->colliderType == ColliderType::PlayerParticle) {
		toDelete = true;
	}
}

void Obstacle::Render()
{
	if (!toDelete) {
		Vector3 screen = transform->GetScreenPositionAndDepth();
		LOG("%f, %f, %f", screen.x, screen.y, screen.z);
		float scale = scalingFactor * transform->GetRenderingScale();
		SDL_Rect& animationFrame = animation.GetCurrentFrame();

		App->renderer->BlitWithPivotScaledZBuffer(graphics, &animationFrame, scale, Pivot2D::BOTTOM_CENTER, screen);
		App->shadow->DrawShadow(*transform);
	}
}
