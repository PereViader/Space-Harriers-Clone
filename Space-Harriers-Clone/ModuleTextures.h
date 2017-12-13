#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Module.h"

#include<map>
#include <string>

class Texture;
struct SDL_Texture;

using namespace std;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	Texture Load(const string& path);
	void Unload(Texture& texture);

private:
	map<string, SDL_Texture*> textures;
	map<SDL_Texture*, unsigned int> textureUsage;
};

#endif // __MODULETEXTURES_H__