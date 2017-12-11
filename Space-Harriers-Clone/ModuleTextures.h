#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include<map>
#include <string>

#include "Module.h"
#include "Globals.h"
#include "Texture.h"

using namespace std;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	Texture Load(const string& path);
	void Unload(const Texture& texture);

private:
	map<string, SDL_Texture*> textures;
	map<SDL_Texture*, unsigned int> textureUsage;
};

#endif // __MODULETEXTURES_H__