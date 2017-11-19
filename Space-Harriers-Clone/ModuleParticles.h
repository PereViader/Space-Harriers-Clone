#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;

class Particle
{
public:
	bool to_delete;
	bool isFirstFrame;

	Animation anim;
	int sfxId;

	float velocityX;
	float velocityY;
	float velocityZ;

	fPoint position;
	float positionZ;

	// TODO 11: Add an optional collider to each particle

	Particle();
	Particle(const Particle& p);
	~Particle();

	void Update();

private:
	void MoveParticle();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status PreUpdate(); // clear dirty particles
	update_status Update(); // draw
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y); // feel free to expand this call

private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle playerParticlePrototype;
};

#endif // __MODULEPARTICLES_H__