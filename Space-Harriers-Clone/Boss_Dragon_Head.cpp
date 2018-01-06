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


Boss_Dragon_Head::Boss_Dragon_Head(const Texture & graphics, const vector<Animation>& forwardAnimations, const vector<Animation>& backwardAnimations, int healthPoints, const SFX & sfx, const Size2D & size, float scalingFactor, const Vector3& speed) :
	Enemy(new ScreenBoundFloorProjectedTransform(), true),
	graphics(graphics),
	scalingFactor(scalingFactor),
	forwardAnimations(forwardAnimations),
	backwardAnimations(backwardAnimations),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	previousPart(nullptr),
	healthPoints(healthPoints),
	forward(1),
	speed(speed),
	positionAngle(static_cast<float>(M_PI/4.0f), static_cast<float>(M_PI/2.0f))
{
	assert(forwardAnimations.size() == healthPoints);
	assert(backwardAnimations.size() == healthPoints);
}

Boss_Dragon_Head::Boss_Dragon_Head(const Boss_Dragon_Head & o) :
	Enemy(o),
	graphics(o.graphics),
	scalingFactor(o.scalingFactor),
	forwardAnimations(o.forwardAnimations),
	backwardAnimations(o.backwardAnimations),
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
		if (!IsAlive()) {
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

bool Boss_Dragon_Head::IsAlive() const
{
	return healthPoints > 0;
}

void Boss_Dragon_Head::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);
}

void Boss_Dragon_Head::Update()
{
	//move angular position
	positionAngle.x = fmod(positionAngle.x + speed.x * App->time->GetDeltaTime(), static_cast<float>(M_PI));
	positionAngle.y = fmod(positionAngle.y + speed.y * App->time->GetDeltaTime(), static_cast<float>(M_PI));

	//calculate New Depth angular position
	float depth = GetTransform().GetDepth() + forward * speed.z * App->time->GetDeltaTime();

	//Project to a position on screen
	Vector3 newPosition;
	newPosition.x = MIN_WIDTH + (MAX_WIDTH - MIN_WIDTH) * sin(positionAngle.x);
	newPosition.y = MIN_HEIGHT + (MAX_HEIGHT - MIN_HEIGHT) * sin(positionAngle.y);
	newPosition.z = depth;

	GetTransform().SetPosition(newPosition);

	// Update if is going forward
	if (forward == 1) { // is Going Forward
		forward = depth <= MIN_DEPTH ? -1 : 1;
	}
	else { // forward == -1 
		forward = depth >= MAX_DEPTH ? 1 : -1;
	}
}

void Boss_Dragon_Head::Render()
{
	if (IsAlive()) {
		Enemy::Render();

		vector<Animation>& currentAnimations = forward == 1 ? forwardAnimations : backwardAnimations;
		Animation& currentAnimation = currentAnimations.at(healthPoints-1);

		currentAnimation.UpdateFrame();
		graphics.UpdateTexture(currentAnimation);

		Vector3 position = GetTransform().GetScreenPositionAndDepth();
		float scale = GetTransform().GetRenderingScale() * scalingFactor;

		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, position);
	}
}

Boss_Dragon_Head * Boss_Dragon_Head::Clone() const
{
	return new Boss_Dragon_Head(*this);
}