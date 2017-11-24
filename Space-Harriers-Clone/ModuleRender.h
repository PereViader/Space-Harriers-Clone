#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"

#include <queue>
#include <utility>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

using namespace std;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool BlitZBuffer(SDL_Texture * texture, SDL_Rect * section, SDL_Rect * screen, float z);
	bool BlitWithPivotScaledZBuffer(SDL_Texture * texture, SDL_Rect * section, float scale, float pivotX, float pivotY, int x, int y, float z);


	bool BlitWithPivotScaled(SDL_Texture*, SDL_Rect* section, float scale, float pivotX, float pivotY, int x, int y);
	bool DirectBlit(SDL_Texture* texture, SDL_Rect* section, SDL_Rect* destiny);

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = SDL_ALPHA_OPAQUE);
	bool DrawQuads(const SDL_Rect rect[], int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool DrawSegment(int startx, int starty, int endx, int endy, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

public:
	SDL_Renderer* renderer = nullptr;

private:
	class ZElement {
	public:
		float zValue;
		SDL_Texture* graphic;
		SDL_Rect section;
		SDL_Rect screen;

		bool operator<(const ZElement& other) const{
			return zValue < other.zValue;
		}
	};

	priority_queue<ZElement,vector<ZElement>> zBuffer;
};

#endif // __MODULERENDER_H__