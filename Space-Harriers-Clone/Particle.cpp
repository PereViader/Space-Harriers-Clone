#include "Particle.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleFloor.h"
#include "ModuleShadow.h"
#include "ScreenBoundFloorProjectedTransform.h"

#include "Collider.h"
#include "Vector3.h"

const float Particle::MINIMUM_HEIGHT_FOR_SHADOW = SCREEN_HEIGHT - 150;

Particle::Particle(const ColliderType& particleType, const Animation& animation, const Size2D & size, const SFX& sfx, const Texture& graphics) :
	GameEntity(new ScreenBoundFloorProjectedTransform()),
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

	float depth = GetTransform().GetDepth();
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
	
	if (screenPosition.y < MINIMUM_HEIGHT_FOR_SHADOW) {
		App->shadow->DrawShadow(GetTransform());
	}
}

void Particle::SetVelocity(const Vector3 & velocity)
{
	this->velocity = velocity;
}

void Particle::OnCollision(const Collider& own, const Collider& other)
{
	MarkAsDeleted();
}