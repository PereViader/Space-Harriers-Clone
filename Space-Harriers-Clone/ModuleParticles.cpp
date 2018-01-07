#include "ModuleParticles.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleEntity.h"


#include "Particle.h"
#include "Collider.h"
#include "ScreenBoundFloorProjectedTransform.h"

#include <math.h>

ModuleParticles::ModuleParticles(bool enabled) :
	Module(enabled)
{
}

ModuleParticles::~ModuleParticles()
{}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("data/sprites/bullets.png");
	assert(graphics != Texture());


	assert(particlePrototypes.size() == 0);
	// ---------- Player
	Animation playerParticleAnimation;
	playerParticleAnimation.frames.push_back({ 3,2,87,56 });
	playerParticleAnimation.speed = 0;
	playerParticleAnimation.loop = true;

	playerSFX = App->audio->LoadFx("data/audio/sfx/playerBullet.wav");
	Size2D size(87, 56);
	Particle* playerParticlePrototype = new Particle(ColliderType::PlayerParticle,playerParticleAnimation,size,playerSFX,graphics);
	particlePrototypes["player"] = playerParticlePrototype;


	// ---------- Ovni
	Animation ovniParticleAnimation;
	ovniParticleAnimation.frames.push_back({ 3,2,87,56 });
	ovniParticleAnimation.speed = 0;
	ovniParticleAnimation.loop = true;

	ovniSFX = App->audio->LoadFx("data/audio/sfx/enemyParticle.wav");
	Size2D ovniSize(87, 56);
	Particle* ovniParticlePrototype = new Particle(ColliderType::EnemyParticle,ovniParticleAnimation,ovniSize,ovniSFX,graphics);
	particlePrototypes["ovni"] = ovniParticlePrototype;

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	for (auto &particleDefinition : particlePrototypes) {
		RELEASE(particleDefinition.second);
	}
	particlePrototypes.clear();

	App->textures->Unload(graphics);
	App->audio->UnloadFx(playerSFX);
	App->audio->UnloadFx(ovniSFX);

	return true;
}

void ModuleParticles::AddParticleByName(const string & name, const Vector3 & position, const Vector3& velocity)
{
	Particle * instance = particlePrototypes.at(name)->Clone();
	instance->GetTransform().SetPosition(position);
	instance->SetVelocity(velocity);
	App->entity->AddGameEntity(instance);
}