#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <map>

#include "Module.h"
#include "SFX.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

using namespace std;

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:

	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();

	virtual bool Init() override;
	virtual bool CleanUp() override;

	// Play a music file
	bool PlayMusic(const string& path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	SFX LoadFx(const string& path);
	void UnloadFx(SFX & sfx);

	void RegisterFxUsage(const SFX&);

	// Play a previously loaded WAV
	bool PlayFx(const SFX& sfx, int repeat = 0);

private:

	Mix_Music*	music = nullptr;

	map<string, Mix_Chunk*> sfxs;
	map<Mix_Chunk*, unsigned int> sfxUsage;
};

#endif // __MODULEAUDIO_H__