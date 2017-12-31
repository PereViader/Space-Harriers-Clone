#include "ShieldedOvni.h"

#include "FloorBasedTransform.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "Collider.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "Explosion.h"
#include "ModuleEnemy.h"

ShieldedOvni::ShieldedOvni(float speed, float projectileSpeed, const Texture & graphics, const Animation & animation, const Size2D & size, float scalingFactor, float timeOpen, float timeClosed, int stateSwitchesToLeave) :
	Enemy(new FloorBasedTransform(),true),
	state(behaviour_state::In),
	graphics(graphics),
	animationOpenClose(animation),
	scalingFactor(scalingFactor),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	isOpen(false),
	stateSwitchesToLeave(stateSwitchesToLeave),
	nStateSwitches(0),
	currentTime(0),
	timeOpen(timeOpen),
	timeClosed(timeClosed),
	speed(speed),
	nextPositionIndex(-1),
	projectileSpeed(projectileSpeed)
{
}

ShieldedOvni::ShieldedOvni(const ShieldedOvni & o) :
	Enemy(o),
	state(o.state),
	graphics(o.graphics),
	animationOpenClose(o.animationOpenClose),
	scalingFactor(o.scalingFactor),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	isOpen(o.isOpen),
	stateSwitchesToLeave(o.stateSwitchesToLeave),
	nStateSwitches(o.nStateSwitches),
	currentTime(o.currentTime),
	timeOpen(o.timeOpen),
	timeClosed(o.timeClosed),
	path(o.path),
	nextPositionIndex(o.nextPositionIndex),
	speed(o.speed),
	projectileSpeed(o.projectileSpeed)
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

void ShieldedOvni::Init(const json& parameters)
{
	Vector3 position = parameters["position"];
	GetTransform().Move(position);
}

void ShieldedOvni::Update()
{
	switch (state)
	{
	case behaviour_state::In: {
		if (nextPositionIndex < path.size()) {
			const Vector3& position = GetTransform().GetPosition();
			const Vector3& destination = path.at(nextPositionIndex);
			Vector3 newPosition = MoveTowards(position, destination, speed * App->time->GetDeltaTime());
			GetTransform().SetPosition(newPosition);
			if (newPosition == destination) {
				nextPositionIndex++;
			}
		}
		else {
			SwitchState();
			state = behaviour_state::Shoot;
			nextPositionIndex--;
		}
		break;
	}
		
	case behaviour_state::Shoot: {
		currentTime += App->time->GetDeltaTime();

		float TIME_CHECK = isOpen ? timeOpen : timeClosed;

		if (currentTime >= TIME_CHECK) {
			currentTime -= TIME_CHECK;
			SwitchState();
		}

		if (nStateSwitches >= stateSwitchesToLeave) {
			state = behaviour_state::Out;
		}
		break;
	}
		
	case behaviour_state::Out: {
		const Vector3& position = GetTransform().GetPosition();
		const Vector3& destination = path.at(nextPositionIndex);
		Vector3 newPosition = MoveTowards(position, destination, speed * App->time->GetDeltaTime());
		GetTransform().SetPosition(newPosition);
		if (newPosition == destination) {
			if (nextPositionIndex == 0) {
				MarkAsDeleted();
			}
			else {
				nextPositionIndex--;
			}
		}
		break;
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

void ShieldedOvni::SwitchState()
{
	isOpen = !isOpen;
	animationOpenClose.speed *= -1;

	if (isOpen) {
		ShootPlayer();
	}

	nStateSwitches++;
}

void ShieldedOvni::SetPath(const vector<Vector3>& path)
{
	this->path = path;
	GetTransform().SetPosition(path.front());
	nextPositionIndex = 1;
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

	Explosion * explosion = static_cast<Explosion*>(App->enemies->InstantiateEnemyByName("explosion", json()));
	explosion->GetTransform().SetPosition(GetTransform());
}
