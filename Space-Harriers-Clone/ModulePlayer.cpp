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

const float ModulePlayer::PLAYER_SPEED = 800;
//const float ModulePlayer::PLAYER_RECOVER_SPEED = 700;

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
	destroyed(false),
	currentAnimation(&hover_center)
{
	transform.SetScreenPosition(Vector2((SCREEN_WIDTH*SCREEN_SIZE / 2.0f), static_cast<float>(MAX_VERTICAL_POSITION)));

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
	Vector2 position = GetNormalizedPosition();
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

void ModulePlayer::ShootLaser()
{
	Vector2 screen = transform.GetScreenPositionAndDepth();

	// correct position to shoot from the gun
	screen.x += 15;
	screen.y -= 120;

	App->particles->AddParticle(App->particles->playerParticlePrototype, screen );
}

void ModulePlayer::MovePlayer()
{
	Vector2 movement;
	movement.x = App->input->GetAxis(Axis::Horizontal)*PLAYER_SPEED*App->time->GetDeltaTime();
	movement.y = App->input->GetAxis(Axis::Vertical)*PLAYER_SPEED*App->time->GetDeltaTime();

	Vector2 position = transform.GetScreenPositionAndDepth();
	
	//Return to center when not moving
	/*if (movement.x == 0.0f)
		movement.x = -position.x*PLAYER_RECOVER_SPEED*App->time->GetDeltaTime();

	if (movement.y == 0.0f)
		movement.y = -position.y*PLAYER_RECOVER_SPEED*App->time->GetDeltaTime();*/

	//Clamp position inside the screen
	if (position.x == MIN_HORIZONTAL_POSITION && movement.x < 0 || position.x < MIN_HORIZONTAL_POSITION)
		movement.x = MIN_HORIZONTAL_POSITION - position.x;
	else if (position.x == MAX_HORIZONTAL_POSITION && movement.x > 0 || position.x > MAX_HORIZONTAL_POSITION)
		movement.x = MAX_HORIZONTAL_POSITION - position.x;
	
	if (position.y == MIN_VERTICAL_POSITION && movement.y < 0 || position.y < MIN_VERTICAL_POSITION)
		movement.y = MIN_VERTICAL_POSITION - position.y;
	else if (position.y == MAX_VERTICAL_POSITION && movement.y > 0 || position.y > MAX_VERTICAL_POSITION)
		movement.y = MAX_VERTICAL_POSITION - position.y;

	transform.Move(movement);
}

void ModulePlayer::Render()
{
	Vector2 screen = transform.GetScreenPositionAndDepth();

	// Draw everything --------------------------------------
	if (!destroyed) {
		currentAnimation->UpdateFrame();
		graphics.UpdateTexture(*currentAnimation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, RENDER_SCALE, Pivot2D::BOTTOM_CENTER, screen);
	}
}

Vector2 ModulePlayer::GetNormalizedPosition() const
{
	Vector2 position = transform.GetScreenPositionAndDepth();
	position.x = (((position.x - MIN_HORIZONTAL_POSITION) / (MAX_HORIZONTAL_POSITION-MIN_HORIZONTAL_POSITION)) - 0.5f ) * 2.0f;
	position.y = (((position.y - MIN_VERTICAL_POSITION) / (MAX_VERTICAL_POSITION- MIN_VERTICAL_POSITION)) - 0.5f) * 2.0f;
	return position;
}

void ModulePlayer::OnCollision(const Collider * own, const Collider * other)
{
	LOG("%s", "collided player");
}


update_status ModulePlayer::Update()
{
	MovePlayer();
	collider->UpdateValues(transform);
	UpdateAnimation();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ShootLaser();
	}

	Render();
	
	return update_status::UPDATE_CONTINUE;
}