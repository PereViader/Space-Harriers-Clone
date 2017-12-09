#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "RectUtil.h"
#include "ModuleFloor.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/bullets.png");

	playerParticlePrototype.anim.frames.push_back({ 3,2,87,56 });
	playerParticlePrototype.sfxId = App->audio->LoadFx("rtype/Laser1.wav");
	playerParticlePrototype.velocity.z = 35;
	playerParticlePrototype.velocity.x = 3;
	playerParticlePrototype.collider = App->collision->AddPrototypeCollider(ColliderType::PlayerParticle,Size2D(87,56),Pivot2D::MIDDLE_CENTER,&playerParticlePrototype);

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleParticles::PreUpdate()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

// Update all particle logic and draw them
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Particle* p = *it;

		p->Update();
		Vector3 screenPosition = p->transform.GetScreenPositionAndDepth();

		float scale = CalculatePercentageOfPositionInFloor(screenPosition.z);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, &p->anim.GetCurrentFrame(), scale, Pivot2D::MIDDLE_CENTER, screenPosition);
		if (p->isFirstFrame)
		{
			App->audio->PlayFx(p->sfxId);
			p->isFirstFrame = false;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, const Vector2& position)
{
	Particle * instance = particle.Clone();
	instance->transform.SetScreenPosition(position);
	instance->collider = App->collision->RegisterPrototypeInstance(instance->collider, instance);
	this->active.push_back(instance);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

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

void Particle::OnCollision(const Collider * own, const Collider * other)
{
	LOG("Particle collision");
}

