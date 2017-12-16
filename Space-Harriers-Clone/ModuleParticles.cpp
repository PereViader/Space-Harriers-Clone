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
	graphics = App->textures->Load("rtype/bullets.png");

	Particle* playerParticlePrototype = new Particle();
	playerParticlePrototype->anim.frames.push_back({ 3,2,87,56 });
	playerParticlePrototype->sfxId = App->audio->LoadFx("rtype/Laser1.wav");
	playerParticlePrototype->collider = App->collision->AddPrototypeCollider(ColliderType::PlayerParticle,Size2D(87,56),Pivot2D::MIDDLE_CENTER,playerParticlePrototype);
	particlePrototypes["player"] = playerParticlePrototype;

	Particle* ovniParticlePrototype = new Particle();
	ovniParticlePrototype->anim.frames.push_back({ 3,2,87,56 });
	ovniParticlePrototype->sfxId = App->audio->LoadFx("rtype/Laser1.wav");
	ovniParticlePrototype->collider = App->collision->AddPrototypeCollider(ColliderType::EnemyParticle, Size2D(87, 56), Pivot2D::MIDDLE_CENTER, ovniParticlePrototype);
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
		if ((*it)->to_delete)
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
	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
	{
		Particle* p = *it;

		p->Update();
		Vector3 screenPosition = p->transform.GetScreenPositionAndDepth();

		float scale = p->transform.GetRenderingScale();
		p->anim.UpdateFrame();
		graphics.UpdateTexture(p->anim);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, screenPosition);
		if (p->isFirstFrame)
		{
			App->audio->PlayFx(p->sfxId);
			p->isFirstFrame = false;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticleByName(const string & name, const Vector3 & position, const Vector3& velocity)
{
	const Particle * prototype = GetParticlePrototypeByName(name);
	assert(prototype != nullptr);
	Particle * instance = prototype->Clone();
	instance->transform.SetScreenPosition(position);
	instance->velocity = velocity;
	instance->collider = App->collision->RegisterPrototypeInstance(*instance->collider, instance);
	this->active.push_back(instance);
}

const Particle * ModuleParticles::GetParticlePrototypeByName(const string & name) const
{
	map<string, Particle*>::const_iterator it = particlePrototypes.find(name);
	return (it != particlePrototypes.cend()) ? it->second : nullptr;
}