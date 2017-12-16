#include "Particle.h"

#include "Collider.h"

Particle::Particle() :
	to_delete(false),
	isFirstFrame(true),
	velocity(Vector3()),
	collider(nullptr)
{}

Particle::Particle(const Particle& p) :
	to_delete(p.to_delete),
	isFirstFrame(p.isFirstFrame),
	anim(p.anim),
	sfxId(p.sfxId),
	velocity(p.velocity),
	collider(p.collider)
{
}

Particle::~Particle()
{
	collider->to_delete = true;
}

void Particle::Update()
{
	MoveParticle();

	collider->UpdateValues(transform);
	float depth = transform.GetScreenPositionAndDepth().z;
	to_delete = depth > Z_MAX || depth < 0;
}

void Particle::MoveParticle()
{
	float deltaTime = App->time->GetDeltaTime();

	transform.Move(velocity * deltaTime);
}

Particle* Particle::Clone() const
{
	return new Particle(*this);
}

void Particle::OnCollision(const Collider& own, const Collider& other)
{
	to_delete = true;
}