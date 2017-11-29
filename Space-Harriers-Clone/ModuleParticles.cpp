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
	playerParticlePrototype.velocityZ = 35;
	playerParticlePrototype.velocityX = 3;
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
		float scale = 1.0f - p->positionZ / Z_MAX;
		App->renderer->BlitWithPivotScaledZBuffer(graphics, &p->anim.GetCurrentFrame(), scale, 0.5f, 0.5f, (int)p->position.x, (int)p->position.y, p->positionZ);
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
	instance->position = { (float)x, (float)y };
	instance->collider = App->collision->RegisterPrototypeInstance(instance->collider, instance);
	this->active.push_back(instance);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle() :
	to_delete(false),
	isFirstFrame(true),
	position({0,0}),
	positionZ(0),
	velocityX(0),
	velocityY(0),
	velocityZ(0),
	collider(nullptr)
{}

Particle::Particle(const Particle& p) : 
	to_delete(p.to_delete),
	isFirstFrame(p.isFirstFrame),
	anim(p.anim), 
	sfxId(p.sfxId),
	velocityX(p.velocityX),
	velocityY(p.velocityY),
	velocityZ(p.velocityZ),
	position(p.position),
	positionZ(p.positionZ),
	collider(p.collider)
{
}

Particle::~Particle()
{
}

void Particle::Update()
{
	MoveParticle();

	float scale = 1.0f - positionZ / Z_MAX;
	collider->rect = GetRectInPositionWithPivot(position.x, position.y, 50 * scale, 50 * scale, 0.5f, 0.5f);

	to_delete = positionZ > Z_MAX || positionZ < 0;
}

void Particle::MoveParticle()
{
	float deltaTime = App->time->GetDeltaTime();

	position.x += velocityX * deltaTime;
	position.y += velocityY * deltaTime;
	positionZ += velocityZ * deltaTime;
}

void Particle::OnCollision(const Collider * own, const Collider * other)
{
	LOG("Particle collision");
}

