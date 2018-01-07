#include "Player.h"

#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleShadow.h"
#include "ModuleParticles.h"

#include "Size2D.h"
#include "Collider.h"

//Screen Bounds
const int Player::MAX_HORIZONTAL_POSITION = SCREEN_WIDTH - 50;
const int Player::MIN_HORIZONTAL_POSITION = 50;
const int Player::MAX_VERTICAL_POSITION = SCREEN_HEIGHT;
const int Player::MIN_VERTICAL_POSITION = 200;

//Animation transitions
const float Player::TRANSITION_LEFT = -0.6f;
const float Player::TRANSITION_CENTER_LEFT = -0.2f;
const float Player::TRANSITION_CENTER_RIGHT = 0.2f;
const float Player::TRANSITION_RIGHT = 0.6f;

// Position and rendering details
const float Player::RENDER_SCALE = 4.0f;
const Vector3 Player::PLAYER_GUN_POSITION_DELTA(15, -120, 0);
const Vector3 Player::PLAYER_CHEST_POSITION_DELTA(0, -93);

//Behaviour "parameters"
const float Player::INVINCIBLE_TIME_AFTER_INTERACTION = 2.0f;
const float Player::FALL_SPEED = 600.0f;
const int Player::STARTING_HEALTH_POINTS = 3;
const Vector3 Player::PLAYER_PARTICLE_VELOCITY(0, 90, 1000);
const float Player::PLAYER_SPEED = 800;

Player::Player() :
	GameEntity(new FloorBasedTransform()),
	destroyed(false),
	currentAnimation(&hover_center),
	invincibleTime(0),
	isInvincible(false),
	isFallingToTheFloor(false),
	healthPoints(STARTING_HEALTH_POINTS)
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
	//takeDamage.frames.push_back({ 158,65,27,31 }); // this frame is useful only if you make a delay on the previous frame before proceding to the next
	takeDamage.loop = false;
	takeDamage.speed = 5;


	graphics = App->textures->Load("data/sprites/player.png");

	ouchSFX = App->audio->LoadFx("data/audio/sfx/ouch.wav");
	aaaaarghSFX = App->audio->LoadFx("data/audio/sfx/aaaaargh.wav");

	Size2D playerColliderSize(80, 186);
	collider = App->collision->AddCollider(ColliderType::Player, playerColliderSize, Pivot2D::BOTTOM_CENTER, *this);

	GetTransform().SetPosition(Vector3((SCREEN_WIDTH / 2.0f), 0, 0));
}

Player::~Player()
{
	App->textures->Unload(graphics);

	App->audio->UnloadFx(aaaaarghSFX);
	App->audio->UnloadFx(ouchSFX);

	collider->MarkAsDeleted();
	collider = nullptr;
}

void Player::UpdateAnimation()
{
	if (currentAnimation == &tripOverHazzard) {
		if (!tripOverHazzard.Finished())
			return;
	}

	Vector2 position = GetNormalizedPosition();

	if (currentAnimation == &takeDamage) {
		if (!takeDamage.Finished() || isInvincible || !IsAlive())
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

void Player::UpdateInvincibility()
{
	if (isInvincible) {
		invincibleTime += App->time->GetDeltaTime();
		if (invincibleTime >= INVINCIBLE_TIME_AFTER_INTERACTION) {
			invincibleTime = 0;
			isInvincible = false;
		}
	}
}

Player* Player::Clone() const
{
	return new Player(*this);
}

Vector2 Player::GetInputMovement() const
{
	Vector2 movement;
	movement.x = App->input->GetAxis(Axis::Horizontal)*PLAYER_SPEED*App->time->GetDeltaTime();
	movement.y = -App->input->GetAxis(Axis::Vertical)*PLAYER_SPEED*App->time->GetDeltaTime();
	return movement;
}

void Player::ShootLaser()
{
	Vector3 screen = GetTransform().GetScreenPositionAndDepth();

	// correct position to shoot from the gun
	screen += PLAYER_GUN_POSITION_DELTA;

	App->particles->AddParticleByName("player", screen, PLAYER_PARTICLE_VELOCITY);
}

void Player::MovePlayer()
{
	if (!isFallingToTheFloor) {
		if (IsAlive()) {
			RegularPlayerMovement();
		}
	}
	else {
		FallToTheFloorPlayerMovement();
	}
}

void Player::FallToTheFloorPlayerMovement()
{
	Vector3 position = GetTransform().GetPosition();
	Vector3 floor(position.x, 0, position.z);
	Vector3 newPosition = MoveTowards(position, floor, FALL_SPEED*App->time->GetDeltaTime());

	GetTransform().SetPosition(newPosition);
	isFallingToTheFloor = GetTransform().GetPosition().y != 0 || isInvincible;
}

void Player::RegularPlayerMovement()
{
	Vector3 position = GetTransform().GetScreenPositionAndDepth();
	Vector2 movement = GetInputMovement();

	//Clamp x movement to not leave the screen horizontally
	if (position.x == MIN_HORIZONTAL_POSITION && movement.x < 0 || position.x < MIN_HORIZONTAL_POSITION) {
		movement.x = MIN_HORIZONTAL_POSITION - position.x;
	}
	else if (position.x == MAX_HORIZONTAL_POSITION && movement.x > 0 || position.x > MAX_HORIZONTAL_POSITION) {
		movement.x = MAX_HORIZONTAL_POSITION - position.x;
	}

	//Clamp y movement to not leave the screen vertically
	if (position.y == MAX_VERTICAL_POSITION && movement.y < 0 || position.y > MAX_VERTICAL_POSITION)
		movement.y = position.y - MAX_VERTICAL_POSITION;
	else if (position.y == MIN_VERTICAL_POSITION && movement.y > 0 || position.y < MIN_VERTICAL_POSITION)
		movement.y = position.y - MIN_VERTICAL_POSITION;

	GetTransform().Move(movement);
}

void Player::Render()
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

Vector2 Player::GetNormalizedPosition() const
{
	Vector2 position = GetTransform().GetScreenPositionAndDepth();
	position.x = (((position.x - MIN_HORIZONTAL_POSITION) / (MAX_HORIZONTAL_POSITION - MIN_HORIZONTAL_POSITION)) - 0.5f) * 2.0f;
	position.y = (((position.y - MIN_VERTICAL_POSITION) / (MAX_VERTICAL_POSITION - MIN_VERTICAL_POSITION)) - 0.5f) * 2.0f;
	return position;
}

Vector3 Player::GetChestPosition() const
{
	return GetTransform().GetScreenPositionAndDepth() + PLAYER_CHEST_POSITION_DELTA;
}

int Player::GetHealthPoints() const
{
	return healthPoints;
}

bool Player::IsAlive() const
{
	return healthPoints > 0;
}

void Player::OnCollision(const Collider& own, const Collider& other)
{
	if (IsAlive()) {
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
}

void Player::Update()
{
	UpdateInvincibility();
	MovePlayer();
	UpdateAnimation();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isFallingToTheFloor && IsAlive() && currentAnimation != &tripOverHazzard)
	{
		ShootLaser();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		healthPoints = 0;

	Render();
}
