#include "ModuleParticles.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Particle.h"
#include "Collider.h"

#include <math.h>

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("data/bullets.png");

	Animation playerParticleAnimation;
	playerParticleAnimation.frames.push_back({ 3,2,87,56 });
	SFX playerSFX = App->audio->LoadFx("data/Laser1.wav");
	Size2D size(87, 56);
	Particle* playerParticlePrototype = new Particle(ColliderType::PlayerParticle,playerParticleAnimation,size,playerSFX,graphics);
	particlePrototypes["player"] = playerParticlePrototype;

	Animation ovniParticleAnimation;
	ovniParticleAnimation.frames.push_back({ 3,2,87,56 });
	SFX ovniSFX = App->audio->LoadFx("data/Laser1.wav");
	Size2D ovniSize(87, 56);
	Particle* ovniParticlePrototype = new Particle(ColliderType::EnemyParticle,ovniParticleAnimation,ovniSize,ovniSFX,graphics);
	particlePrototypes["ovni"] = ovniParticlePrototype;

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	for (map<string, Particle*>::iterator it = particlePrototypes.begin(); it != particlePrototypes.end(); ++it)
		RELEASE(it->second);

	return true;
}

// PreUpdate to clear up all dirty particles
update_status ModuleParticles::PreUpdate()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		if ((*it)->ToDelete())
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
			++it;
	}

	return update_status::UPDATE_CONTINUE;
}

// Update all particle logic and draw them
update_status ModuleParticles::Update()
{
	for (Particle* particle : active) {
		particle->Update();
	}

	for (Particle* particle : active) {
		particle->Render();
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticleByName(const string & name, const Vector3 & position, const Vector3& velocity)
{
	const Particle * prototype = GetParticlePrototypeByName(name);
	assert(prototype != nullptr);
	Particle * instance = prototype->Clone();
	instance->GetTransform().SetScreenPosition(position);
	instance->SetVelocity(velocity);
	this->active.push_back(instance);
}

const Particle * ModuleParticles::GetParticlePrototypeByName(const string & name) const
{
	map<string, Particle*>::const_iterator it = particlePrototypes.find(name);
	return (it != particlePrototypes.cend()) ? it->second : nullptr;
}