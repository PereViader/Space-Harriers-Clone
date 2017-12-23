#include "ModulePlayer.h"

#include <iostream>
#include <algorithm>

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleShadow.h"
#include "ModuleTime.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"

#include "Collider.h"
#include "Vector3.h"



const float ModulePlayer::PLAYER_SPEED = 800;

const int ModulePlayer::MAX_HORIZONTAL_POSITION = SCREEN_WIDTH - 50;
const int ModulePlayer::MIN_HORIZONTAL_POSITION = 50;
const int ModulePlayer::MAX_VERTICAL_POSITION = SCREEN_HEIGHT;
const int ModulePlayer::MIN_VERTICAL_POSITION = 200;


const float ModulePlayer::TRANSITION_LEFT = -0.6f;
const float ModulePlayer::TRANSITION_CENTER_LEFT = -0.2f;
const float ModulePlayer::TRANSITION_CENTER_RIGHT = 0.2f;
const float ModulePlayer::TRANSITION_RIGHT = 0.6f;

const float ModulePlayer::RENDER_SCALE = 4.0f;

const Vector3 ModulePlayer::PLAYER_PARTICLE_VELOCITY(0, 90, 1000);


ModulePlayer::ModulePlayer(bool active) : 
	Module(active),
	GameEntity(new FloorBasedTransform()),
	destroyed(false),
	currentAnimation(&hover_center)
{
	GetTransform().SetPosition(Vector3((SCREEN_WIDTH / 2.0f), 0, 10));

	ground_running.frames.push_back({ 4, 4, 20, 47 });
	ground_running.frames.push_back({ 25, 4, 20, 47 });
	ground_running.frames.push_back({ 49, 2, 25, 49 });
	ground_running.frames.push_back({ 75, 3, 21, 47 });
	ground_running.speed = 5.0f;
	ground_running.loop = true;

	hover_center.frames.push_back({ 108,2,26,49 });
	hover_center.speed = 0;
	hover_center.loop = false;

	hover_left.frames.push_back({ 142,2,22,50 });
	hover_left.speed = 0;
	hover_left.loop = false;

	hover_left_most.frames.push_back({ 170,3,20,48 });
	hover_left_most.speed = 0;
	hover_left_most.loop = false;

	hover_right.frames.push_back({ 221,3,22,49 });
	hover_right.speed = 0;
	hover_right.loop = false;

	hover_right_most.frames.push_back({ 197,3,20,48 });
	hover_right_most.speed = 0;
	hover_right_most.loop = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("data/player.png");

	Size2D playerColliderSize(80, 186);
	collider = App->collision->AddCollider(ColliderType::Player,playerColliderSize, Pivot2D::BOTTOM_CENTER,*this);

	ouchSFX = App->audio->LoadFx("data/sfx/ouch.wav");

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

ModulePlayer* ModulePlayer::Clone() const
{
	assert(false);
	return nullptr;
}

Vector2 ModulePlayer::GetInputMovement() const
{
	Vector2 movement;
	movement.x = App->input->GetAxis(Axis::Horizontal)*PLAYER_SPEED*App->time->GetDeltaTime();
	movement.y = -App->input->GetAxis(Axis::Vertical)*PLAYER_SPEED*App->time->GetDeltaTime();
	return movement;
}

void ModulePlayer::ShootLaser()
{
	Vector3 screen = GetTransform().GetScreenPositionAndDepth();

	// correct position to shoot from the gun
	screen.x += 15;
	screen.y -= 120;

	App->particles->AddParticleByName("player", screen, PLAYER_PARTICLE_VELOCITY);
}

void ModulePlayer::MovePlayer()
{
	Vector2 movement = GetInputMovement();

	//Clamp movement to not leave the screen
	{
		Vector2 position = GetTransform().GetScreenPositionAndDepth();

		if (position.x == MIN_HORIZONTAL_POSITION && movement.x < 0 || position.x < MIN_HORIZONTAL_POSITION) {
			movement.x = MIN_HORIZONTAL_POSITION - position.x;
		}
		else if (position.x == MAX_HORIZONTAL_POSITION && movement.x > 0 || position.x > MAX_HORIZONTAL_POSITION) {
			movement.x = MAX_HORIZONTAL_POSITION - position.x;
		}

		if (position.y == MAX_VERTICAL_POSITION && movement.y < 0 || position.y > MAX_VERTICAL_POSITION)
			movement.y = position.y - MAX_VERTICAL_POSITION;
		else if (position.y == MIN_VERTICAL_POSITION && movement.y > 0 || position.y < MIN_VERTICAL_POSITION)
			movement.y = position.y - MIN_VERTICAL_POSITION;
	}

	GetTransform().Move(movement);
}

void ModulePlayer::Render()
{
	if (!destroyed) {
		Vector3 screen = GetTransform().GetScreenPositionAndDepth();
		float scale = GetTransform().GetRenderingScale() * RENDER_SCALE;
		currentAnimation->UpdateFrame();
		graphics.UpdateTexture(*currentAnimation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::BOTTOM_CENTER, screen);
		App->shadow->DrawShadow(GetTransform());
	}
}

Vector2 ModulePlayer::GetNormalizedPosition() const
{
	Vector2 position = GetTransform().GetScreenPositionAndDepth();
	position.x = (((position.x - MIN_HORIZONTAL_POSITION) / (MAX_HORIZONTAL_POSITION-MIN_HORIZONTAL_POSITION)) - 0.5f ) * 2.0f;
	position.y = (((position.y - MIN_VERTICAL_POSITION) / (MAX_VERTICAL_POSITION- MIN_VERTICAL_POSITION)) - 0.5f) * 2.0f;
	return position;
}

Vector3 ModulePlayer::GetChestPosition() const
{
	return GetTransform().GetScreenPositionAndDepth() + Vector3(0,-93);
}

void ModulePlayer::OnCollision(const Collider& own, const Collider& other)
{
	if (other.colliderType == ColliderType::NonDamagingEnemy) {
		App->audio->PlayFx(ouchSFX);
	}
	else { // collided enemy or enemy particle

	}
}

update_status ModulePlayer::Update()
{
	MovePlayer();
	UpdateAnimation();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ShootLaser();
	}

	Render();
	
	return update_status::UPDATE_CONTINUE;
}