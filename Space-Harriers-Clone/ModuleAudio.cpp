#include "ModuleAudio.h"
#include "Globals.h"

#include <assert.h>

#include "Application.h"
#include "SFX.h"
#include "SDL/include/SDL.h"

#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )


using namespace std;

ModuleAudio::ModuleAudio( bool start_enabled) : Module( start_enabled)
{}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != nullptr)
	{
		Mix_FreeMusic(music);
	}

	for (auto it = sfxs.begin(); it != sfxs.end(); ++it) {
		Mix_Chunk* chunk = it->second;
		Mix_FreeChunk(chunk);
	}
	sfxs.clear();
	sfxUsage.clear();

	Mix_CloseAudio();
	while (Mix_Init(0)) {
		Mix_Quit();
	}
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const string& path, float fade_time)
{
	bool ret = true;

	if(music != nullptr)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path.c_str());

	if(music == nullptr)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
SFX ModuleAudio::LoadFx(const string& path)
{
	Mix_Chunk* chunk = nullptr;
	
	auto existingChunkIt = sfxs.find(path);
	if (existingChunkIt != sfxs.end()) {
		chunk = existingChunkIt->second;
		sfxUsage[chunk] += 1;
	}
	else {
		chunk = Mix_LoadWAV(path.c_str());
		if (chunk == nullptr)
		{
			LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
		}
		else
		{
			sfxs[path] = chunk;
			sfxUsage[chunk] = 1;
		}
	}
	
	return SFX(chunk);
}


void ModuleAudio::UnloadFx(SFX & sfx)
{
	assert(sfx.GetSfx() != nullptr);
	auto it0 = sfxUsage.find(sfx.GetSfx());
	if (it0 != sfxUsage.end()) {
		it0->second -= 1;
		if (it0->second == 0) {
			// remove sfx from the path to texture map
			for (auto it1 = sfxs.begin(); it1 != sfxs.end(); it1++) {
				if (it1->second == it0->first)
				{
					sfxs.erase(it1);
					break;
				}
			}

			// remove texture from the usage map
			sfxUsage.erase(it0);
			Mix_FreeChunk(sfx.GetSfx());
		}
		sfx = SFX();
	}
}

void ModuleAudio::RegisterFxUsage(const SFX & sfx)
{
	assert(sfx.GetSfx());
	auto it = sfxUsage.find(sfx.GetSfx());
	assert(it != sfxUsage.end());
	if (it != sfxUsage.end()) {
		it->second += 1;
	}
}

// Play WAV
bool ModuleAudio::PlayFx(const SFX& sfx, int repeat)
{
	bool ret = false;

	Mix_Chunk * chunk = sfx.GetSfx();
	assert(chunk);

	if(chunk)
	{
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;
	}

	return ret;
}