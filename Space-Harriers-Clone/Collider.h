#pragma once

#include "IDeletable.h"

#include "Vector3.h"
#include "Size2D.h"
#include "Pivot2D.h"
#include "ModuleCollision.h"

class Transform;
enum class ColliderType;
class ICollidable;

class Collider :
	public IDeletable
{
public:
	Collider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner);

	void UpdateValues(const Transform& transform);

	bool CheckCollision(const Collider& r) const;

public:
	ColliderType colliderType;
	Size2D size;
	Pivot2D pivot;
	Vector3 position;
	ICollidable* owner;
	SDL_Rect rect;
};