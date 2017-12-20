#pragma once

#include "IDeletable.h"

#include "Vector3.h"
#include "Size2D.h"
#include "Pivot2D.h"
#include "ModuleCollision.h"

class Transform;
class GameEntity;

class Collider :
	public IDeletable
{
	friend ModuleCollision;
public:
	Collider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, GameEntity& owner);

	void Update();


	bool CheckCollision(const Collider& r) const;

public:
	GameEntity * owner;

	ColliderType colliderType;
	Size2D size;
	Pivot2D pivot;

	SDL_Rect rect;
};