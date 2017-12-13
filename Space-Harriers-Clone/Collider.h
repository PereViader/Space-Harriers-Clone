#pragma once

#include "ModuleCollision.h"

class Collider
{
public:
	ColliderType colliderType;

	Size2D size;
	Pivot2D pivot;
	Vector3 position;


	SDL_Rect rect;

	bool to_delete;

	ICollidable* owner;

	Collider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable* owner) :
		colliderType(colliderType),
		size(size),
		pivot(pivot),
		rect({ 0,0,static_cast<int>(size.width),static_cast<int>(size.height) }),
		to_delete(false),
		owner(owner),
		position(-1, -1, -1)
	{
	}

	void UpdateValues(const Transform& transform) {
		position = transform.GetScreenPositionAndDepth();
		float scale = transform.GetRenderingScale();
		Size2D currentSize(size.width*scale, size.height*scale);
		rect = GetRectInPositionWithPivot(position, currentSize, pivot);
	}

	bool CheckCollision(const Collider& r) const;
};