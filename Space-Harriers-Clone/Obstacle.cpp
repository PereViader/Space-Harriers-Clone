#include "Obstacle.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Collider.h"
#include "FloorAnchoredTransform.h"
#include "ModuleEnemy.h"
#include "Explosion.h"

#include <assert.h>

Obstacle::Obstacle(const Texture& graphics, const Animation& animation, bool hasShadow, const Size2D& size, float scalingFactor, bool isNonDamaging) :
	Enemy(new FloorAnchoredTransform(0,0,0), hasShadow),
	graphics(graphics),
	animation(animation),
	collider(App->collision->AddPrototypeCollider(isNonDamaging? ColliderType::NonDamagingEnemy : ColliderType::Enemy, size, Pivot2D::BOTTOM_CENTER, *this)),
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
	collider->MarkAsDeleted();
}

Obstacle * Obstacle::Clone() const
{
	return new Obstacle(*this);
}

void Obstacle::Init(const json& parameters)
{
	GetTransformTypped<FloorAnchoredTransform>().ResetPositionToTheHorizon();

	Vector3 horizontalPosition = parameters["position"];
	GetTransform().Move(horizontalPosition);
}

void Obstacle::Update()
{
	GetTransform().Move(GetMovement());
	
	if (GetTransform().GetDepth() <= 0) {
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
	MarkAsDeleted();

	Explosion * explosion = static_cast<Explosion*>(App->enemies->InstantiateEnemyByName("explosion", json()));
	explosion->GetTransform().SetPosition(GetTransform());
}
