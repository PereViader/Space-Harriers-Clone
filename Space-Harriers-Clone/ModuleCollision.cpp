#include "ModuleCollision.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Collider.h"
#include "ICollidable.h"

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
		if ((*it)->ToDelete())
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	DoCollisionDetection();
	DoDebug();

	return update_status::UPDATE_CONTINUE;
}

void ModuleCollision::DoDebug()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
		DebugDraw();
}

void ModuleCollision::DoCollisionDetection()
{
	for (list<Collider*>::const_iterator it0 = colliders.cbegin(), end = colliders.cend(); it0 != end; ++it0) {
		for (list<Collider*>::const_iterator it1 = next(it0, 1); it1 != end; ++it1) {
			Collider& c0 = **it0;
			Collider& c1 = **it1;

			assert(!c0.ToDelete());
			assert(!c1.ToDelete());

			if (c0.CheckCollision(c1)) {
				c0.owner->OnCollision(c0, c1);
				c1.owner->OnCollision(c1, c0);
			}
		}
	}
}

void ModuleCollision::DebugDraw()
{
	for (Collider* c : colliders) {
		if (!c->ToDelete()) {
			App->renderer->DrawQuad(c->rect, 255, 0, 0, 80);
		}
	}
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

Collider* ModuleCollision::AddCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner)
{
	Collider* ret = CreateCollider(colliderType,size,pivot, owner);

	colliders.push_back(ret);

	return ret;
}

Collider * ModuleCollision::AddPrototypeCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner)
{
	Collider* ret = CreateCollider(colliderType, size, pivot, owner);

	prototypes.push_back(ret);

	return ret;
}

Collider * ModuleCollision::CreateCollider(const ColliderType& colliderType, const Size2D& size, const Pivot2D& pivot, ICollidable& owner) const {
	return new Collider(colliderType,size,pivot, owner);
}

Collider * ModuleCollision::RegisterPrototypeInstance(const Collider & prototype, ICollidable& owner)
{
	Collider* ret = new Collider(prototype);
	ret->owner = &owner;
	colliders.push_back(ret);
	return ret;
}