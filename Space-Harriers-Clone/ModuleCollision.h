#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "ICollidable.h"
#include "RectUtil.h"
#include <assert.h>


#include "Vector3.h"
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

	float width;
	float height;

	float xPivot;
	float yPivot;

	SDL_Rect rect;

	bool to_delete;

	ICollidable* owner;

	Collider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner) :
		colliderType(colliderType),
		width(width),
		height(height),
		xPivot(xPivot),
		yPivot(yPivot),
		rect({0,0,static_cast<int>(width),static_cast<int>(height)}),
		to_delete(false),
		owner(owner)
	{
	}

	void UpdateValues(const Transform& transform) {
		Vector3 position = transform.GetScreenPositionAndDepth();
		float scale = CalculatePercentageOfPositionInFloor(position.z);
		rect = GetRectInPositionWithPivot(static_cast<int>(position.x), static_cast<int>(position.y), width*scale, height*scale, xPivot, yPivot);
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

	Collider* AddCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner);
	Collider* AddPrototypeCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner);

	Collider* RegisterPrototypeInstance(Collider* prototype, ICollidable * owner);

	void DebugDraw();

private:

	std::list<Collider*> colliders;
	std::list<Collider*> prototypes;
	bool debug = false;

private:
	Collider * CreateCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner) const;
};

#endif // __ModuleCollision_H__