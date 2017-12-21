#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "ICollidable.h"
#include "IClonable.h"

#include "ScreenBoundTransform.h"

#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"
#include "Texture.h"

#include<list>
#include <map>

class Vector3;
class Particle;

using namespace std;

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status PreUpdate(); // clear dirty particles
	update_status Update(); // draw
	bool CleanUp();

	void AddParticleByName(const string& name, const Vector3& position, const Vector3& velocity);

private:
	Texture graphics;
	list<Particle*> active;
	map<string, Particle*> particlePrototypes;
};

#endif // __MODULEPARTICLES_H__