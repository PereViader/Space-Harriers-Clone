#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
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
	// debug camera
	int speed = 1;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->renderer->camera.y += speed;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->renderer->camera.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->renderer->camera.x += speed;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->renderer->camera.x -= speed;

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

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int)(camera.y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
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

	SDL_Rect rectForPivot;
	rectForPivot.w = (int)(width);
	rectForPivot.h = (int)(height);
	rectForPivot.x = (int)(x - (width*pivotX));
	rectForPivot.y = (int)(y - (height*pivotY));

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


bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

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


