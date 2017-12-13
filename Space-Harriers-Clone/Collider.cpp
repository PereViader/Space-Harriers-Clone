#include "Collider.h"

inline bool DoColliderLayersCollide(const Collider& a, const Collider& b) {
	return collisionMatrix[static_cast<int>(a.colliderType)][static_cast<int>(b.colliderType)];
}

bool Collider::CheckCollision(const Collider& r) const
{
	return DoColliderLayersCollide(*this, r) && abs(position.z - r.position.z) <= 1 && SDL_HasIntersection(&this->rect, &r.rect);
}