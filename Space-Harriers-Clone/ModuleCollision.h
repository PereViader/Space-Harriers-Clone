#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include "Module.h"

#include "RectUtil.h"

#include <assert.h>
#include<list>

class Vector3;
class Size2D;
class Pivot2D;
class Transform;
class ICollidable;

class Collider;

enum class ColliderType {
	Enemy = 0,
	Player = 1,
	EnemyParticle = 2,
	PlayerParticle = 3,
	
};

static const bool COLLISION_MATRIX[4][4] =
{
	{ 0,1,0,1 },
	{ 1,0,1,0 },
	{ 0,1,0,0 },
	{ 1,0,0,0 }
};

inline bool DoColliderLayersCollide(const ColliderType& a, const ColliderType& b) {
	return COLLISION_MATRIX[static_cast<int>(a)][static_cast<int>(b)];
}

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	

	bool CleanUp();

	Collider* AddCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner);
	Collider* AddPrototypeCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner);
	Collider* RegisterPrototypeInstance(const Collider & prototype, ICollidable& owner);


private:

	std::list<Collider*> colliders;
	std::list<Collider*> prototypes;
	bool debug = false;

	

private:
	Collider * CreateCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner) const;

	void DoCollisionDetection();
	void DoDebug();
	void DebugDraw();
};

#endif // __ModuleCollision_H__