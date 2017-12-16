#include "Collider.h"

#include "ModuleCollision.h"
#include "Size2D.h"
#include "Pivot2D.h"
#include "Vector3.h"
#include "Transform.h"

Collider::Collider(const ColliderType& colliderType, const Size2D & size, const Pivot2D & pivot, ICollidable & owner) :
	colliderType(colliderType),
	size(size),
	pivot(pivot),
	rect({ 0,0,static_cast<int>(size.width),static_cast<int>(size.height) }),
	owner(&owner),
	position(-1, -1, -1)
{
}

void Collider::UpdateValues(const Transform & transform)
{
	position = transform.GetScreenPositionAndDepth();
	float scale = transform.GetRenderingScale();
	Size2D currentSize(size.width*scale, size.height*scale);
	rect = GetRectInPositionWithPivot(position, currentSize, pivot);
}

bool Collider::CheckCollision(const Collider& r) const
{
	return DoColliderLayersCollide(colliderType, r.colliderType) && abs(position.z - r.position.z) <= 1 && SDL_HasIntersection(&this->rect, &r.rect);
}