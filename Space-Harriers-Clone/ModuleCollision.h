#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "ICollidable.h"
#include "RectUtil.h"
#include <assert.h>
#include "Vector3.h"

// TODO 9: Create a matrix of game specific types of collision for early discard
// Example: lasers should not collide with lasers but should collider with walls
// enemy shots will collide with other enemies ? and against walls ?

struct Collider
{
	SDL_Rect rect;
	bool to_delete;

	ICollidable* owner;

	Collider(SDL_Rect rectangle, ICollidable* owner) : 
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

	Collider* AddCollider(const SDL_Rect& rect, ICollidable* owner);
	Collider* AddPrototypeCollider(ICollidable* owner);

	Collider* RegisterPrototypeInstance(Collider* prototype, ICollidable * owner);

	void DebugDraw();

private:

	std::list<Collider*> colliders;
	std::list<Collider*> prototypes;
	bool debug = false;
};

#endif // __ModuleCollision_H__