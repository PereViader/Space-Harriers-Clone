#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Texture.h"

#include <queue>
#include <utility>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class Pivot2D;
class Vector2;
class Vector3;

using namespace std;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	bool BlitZBuffer(const Texture& texture, SDL_Rect * screen, float z);
	bool BlitWithPivotScaledZBuffer(const Texture& texture, float scale, const Pivot2D& pivot, const Vector3& position);


	bool BlitWithPivotScaled(const Texture& texture, float scale, const Pivot2D& pivot, const Vector2& position);
	bool DirectBlit(const Texture& texture, SDL_Rect* destiny);

	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = SDL_ALPHA_OPAQUE);
	bool DrawQuads(const SDL_Rect rect[], int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool DrawSegment(const Vector2& start, const Vector2& end, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
public:
	SDL_Renderer* renderer = nullptr;

private:
	class ZElement {
	public:
		ZElement(const Texture& texture, const SDL_Rect& screen, float zValue) :
			texture(texture),
			screen(screen),
			zValue(zValue)
		{
		}

		Texture texture;
		SDL_Rect screen;
		float zValue;

		bool operator<(const ZElement& other) const{
			return zValue < other.zValue;
		}
	};

	priority_queue<ZElement,vector<ZElement>> zBuffer;
};

#endif // __MODULERENDER_H__