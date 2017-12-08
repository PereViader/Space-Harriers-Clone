#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include <assert.h>

#include <iterator>     // std::advance

using namespace std;

ModuleCollision::ModuleCollision()
{
}

ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	for(list<Collider*>::const_iterator it0 = colliders.cbegin(), end = colliders.cend(); it0 != end; it0++) 
		for (list<Collider*>::const_iterator it1 = next(it0, 1); it1 != end; it1++)
			if ((*it0)->CheckCollision(**it1)) {
				(*it0)->owner->OnCollision(*it0, *it1);
				(*it1)->owner->OnCollision(*it1, *it0);
			}


	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
}

bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);
	colliders.clear();

	for (list<Collider*>::iterator it = prototypes.begin(); it != prototypes.end(); ++it)
		RELEASE(*it);
	prototypes.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner)
{
	Collider* ret = CreateCollider(colliderType, width, height, xPivot, yPivot, owner);

	colliders.push_back(ret);

	return ret;
}

Collider * ModuleCollision::AddPrototypeCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner)
{
	Collider* ret = CreateCollider(colliderType, width, height, xPivot, yPivot, owner);

	prototypes.push_back(ret);

	return ret;
}

Collider * ModuleCollision::CreateCollider(ColliderType colliderType, float width, float height, float xPivot, float yPivot, ICollidable* owner) const {
	return new Collider(colliderType, width, height, xPivot, yPivot, owner);
}

Collider * ModuleCollision::RegisterPrototypeInstance(Collider * prototype, ICollidable * owner)
{
	assert(prototype != nullptr);
	Collider* ret = new Collider(*prototype);
	ret->owner = owner;
	colliders.push_back(ret);
	return ret;
}



// -----------------------------------------------------

inline bool DoColliderLayersCollide(const Collider& a, const Collider& b) {
	return collisionMatrix[static_cast<int>(a.colliderType)][static_cast<int>(b.colliderType)];
}

bool Collider::CheckCollision(const Collider& r) const
{
	//TODO Improve call to filter collision layers
	return DoColliderLayersCollide(*this, r) && SDL_HasIntersection(&this->rect, &r.rect);
}