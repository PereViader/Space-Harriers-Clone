#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <vector>
#include <string>

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

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const string& path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	SFX LoadFx(const string& path);

	// Play a previously loaded WAV
	bool PlayFx(const SFX& sfx, int repeat = 0);

private:

	Mix_Music*	music = nullptr;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __MODULEAUDIO_H__