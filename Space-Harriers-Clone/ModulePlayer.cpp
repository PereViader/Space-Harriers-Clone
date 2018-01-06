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

const float ModulePlayer::INVINCIBLE_TIME_AFTER_INTERACTION = 2.0f;

const float ModulePlayer::FALL_SPEED = 600.0f;

const int ModulePlayer::STARTING_HEALTH_POINTS = 3;


ModulePlayer::ModulePlayer(bool active) : 
	Module(active),
	GameEntity(new FloorBasedTransform()),
	destroyed(false),
	currentAnimation(&hover_center),
	invincibleTime(0),
	isInvincible(false),
	isFallingToTheFloor(false),
	healthPoints(STARTING_HEALTH_POINTS)
{
}

ModulePlayer::~ModulePlayer()
{}

bool ModulePlayer::Init()
{
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

	tripOverHazzard.frames.push_back({ 6,118,22,38 });
	tripOverHazzard.frames.push_back({ 45,120,22,33 });
	tripOverHazzard.frames.push_back({ 83,123,24,28 });
	tripOverHazzard.speed = 5;
	tripOverHazzard.loop = false;

	takeDamage.frames.push_back({ 6,61,22,39 });
	takeDamage.frames.push_back({ 34,61,26,40 });
	takeDamage.frames.push_back({ 73,70,26,22 });
	takeDamage.frames.push_back({ 113,74,27,20 });
	//takeDamage.frames.push_back({ 158,65,27,31 }); // this frame is useful only if you make a delay on the previous frame and proceding to the next
	takeDamage.loop = false;
	takeDamage.speed = 5;

	return true;
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("data/sprites/player.png");

	ouchSFX = App->audio->LoadFx("data/audio/sfx/ouch.wav");
	aaaaarghSFX = App->audio->LoadFx("data/audio/sfx/aaaaargh.wav");

	Size2D playerColliderSize(80, 186);
	collider = App->collision->AddCollider(ColliderType::Player,playerColliderSize, Pivot2D::BOTTOM_CENTER,*this);

	destroyed = false;
	currentAnimation = &hover_center;
	invincibleTime = 0;
	isInvincible = false;
	isFallingToTheFloor = false;
	healthPoints = STARTING_HEALTH_POINTS;

	GetTransform().SetPosition(Vector3((SCREEN_WIDTH / 2.0f), 0, 0));

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	App->audio->UnloadFx(aaaaarghSFX);
	App->audio->UnloadFx(ouchSFX);

	return true;
}

void ModulePlayer::UpdateAnimation()
{
	if (currentAnimation == &tripOverHazzard) {
		if (!tripOverHazzard.Finished())
			return;
	}

	Vector2 position = GetNormalizedPosition();

	if (currentAnimation == &takeDamage) {
		if (!takeDamage.Finished() || isInvincible || healthPoints==0)
			return;
	}

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

void ModulePlayer::UpdateInvincibility()
{
	if (isInvincible) {
		invincibleTime += App->time->GetDeltaTime();
		if (invincibleTime >= INVINCIBLE_TIME_AFTER_INTERACTION) {
			invincibleTime = 0;
			isInvincible = false;
		}
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
	if (healthPoints > 0) {
		if (!isFallingToTheFloor) {
			//Clamp movement to not leave the screen
			Vector3 position = GetTransform().GetScreenPositionAndDepth();
			Vector2 movement = GetInputMovement();
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

			GetTransform().Move(movement);
		}
		else {
			Vector3 position = GetTransform().GetPosition();
			Vector3 floor(position.x, 0, position.z);
			Vector3 newPosition = MoveTowards(position, floor, FALL_SPEED*App->time->GetDeltaTime());

			GetTransform().SetPosition(newPosition);
			isFallingToTheFloor = GetTransform().GetPosition().y != 0 || isInvincible;
		}
	}
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

int ModulePlayer::GetHealthPoints() const
{
	return healthPoints;
}

void ModulePlayer::OnCollision(const Collider& own, const Collider& other)
{
	if (!isInvincible) {
		if (other.colliderType == ColliderType::NonDamagingEnemy) {
			App->audio->PlayFx(ouchSFX);
			tripOverHazzard.Reset();
			currentAnimation = &tripOverHazzard;
		}
		else { // collided enemy or enemy particle
			App->audio->PlayFx(aaaaarghSFX);
			takeDamage.Reset();
			currentAnimation = &takeDamage;
			isInvincible = true;
			isFallingToTheFloor = true;
			healthPoints--;
		}
	}
}

update_status ModulePlayer::Update()
{
	UpdateInvincibility();
	MovePlayer();
	UpdateAnimation();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isFallingToTheFloor)
	{
		ShootLaser();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		healthPoints = 0;

	Render();
	
	return update_status::UPDATE_CONTINUE;
}