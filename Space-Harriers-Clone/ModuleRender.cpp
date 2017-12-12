#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "RectUtil.h"
#include "SDL/include/SDL.h"

#include "Pivot2D.h"
#include "Size2D.h"
#include "Vector3.h"
#include "Texture.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	while (zBuffer.size() > 0) {
		ZElement element = zBuffer.top();
		zBuffer.pop();
		DirectBlit(element.texture, &element.screen);
	}

	SDL_RenderPresent(renderer);
	return update_status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

bool ModuleRender::BlitZBuffer(const Texture& texture, SDL_Rect * screen, float z)
{
	ZElement zElement(texture,*screen,z);
	zBuffer.push(zElement);
	return true;
}

bool ModuleRender::BlitWithPivotScaledZBuffer(const Texture& texture, float scale, const Pivot2D& pivot, const Vector3& position)
{
	Size2D size(scale * texture.GetSection().w, scale * texture.GetSection().h);
	SDL_Rect rectForPivot = GetRectInPositionWithPivot(position,size,pivot);
	BlitZBuffer(texture, &rectForPivot, position.z);
	return true;
}

bool ModuleRender::BlitWithPivotScaled(const Texture& texture, float scale, const Pivot2D& pivot, const Vector2& position)
{
	Size2D size(scale * texture.GetSection().w, scale * texture.GetSection().h);
	SDL_Rect rectForPivot = GetRectInPositionWithPivot(position,size,pivot);
	DirectBlit(texture, &rectForPivot);
	return true;
}

bool ModuleRender::DirectBlit(const Texture& texture, SDL_Rect* destiny)
{
	bool ret = true;
	if (SDL_RenderCopy(renderer, texture.GetTexture(), &texture.GetSection(), destiny) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}


bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);

	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}
	
	return ret;
}

bool ModuleRender::DrawQuads(const SDL_Rect rects[], int count, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderFillRects(renderer,rects,count) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRects error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	bool ret = true;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderDrawPoint(renderer, x, y)	!= 0)
	{
		LOG("Cannot draw pixel to screen. SDL_RenderDrawPoint error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}

bool ModuleRender::DrawSegment(const Vector2& start, const Vector2& end, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderDrawLine(renderer, static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y)) != 0)
	{
		LOG("Cannot draw segment to screen. SDL_RenderDrawLine error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}