#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "ICollidable.h"
#include "RectUtil.h"
#include <assert.h>


#include "Vector3.h"
#include "Size2D.h"
#include "Pivot2D.h"
#include "Transform.h"
#include "ModuleFloor.h"

enum class ColliderType {
	Enemy = 0,
	Player = 1,
	EnemyParticle = 2,
	PlayerParticle = 3,
	
};

static const bool collisionMatrix[4][4] =
{
	{0,1,0,1},
	{1,0,1,0},
	{0,1,0,0},
	{1,0,0,0}
};

struct Collider
{
	ColliderType colliderType;

	Size2D size;
	Pivot2D pivot;

	SDL_Rect rect;

	bool to_delete;

	ICollidable* owner;

	Collider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable* owner) :
		colliderType(colliderType),
		size(size),
		pivot(pivot),
		rect({0,0,static_cast<int>(size.width),static_cast<int>(size.height)}),
		to_delete(false),
		owner(owner)
	{
	}

	void UpdateValues(const Transform& transform) {
		Vector3 position = transform.GetScreenPositionAndDepth();
		float scale = transform.GetRenderingScale();
		Size2D currentSize(size.width*scale, size.height*scale);
		rect = GetRectInPositionWithPivot(position,currentSize,pivot);
	}

	bool CheckCollision(const Collider& r) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable* owner);
	Collider* AddPrototypeCollider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable* owner);

	Collider* RegisterPrototypeInstance(Collider* prototype, ICollidable * owner);

	void DebugDraw();

private:

	std::list<Collider*> colliders;
	std::list<Collider*> prototypes;
	bool debug = false;

private:
	Collider * CreateCollider(ColliderType colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable* owner) const;
};

#endif // __ModuleCollision_H__