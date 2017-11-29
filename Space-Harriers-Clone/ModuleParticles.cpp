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

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/bullets.png");

	playerParticlePrototype.anim.frames.push_back({ 3,2,87,56 });
	playerParticlePrototype.sfxId = App->audio->LoadFx("rtype/Laser1.wav");
	playerParticlePrototype.velocity.z = 35;
	playerParticlePrototype.velocity.x = 3;
	playerParticlePrototype.collider = App->collision->AddPrototypeCollider(&playerParticlePrototype);

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};

	return true;
}

// Unload assets
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
		float scale = 1.0f - p->position.z / Z_MAX;
		App->renderer->BlitWithPivotScaledZBuffer(graphics, &p->anim.GetCurrentFrame(), scale, 0.5f, 0.5f, (int)p->position.x, (int)p->position.y, p->position.z);
		if (p->isFirstFrame)
		{
			App->audio->PlayFx(p->sfxId);
			p->isFirstFrame = false;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	Particle * instance = new Particle(particle);
	instance->position = Vector3(static_cast<float>(x), static_cast<float>(y));
	instance->collider = App->collision->RegisterPrototypeInstance(instance->collider, instance);
	this->active.push_back(instance);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle() :
	to_delete(false),
	isFirstFrame(true),
	position(Vector3()),
	velocity(Vector3()),
	collider(nullptr)
{}

Particle::Particle(const Particle& p) : 
	to_delete(p.to_delete),
	isFirstFrame(p.isFirstFrame),
	anim(p.anim), 
	sfxId(p.sfxId),
	velocity(p.velocity),
	position(p.position),
	collider(p.collider)
{
}

Particle::~Particle()
{
}

void Particle::Update()
{
	MoveParticle();

	float scale = 1.0f - position.z / Z_MAX;
	collider->rect = GetRectInPositionWithPivot(static_cast<int>(position.x), static_cast<int>(position.y), 50 * scale, 50 * scale, 0.5f, 0.5f);

	to_delete = position.z > Z_MAX || position.z < 0;
}

void Particle::MoveParticle()
{
	float deltaTime = App->time->GetDeltaTime();

	position = position + velocity * deltaTime;
}

void Particle::OnCollision(const Collider * own, const Collider * other)
{
	LOG("Particle collision");
}

