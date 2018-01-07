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
#include "SFX.h"

#include<list>
#include <map>

class Vector3;
class Particle;

using namespace std;

class ModuleParticles : public Module
{
public:
	ModuleParticles(bool enabled);
	~ModuleParticles();

	bool Start();
	bool CleanUp();

	void AddParticleByName(const string& name, const Vector3& position, const Vector3& velocity);

private:
	Texture graphics;
	map<string, Particle*> particlePrototypes;

	SFX ovniSFX;
	SFX playerSFX;
};

#endif // __MODULEPARTICLES_H__