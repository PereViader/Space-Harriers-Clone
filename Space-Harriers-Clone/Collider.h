#pragma once

#include "ModuleCollision.h"

class Collider
{
public:
	Collider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner);

	void UpdateValues(const Transform& transform);

	bool CheckCollision(const Collider& r) const;

public:
	ColliderType colliderType;
	Size2D size;
	Pivot2D pivot;
	Vector3 position;
	ICollidable* owner;
	SDL_Rect rect;
	bool to_delete;
};