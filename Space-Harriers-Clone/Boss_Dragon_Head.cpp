#include "Boss_Dragon_Head.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleEnemy.h"
#include "Collider.h"
#include "Explosion.h"

#include <math.h>

#include "Boss_Dragon_Body.h"

const int Boss_Dragon_Head::MIN_HEIGHT = 200;
const int Boss_Dragon_Head::MAX_HEIGHT = SCREEN_HEIGHT - 400;
const int Boss_Dragon_Head::MIN_WIDTH = 200;
const int Boss_Dragon_Head::MAX_WIDTH = SCREEN_WIDTH - 200;
const int Boss_Dragon_Head::MIN_DEPTH = 100;
const int Boss_Dragon_Head::MAX_DEPTH = Z_MAX - 50;


Boss_Dragon_Head::Boss_Dragon_Head(const Texture & graphics, const Animation & animation, const SFX & sfx, const Size2D & size, float scalingFactor) :
	Enemy(new ScreenBoundFloorProjectedTransform(), true),
	graphics(graphics),
	scalingFactor(scalingFactor),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	previousPart(nullptr),
	healthPoints(4),
	forward(1),
	speed(0.4f,1.4f,-50),
	positionAngle(static_cast<float>(M_PI/4.0f), static_cast<float>(M_PI/2.0f))
{
}

Boss_Dragon_Head::Boss_Dragon_Head(const Boss_Dragon_Head & o) :
	Enemy(o),
	graphics(o.graphics),
	scalingFactor(o.scalingFactor),
	animation(o.animation),
	sfx(o.sfx),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	previousPart(o.previousPart),
	healthPoints(o.healthPoints),
	forward(o.forward),
	speed(o.speed),
	positionAngle(o.positionAngle)
{
}

Boss_Dragon_Head::~Boss_Dragon_Head()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Boss_Dragon_Head::OnCollision(const Collider & own, const Collider & other)
{
	if (other.colliderType == ColliderType::PlayerParticle) {
		App->audio->PlayFx(sfx);
		healthPoints--;
		if (healthPoints < 0) {
			OnBossDragonHeadDied();
		}
	}
}

void Boss_Dragon_Head::OnBossDragonHeadDied()
{
	MarkAsDeleted();

	Explosion * explosion = static_cast<Explosion*>(App->enemies->InstantiateEnemyByName("explosion", json()));
	explosion->GetTransform().SetPosition(GetTransform());

	assert(previousPart);
	previousPart->DragonDied();
}

void Boss_Dragon_Head::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);
}


#include <iostream>

void Boss_Dragon_Head::Update()
{
	positionAngle.x = fmod(positionAngle.x + speed.x * App->time->GetDeltaTime(), static_cast<float>(M_PI));
	positionAngle.y = fmod(positionAngle.y + speed.y * App->time->GetDeltaTime(), static_cast<float>(M_PI));
	//std::cout << positionAngle.y << std::endl;

	float depth = GetTransform().GetDepth() + forward * speed.z * App->time->GetDeltaTime();

	Vector3 newPosition;
	newPosition.x = MIN_WIDTH + (MAX_WIDTH - MIN_WIDTH) * sin(positionAngle.x);
	newPosition.y = MIN_HEIGHT + (MAX_HEIGHT - MIN_HEIGHT) * sin(positionAngle.y);
	newPosition.z = depth;

	Vector3 delta = newPosition - GetTransform().GetScreenPositionAndDepth();
	GetTransform().SetPosition(newPosition);


	if (forward == 1) { // is Going Forward
		forward = GetTransform().GetDepth() <= MIN_DEPTH ? -1 : 1;
	}
	else { // forward == -1 
		forward = GetTransform().GetDepth() >= MAX_DEPTH ? 1 : -1;
	}
}

void Boss_Dragon_Head::Render()
{
	Enemy::Render();

	animation.UpdateFrame();
	graphics.UpdateTexture(animation);

	Vector3 position = GetTransform().GetScreenPositionAndDepth();
	float scale = GetTransform().GetRenderingScale() * scalingFactor;

	App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, position);
}

Boss_Dragon_Head * Boss_Dragon_Head::Clone() const
{
	return new Boss_Dragon_Head(*this);
}
