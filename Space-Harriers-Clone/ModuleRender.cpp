#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "RectUtil.h"
#include "SDL/include/SDL.h"

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
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
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

bool ModuleRender::BlitZBuffer(SDL_Texture * texture, SDL_Rect * section, SDL_Rect * screen, float z)
{
	ZElement zElement;
	zElement.graphic = texture;
	zElement.section = *section;
	zElement.screen = *screen;
	zElement.zValue = z;

	zBuffer.push(zElement);
	return true;
}

bool ModuleRender::BlitWithPivotScaledZBuffer(SDL_Texture * texture, SDL_Rect * section, float scale, float pivotX, float pivotY, int x, int y, float z)
{
	float width;
	float height;
	if (section != nullptr) {
		width = (float)section->w;
		height = (float)section->h;
	}
	else {
		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		width = (float)w;
		height = (float)h;
	}
	width *= scale;
	height *= scale;


	SDL_Rect rectForPivot = GetRectInPositionWithPivot(x, y, width, height, pivotX, pivotY);
	BlitZBuffer(texture, section, &rectForPivot, z);

	return true;
}

bool ModuleRender::BlitWithPivotScaled(SDL_Texture * texture, SDL_Rect * section, float scale, float pivotX, float pivotY, int x, int y)
{
	float width;
	float height;
	if (section != nullptr) {
		width = (float)section->w;
		height = (float)section->h;
	}
	else {
		int w, h;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		width = (float)w;
		height = (float)h;
	}
	width *= scale;
	height *= scale;


	SDL_Rect rectForPivot = GetRectInPositionWithPivot(x, y, width, height, pivotX, pivotY);

	DirectBlit(texture, section, &rectForPivot);
	return true;
}

bool ModuleRender::DirectBlit(SDL_Texture* texture, SDL_Rect* section, SDL_Rect* destiny)
{
	bool ret = true;
	if (SDL_RenderCopy(renderer, texture, section, destiny) != 0)
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

bool ModuleRender::DrawSegment(int startx, int starty, int endx, int endy, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	if (SDL_RenderDrawLine(renderer, startx, starty, endx, endy) != 0)
	{
		LOG("Cannot draw segment to screen. SDL_RenderDrawLine error: %s", SDL_GetError());
		ret = false;
	}
	return ret;
}


