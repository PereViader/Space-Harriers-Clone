#include "ModulePlayer.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleTime.h"
#include "ModuleCollision.h"

#include "Vector2.h"


#include <iostream>
#include <algorithm>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

const float ModulePlayer::PLAYER_SPEED = 3;
const float ModulePlayer::PLAYER_RECOVER_SPEED = 2.5;

const int ModulePlayer::MAX_HORIZONTAL_POSITION = SCREEN_WIDTH*SCREEN_SIZE - 50;
const int ModulePlayer::MIN_HORIZONTAL_POSITION = 50;
const int ModulePlayer::MAX_VERTICAL_POSITION = SCREEN_HEIGHT*SCREEN_SIZE;
const int ModulePlayer::MIN_VERTICAL_POSITION = 200;


const float ModulePlayer::TRANSITION_LEFT = -0.6f;
const float ModulePlayer::TRANSITION_CENTER_LEFT = -0.2f;
const float ModulePlayer::TRANSITION_CENTER_RIGHT = 0.2f;
const float ModulePlayer::TRANSITION_RIGHT = 0.6f;

const float ModulePlayer::RENDER_SCALE = 4.0f;


ModulePlayer::ModulePlayer(bool active) : 
	Module(active),
	position({0,1}),
	destroyed(false),
	currentAnimation(&hover_center)
{
	ground_running.frames.push_back({ 4, 4, 20, 47 });
	ground_running.frames.push_back({ 25, 4, 20, 47 });
	ground_running.frames.push_back({ 49, 2, 25, 49 });
	ground_running.frames.push_back({ 75, 3, 21, 47 });
	ground_running.speed = 5.0f;

	hover_center.frames.push_back({ 108,2,26,49 });
	hover_left.frames.push_back({ 142,2,22,50 });
	hover_left_most.frames.push_back({ 170,3,20,48 });
	hover_right.frames.push_back({ 221,3,22,49 });
	hover_right_most.frames.push_back({ 197,3,20,48 });
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("rtype/player.png");

	Size2D playerColliderSize(80, 186);
	collider = App->collision->AddCollider(ColliderType::Player,playerColliderSize, Pivot2D::BOTTOM_CENTER,this);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

void ModulePlayer::UpdateAnimation()
{
	if (position.y == 1)
		currentAnimation = &ground_running;
	else {
		if (position.x < TRANSITION_CENTER_LEFT && position.x >= TRANSITION_LEFT)
			currentAnimation = &hover_left;
		else if (position.x < TRANSITION_LEFT)
			currentAnimation = &hover_left_most;
		else if (position.x > TRANSITION_CENTER_RIGHT && position.x <= TRANSITION_RIGHT)
			currentAnimation = &hover_right;
		else if (position.x > TRANSITION_RIGHT)
			currentAnimation = &hover_right_most;
		else
			currentAnimation = &hover_center;
	}
}

const fPoint & ModulePlayer::GetNormalizedPosition() const
{
	return position;
}

Vector2 ModulePlayer::GetScreenPosition() const
{
	Vector2 screenPosition;
	screenPosition.x = MIN_HORIZONTAL_POSITION + (MAX_HORIZONTAL_POSITION - MIN_HORIZONTAL_POSITION)  * ((position.x + 1.0f) / 2.0f);
	screenPosition.y = MIN_VERTICAL_POSITION + (MAX_VERTICAL_POSITION - MIN_VERTICAL_POSITION)  * ((position.y + 1.0f) / 2.0f);
	return screenPosition;
}

void ModulePlayer::ShootLaser()
{
	Vector2 screen = GetScreenPosition();

	// correct position to shoot from the gun
	screen.x += 15;
	screen.y -= 120;

	App->particles->AddParticle(App->particles->playerParticlePrototype, screen );
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

void ModulePlayer::MoveCollider()
{
	Vector2 colliderPosition = GetScreenPosition();

	collider->rect.x = colliderPosition.x - (collider->rect.w / 2.0f);
	collider->rect.y = colliderPosition.y - collider->rect.h;
}

void ModulePlayer::RenderPlayer()
{
	// Project player position to the screen
	Vector2 screen = GetScreenPosition();

	// Draw everything --------------------------------------
	if (!destroyed)
		App->renderer->BlitWithPivotScaledZBuffer(graphics, &currentAnimation->GetCurrentFrame(), RENDER_SCALE,Pivot2D::BOTTOM_CENTER,screen);

}

void ModulePlayer::OnCollision(const Collider * own, const Collider * other)
{
	LOG("%s", "collided player");
}


update_status ModulePlayer::Update()
{
	MovePlayer();
	MoveCollider();
	UpdateAnimation();

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
