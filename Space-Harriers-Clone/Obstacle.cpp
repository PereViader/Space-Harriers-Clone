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

Obstacle::Obstacle(const Obstacle & o) :
	Enemy(o),
	graphics(o.graphics),
	animation(o.animation),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	scalingFactor(o.scalingFactor)
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
	GetTransformTypped<FloorAnchoredTransform>().ResetPositionToTheHorizon();
}

void Obstacle::Update()
{
	GetTransform().Move(GetMovement());
	
	if (GetTransform().GetFloorPositionAndDepth().z <= 0) {
		OnObstacleDied();
	}
}

void Obstacle::OnCollision(const Collider& own, const Collider& other)
{
	assert(&own == collider);
	if (other.colliderType == ColliderType::PlayerParticle) {
		OnObstacleDied();
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

Vector3 Obstacle::GetMovement() const
{
	return Vector3(App->floor->GetCurrentFloorMovement(), 0, 0);
}

void Obstacle::OnObstacleDied()
{
	collider->MarkAsDeleted();
	MarkAsDeleted();
}
