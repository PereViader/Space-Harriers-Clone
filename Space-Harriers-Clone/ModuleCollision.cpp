#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

#include <iterator>     // std::advance

using namespace std;

ModuleCollision::ModuleCollision()
{
}

// Destructor
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
				(*it0)->owner.OnCollision(*it0, *it1);
				(*it1)->owner.OnCollision(*it1, *it0);
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

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, ICollidable& owner)
{
	Collider* ret = new Collider(rect, owner);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const Collider& r) const
{
	return SDL_HasIntersection(&this->rect,&r.rect);
}
