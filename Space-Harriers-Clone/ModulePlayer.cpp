#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

#include <algorithm>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	speed = 0.02f;
	recoverSpeed = 0.022f;
	position = { 0.0f, 0.0f };

	// idle animation (just the ship)
	idle.frames.push_back({66, 1, 32, 14});

	// move upwards
	up.frames.push_back({100, 1, 32, 14});
	up.frames.push_back({132, 0, 32, 14});
	up.loop = false;
	up.speed = 0.1f;

	// Move down
	down.frames.push_back({33, 1, 32, 14});
	down.frames.push_back({0, 1, 32, 14});
	down.loop = false;
	down.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("rtype/ship.png");

	destroyed = false;
	position.x = 150;
	position.y = 120;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	fPoint movement = { 0.0f, 0.0f };

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		movement.x += -speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		movement.x += speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		movement.y += speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		movement.y += -speed;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
	}

	if (movement.x == 0.0f && movement.y == 0.0f) {
		movement.x = -position.x*recoverSpeed;
		movement.y = -position.y*recoverSpeed;
	}

	// Clamp position values between -1 and 1
	position.x =  min(1.0f, max(-1.0f, position.x + movement.x));
	position.y = min(1.0f, max(-1.0f, position.y + movement.y));

	// Project player position to the screen
	int screenX = SCREEN_WIDTH * SCREEN_SIZE * ((position.x + 1.0f) / 2.0f);
	int screenY = SCREEN_HEIGHT * SCREEN_SIZE * ((position.y + 1.0f) / 2.0f);
	
	SDL_Rect screenPosition = { screenX,screenY,10,10 };

	// Draw everything --------------------------------------
	if (!destroyed)
		App->renderer->DrawQuad(screenPosition, 255, 0, 0, 255, false);

	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
