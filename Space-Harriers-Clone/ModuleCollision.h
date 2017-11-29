#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "ICollidable.h"
#include "RectUtil.h"
#include <assert.h>
#include "Vector3.h"

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

	SDL_Rect rect;
	bool to_delete;

	ICollidable* owner;

	Collider(SDL_Rect rectangle, ICollidable* owner, ColliderType colliderType) :
		colliderType(colliderType),
		rect(rectangle),
		to_delete(false),
		owner(owner)
	{
	}

	void UpdateValues(const Vector3& position, float xPivot, float yPivot, float width, float height) {
		assert(xPivot >= 0.0f && yPivot >= 0.0f && xPivot <= 1.0f && yPivot <= 1.0f);
		rect = GetRectInPositionWithPivot(static_cast<int>(position.x), static_cast<int>(position.y), width, height, xPivot, yPivot);
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

	Collider* AddCollider(const SDL_Rect& rect, ICollidable* owner, ColliderType colliderType);
	Collider* AddPrototypeCollider(ICollidable* owner, ColliderType colliderType);

	Collider* RegisterPrototypeInstance(Collider* prototype, ICollidable * owner);

	void DebugDraw();

private:

	std::list<Collider*> colliders;
	std::list<Collider*> prototypes;
	bool debug = false;
};

#endif // __ModuleCollision_H__