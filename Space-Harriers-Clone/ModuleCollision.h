#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include "ICollidable.h"

// TODO 9: Create a matrix of game specific types of collision for early discard
// Example: lasers should not collide with lasers but should collider with walls
// enemy shots will collide with other enemies ? and against walls ?

struct Collider
{
	SDL_Rect rect;
	bool to_delete;

	// TODO 10: Add a way to notify other classes that a collision happened
	ICollidable& owner;

	Collider(SDL_Rect rectangle, ICollidable& owner) : // expand this call if you need to
		rect(rectangle),
		to_delete(false),
		owner(owner)
	{
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
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

	Collider* AddCollider(const SDL_Rect& rect, ICollidable& owner);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
};

#endif // __ModuleCollision_H__