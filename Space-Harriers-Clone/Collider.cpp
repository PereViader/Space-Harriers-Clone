#include "Collider.h"

#include "ModuleCollision.h"
#include "Size2D.h"
#include "Pivot2D.h"
#include "Vector3.h"
#include "Transform.h"
#include "GameEntity.h"

Collider::Collider(const ColliderType& colliderType, const Size2D & size, const Pivot2D & pivot, GameEntity& owner) :
	colliderType(colliderType),
	size(size),
	pivot(pivot),
	rect({ 0,0,static_cast<int>(size.width),static_cast<int>(size.height) }),
	owner(&owner)
{
}
/*
void Collider::UpdateValues(const Transform & transform)
{
	float scale = transform.GetRenderingScale();
	Size2D currentSize(size.width*scale, size.height*scale);
	rect = GetRectInPositionWithPivot(position, currentSize, pivot);
}*/

void Collider::Update()
{
	Vector3 position = owner->GetTransform().GetScreenPositionAndDepth();
	float scale = owner->GetTransform().GetRenderingScale();
	Size2D currentSize(size.width*scale, size.height*scale);
	rect = GetRectInPositionWithPivot(position, currentSize, pivot);
}

bool Collider::CheckCollision(const Collider& r) const
{
	float thisZ = owner->GetTransform().GetDepth();
	float otherZ = r.owner->GetTransform().GetDepth();
	return DoColliderLayersCollide(colliderType, r.colliderType) && abs(thisZ - otherZ) <= 28 && SDL_HasIntersection(&this->rect, &r.rect);
}