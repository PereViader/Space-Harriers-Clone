#include "ShieldedOvni.h"

#include "FloorBasedTransform.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "Collider.h"
#include "ModuleRender.h"


ShieldedOvni::ShieldedOvni(float speed, const Texture & graphics, const Animation & animation, const Size2D & size, float scalingFactor) :
	Enemy(new FloorBasedTransform(),true),
	graphics(graphics),
	animationOpenClose(animation),
	scalingFactor(scalingFactor),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	isOpen(false),
	speed(speed)
{
}

ShieldedOvni::ShieldedOvni(const ShieldedOvni & o) :
	Enemy(o),
	graphics(o.graphics),
	animationOpenClose(o.animationOpenClose),
	scalingFactor(o.scalingFactor),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	isOpen(o.isOpen),
	path(o.path),
	speed(o.speed)
{
}

ShieldedOvni::~ShieldedOvni()
{
	collider->MarkAsDeleted();
}

void ShieldedOvni::OnCollision(const Collider & own, const Collider & other)
{
	if (isOpen) {
  		MarkAsDeleted();
	}
}

void ShieldedOvni::Init(map<string, void*> values)
{
}

void ShieldedOvni::Update()
{
	if (path.size() > 0) {
		Vector3 position = GetTransform().GetPosition();
		Vector3 newPosition = MoveTowards(position, path.front(), speed * App->time->GetDeltaTime());
		GetTransform().SetPosition(newPosition);
		if (newPosition == path.front()) {
			path.pop_front();
		}
	}
}

Enemy * ShieldedOvni::Clone() const
{
	return new ShieldedOvni(*this);
}

void ShieldedOvni::Render()
{
	Enemy::Render();

	animationOpenClose.UpdateFrame();
	graphics.UpdateTexture(animationOpenClose);

	Vector3 position = GetTransform().GetScreenPositionAndDepth();
	float scale = GetTransform().GetRenderingScale() * scalingFactor;

	App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, position);
}

void ShieldedOvni::SetOpen(bool state)
{
	if (state != isOpen) {
		animationOpenClose.speed *= -1;
		isOpen = state;
	}
}


#include <iostream>

void ShieldedOvni::SetPath(const list<Vector3>& path)
{
	using namespace std;
	this->path = path;
	cout << path.front().x << "," << path.front().y << "," << path.front().z << endl;
	GetTransform().SetPosition(path.front());
	this->path.pop_front();
}
