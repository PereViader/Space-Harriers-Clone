#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleTime.h"

#include <algorithm>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

const float ModulePlayer::PLAYER_SPEED = 3;
const float ModulePlayer::PLAYER_RECOVER_SPEED = 2.5;


ModulePlayer::ModulePlayer(bool active) : Module(active)
{
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

const fPoint & ModulePlayer::GetNormalizedPosition() const
{
	return position;
}

iPoint ModulePlayer::GetScreenPosition() const
{
	iPoint screenPosition;
	screenPosition.x = (int)(SCREEN_WIDTH  * SCREEN_SIZE * ((position.x + 1.0f) / 2.0f));
	screenPosition.y = (int)(SCREEN_HEIGHT * SCREEN_SIZE * ((position.y + 1.0f) / 2.0f));
	return screenPosition;
}

void ModulePlayer::ShootLaser()
{
	iPoint screen = GetScreenPosition();

	// correct position to shoot from the gun
	screen.x += 15;
	screen.y -= 120;

	App->particles->AddParticle(App->particles->playerParticlePrototype, screen.x, screen.y);
}

void ModulePlayer::MovePlayer()
{
	fPoint movement;
	movement.x = App->input->GetAxis(Axis::Horizontal)*PLAYER_SPEED*App->time->GetDeltaTime();
	movement.y = App->input->GetAxis(Axis::Vertical)*PLAYER_SPEED*App->time->GetDeltaTime();

	if (movement.x == 0.0f)
		movement.x = -position.x*PLAYER_RECOVER_SPEED*App->time->GetDeltaTime();

	if (movement.y == 0.0f)
		movement.y = -position.y*PLAYER_RECOVER_SPEED*App->time->GetDeltaTime();

	// Clamp position values between -1 and 1
	position.x = min(1.0f, max(-1.0f, position.x + movement.x));
	position.y = min(1.0f, max(-1.0f, position.y + movement.y));

}

void ModulePlayer::RenderPlayer()
{
	// Project player position to the screen
	iPoint screen = GetScreenPosition();

	// Draw everything --------------------------------------
	if (!destroyed)
		App->renderer->BlitWithPivotScaled(graphics, &currentAnimation->GetCurrentFrame(), 4, 0.5f, 1.0f, screen.x, screen.y);

}


update_status ModulePlayer::Update()
{
	MovePlayer();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ShootLaser();
	}

	RenderPlayer();
	
	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player
