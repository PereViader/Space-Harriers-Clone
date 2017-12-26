#include "ShieldedOvni.h"

#include "FloorBasedTransform.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "Collider.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "ShieldedOvniBrain.h"


ShieldedOvni::ShieldedOvni(float speed, float projectileSpeed, const Texture & graphics, const Animation & animation, const Size2D & size, float scalingFactor) :
	Enemy(new FloorBasedTransform(),true),
	graphics(graphics),
	animationOpenClose(animation),
	scalingFactor(scalingFactor),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	isOpen(false),
	speed(speed),
	projectileSpeed(projectileSpeed),
	owner(nullptr)
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
	speed(o.speed),
	projectileSpeed(o.projectileSpeed),
	owner(o.owner)
{
}

ShieldedOvni::~ShieldedOvni()
{ 
	collider->MarkAsDeleted();
}

void ShieldedOvni::OnCollision(const Collider & own, const Collider & other)
{
	if (isOpen) {
		OnShieldedOvniDied();
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

	if (isOpen) {
		ShootPlayer();
	}
}

void ShieldedOvni::SetPath(const list<Vector3>& path)
{
	using namespace std;
	this->path = path;
	GetTransform().SetPosition(path.front());
	this->path.pop_front();
}

void ShieldedOvni::SetShieldedOvniBrain(ShieldedOvniBrain & shieldedOvniBrain)
{
	owner = &shieldedOvniBrain;
}

void ShieldedOvni::ShootPlayer()
{
	Vector3 playerPosition = App->player->GetChestPosition();
	Vector3 ownPosition = GetTransform().GetScreenPositionAndDepth();
	Vector3 direction = playerPosition - ownPosition;
	Vector3 velocity = direction.Normalized() * projectileSpeed;

	App->particles->AddParticleByName("ovni", ownPosition, velocity);
}

void ShieldedOvni::OnShieldedOvniDied()
{
	MarkAsDeleted();

	if (owner)
		owner->OnShieldedOvniDied(*this);
}
