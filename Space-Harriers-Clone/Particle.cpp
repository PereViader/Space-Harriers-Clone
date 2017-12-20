#include "Particle.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFloor.h"

#include "Collider.h"
#include "Vector3.h"


Particle::Particle(const ColliderType& particleType, const Animation& animation, const Size2D & size, const SFX& sfx, const Texture& graphics) :
	GameEntity(new ScreenBoundTransform()),
	graphics(graphics),
	sfx(sfx),
	animation(animation),
	collider(App->collision->AddPrototypeCollider(particleType, size, Pivot2D::MIDDLE_CENTER, *this)),
	isFirstFrame(true)
{}

Particle::Particle(const Particle& p) :
	GameEntity(p),
	graphics(p.graphics),
	sfx(p.sfx),
	animation(p.animation),
	collider(App->collision->RegisterPrototypeInstance(*p.collider, *this)),
	isFirstFrame(p.isFirstFrame),
	velocity(p.velocity)
{
}

Particle::~Particle()
{
	collider->MarkAsDeleted();
}

void Particle::Update()
{
	if (isFirstFrame)
	{
		App->audio->PlayFx(sfx);
		isFirstFrame = false;
	}

	MoveParticle();

	collider->UpdateValues(GetTransform());
	float depth = GetTransform().GetScreenPositionAndDepth().z;
	SetDeleted(depth > Z_MAX || depth < 0);
}

void Particle::MoveParticle()
{
	float deltaTime = App->time->GetDeltaTime();

	GetTransform().Move(velocity * deltaTime);
}

Particle* Particle::Clone() const
{
	return new Particle(*this);
}

void Particle::Render()
{
	Vector3 screenPosition = GetTransform().GetScreenPositionAndDepth();
	float scale = GetTransform().GetRenderingScale();
	animation.UpdateFrame();
	graphics.UpdateTexture(animation);
	App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, screenPosition);
}

void Particle::SetVelocity(const Vector3 & velocity)
{
	this->velocity = velocity;
}

void Particle::OnCollision(const Collider& own, const Collider& other)
{
	MarkAsDeleted();
}