#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Collider.h"
#include "FloorAnchoredTransform.h"

#include <assert.h>

Obstacle::Obstacle(const Texture& graphics, const Animation& animation, bool hasShadow, const Size2D& size, float scalingFactor) :
	Enemy(new FloorAnchoredTransform(0,0,0), hasShadow),
	graphics(graphics),
	animation(animation),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::BOTTOM_CENTER, *this)),
	scalingFactor(scalingFactor)
{
}


//TODO copy constructor

Obstacle::~Obstacle()
{
	collider->MarkAsDeleted();
}

Obstacle * Obstacle::Clone() const
{
	Obstacle* o = new Obstacle(*this);
	o->collider = App->collision->RegisterPrototypeInstance(*o->collider, *o);

	static_cast<FloorAnchoredTransform&>(o->GetTransform()).ResetPositionToTheHorizon();
	return o;
}

void Obstacle::Init(map<string, void*> parameters)
{
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

void Obstacle::OnCollision(const Collider& own, const Collider& other)
{
	assert(&own == collider);
	LOG("%s", "enemy collided");
	if (other.colliderType == ColliderType::PlayerParticle) {
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
