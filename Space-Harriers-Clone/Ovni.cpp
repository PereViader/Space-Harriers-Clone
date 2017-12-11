#include "Ovni.h"

#include "ScreenBoundTransform.h"
#include "Application.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "Pivot2D.h"
#include "ModuleShadow.h"
#include "ModuleCollision.h"

Ovni::Ovni(const Vector3 startingPosition, float speed, const Texture& texture, const Animation& animation, const vector<Vector3>& path, float scalingFactor) :
	Enemy(new ScreenBoundTransform(startingPosition),true),
	collider(nullptr),
	speed(speed),
	graphics(texture),
	animation(animation),
	scalingFactor(scalingFactor),
	currentTarget(0),
	path(path)
{
}

Ovni::Ovni(const Ovni& other) :
	Enemy(other.transform->Clone(), true),
	collider(other.collider),
	speed(other.speed),
	graphics(other.graphics),
	animation(other.animation),
	scalingFactor(other.scalingFactor),
	currentTarget(other.currentTarget),
	path(other.path) 
{
}


Ovni::~Ovni()
{
}

void Ovni::Init(map<string, void*> values)
{
	App->collision->RegisterPrototypeInstance(collider, this);
}

void Ovni::Update()
{
	collider->UpdateValues(*transform);

	float deltaTime = App->time->GetDeltaTime();
	Vector3 newPosition = MoveTowards(transform->GetScreenPositionAndDepth(), path[currentTarget], speed*deltaTime);
	static_cast<ScreenBoundTransform*>(transform)->SetScreenPosition(newPosition);
	if (newPosition == path[currentTarget]) {
		currentTarget += 1;
		if (currentTarget == path.size()) {
			//toDelete = true;
			currentTarget = 0;
		}
	}
}

void Ovni::OnCollision(const Collider * own, const Collider * other)
{
	LOG("OVNI COLLIDED");
}

Ovni* Ovni::Clone() const
{
	return new Ovni(*this);
}

void Ovni::Render()
{
	//TODO Implement the part of hasshadow inside enemy
	if (!toDelete) {
		Vector3 screen = transform->GetScreenPositionAndDepth();
		float scale = scalingFactor * transform->GetRenderingScale();
		animation.UpdateFrame();
		graphics.UpdateTexture(animation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::BOTTOM_CENTER, screen);
		App->shadow->DrawShadow(*transform);
	}
}
