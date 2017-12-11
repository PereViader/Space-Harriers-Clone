#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

#include <assert.h>

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

using namespace std;

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	//TODO: Should check if all textures have been unloaded insted of unloading them now

	for(map<string, SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(it->second);

	textures.clear();
	textureUsage.clear();

	return true;
}

// Load new texture from file path
Texture ModuleTextures::Load(const string& path)
{
	SDL_Texture* texture = nullptr;

	auto aviableTexture = textures.find(path);
	if (aviableTexture != textures.end()) {
		textureUsage[aviableTexture->second] += 1;
		texture = aviableTexture->second;
	}
	else {
		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr)
		{
			LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
		}
		else
		{
			texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);

			if (texture == nullptr)
			{
				LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				textures[path] = texture;
				textureUsage[texture] = 1;
			}

			SDL_FreeSurface(surface);
		}
	}

	return Texture(texture);
}

// Free texture from memory
void ModuleTextures::Unload(const Texture& texture)
{
	map<SDL_Texture*, unsigned int>::iterator it0 = textureUsage.find(texture.GetTexture());
	assert(it0 != textureUsage.end());
	if (it0 != textureUsage.end())
	{
		it0->second -= 1;
		if (it0->second == 0) {
			// remove texture from the path to texture map
			for (map<string, SDL_Texture*>::iterator it1 = textures.begin(); it1 != textures.end(); it1++) {
				if (it1->second == it0->first)
				{
					textures.erase(it1);
					break;
				}
			}

			// remove texture from the usage map
			textureUsage.erase(it0);

			SDL_DestroyTexture(texture.GetTexture());
		}
	}
}