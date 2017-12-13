#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"
#include "ModuleFloor.h"
#include "RectUtil.h"
#include "ModuleCollision.h"
#include "ModuleShadow.h"
#include "Collider.h"
#include "FloorBoundTransform.h"

#include <assert.h>

Obstacle::Obstacle(const Texture& graphics, const Animation& animation, bool hasShadow, float scalingFactor) :
	Enemy(new FloorBoundTransform(0,0,0), hasShadow),
	graphics(graphics),
	animation(animation),
	collider(nullptr),
	scalingFactor(scalingFactor)
{
}

Obstacle::~Obstacle()
{
	collider->to_delete = true;
}

Obstacle * Obstacle::Clone() const
{
	Obstacle* o = new Obstacle(*this);
	static_cast<FloorBoundTransform&>(o->GetTransform()).ResetPositionToTheHorizon();
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
	GetTransform().Move(movement);

	//Update collider
	collider->UpdateValues(GetTransform());
	
	if (GetTransform().GetFloorPositionAndDepth().z <= 0) {
		MarkAsDeleted();
	}
}

void Obstacle::OnCollision(const Collider * own, const Collider * other)
{
	assert(own == collider);
	LOG("%s", "enemy collided");
	if (other->colliderType == ColliderType::PlayerParticle) {
		MarkAsDeleted();
	}
}

void Obstacle::Render()
{
	Enemy::Render();
	if (!ToDelete()) {
		Vector3 screen = GetTransform().GetScreenPositionAndDepth();
		float scale = scalingFactor * GetTransform().GetRenderingScale();
		animation.UpdateFrame();
		graphics.UpdateTexture(animation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::BOTTOM_CENTER, screen);
	}
}
