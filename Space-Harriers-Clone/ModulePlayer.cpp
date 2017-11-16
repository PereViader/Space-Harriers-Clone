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

	ground_running.frames.push_back({ 4, 4, 20, 47 });
	ground_running.frames.push_back({ 25, 4, 20, 47 });
	ground_running.frames.push_back({ 49, 2, 25, 49 });
	ground_running.frames.push_back({ 75, 3, 21, 47 });
	ground_running.speed = 0.05f;

	hover_center.frames.push_back({ 108,2,26,49 });

	hover_left.frames.push_back({ 142,2,22,50 });

	hover_left_most.frames.push_back({ 170,3,20,48 });

	hover_right.frames.push_back({ 197,3,20,48 });

	hover_right_most.frames.push_back({ 221,3,22,49 });

	currentAnimation = &hover_center;


}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("rtype/player.png");

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

const fPoint & ModulePlayer::GetPosition() const
{
	return position;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	fPoint movement;
	movement.x = App->input->GetAxis(Axis::Horizontal)*speed;
	movement.y = App->input->GetAxis(Axis::Vertical)*speed;

	if (movement.x == 0.0f)
		movement.x = -position.x*recoverSpeed;

	if (movement.y == 0.0f)
		movement.y = -position.y*recoverSpeed;

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
	}

	// Clamp position values between -1 and 1
	position.x =  min(1.0f, max(-1.0f, position.x + movement.x));
	position.y = min(1.0f, max(-1.0f, position.y + movement.y));

	// Project player position to the screen
	int screenX = (int)(SCREEN_WIDTH  * SCREEN_SIZE * ((position.x + 1.0f) / 2.0f));
	int screenY = (int)(SCREEN_HEIGHT * SCREEN_SIZE * ((position.y + 1.0f) / 2.0f));
	
	// Draw everything --------------------------------------
	if (!destroyed)
		App->renderer->BlitWithPivotScaled(graphics, &currentAnimation->GetCurrentFrame(),4, 0.5f, 1.0f, screenX, screenY);

	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
