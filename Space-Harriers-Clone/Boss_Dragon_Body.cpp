#include "Boss_Dragon_Body.h"

#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleEnemy.h"
#include "Explosion.h"

#include "Collider.h"

Boss_Dragon_Body::Boss_Dragon_Body(const Texture & graphics, const Animation & animation, const SFX & sfx, const Size2D & size, float scalingFactor) :
	Enemy(new ScreenBoundFloorProjectedTransform(), true),
	graphics(graphics),
	scalingFactor(scalingFactor),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	nextPart(nullptr),
	previousPart(nullptr)
{
}

Boss_Dragon_Body::Boss_Dragon_Body(const Boss_Dragon_Body & o) :
	Enemy(o),
	graphics(o.graphics),
	scalingFactor(o.scalingFactor),
	animation(o.animation),
	sfx(o.sfx),
	collider(App->collision->RegisterPrototypeInstance(*o.collider, *this)),
	nextPart(o.nextPart),
	previousPart(o.previousPart)
{
}

Boss_Dragon_Body::~Boss_Dragon_Body()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Boss_Dragon_Body::OnCollision(const Collider & own, const Collider & other)
{
	if (other.colliderType == ColliderType::PlayerParticle) {
		App->audio->PlayFx(sfx);
	}
}

void Boss_Dragon_Body::DragonDied()
{
	OnBossDragonBodyDied();

	if (previousPart)
		previousPart->DragonDied();
}

void Boss_Dragon_Body::OnBossDragonBodyDied()
{
	MarkAsDeleted();

	Explosion * explosion = static_cast<Explosion*>(App->enemies->InstantiateEnemyByName("explosion", json()));
	explosion->GetTransform().SetPosition(GetTransform());
}

void Boss_Dragon_Body::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);
}

void Boss_Dragon_Body::Update()
{
	assert(nextPart);

	const float distanceBetweenBodyParts = 40;
	Vector3 currentOwnPosition = GetTransform().GetScreenPositionAndDepth();
	Vector3 currentNextPartPosition = nextPart->GetTransform().GetScreenPositionAndDepth();

	Vector3 currentNextPartToOldNextPart = oldNextPartPosition - currentNextPartPosition;
	float distanceNextPartAndOldNextPart = currentNextPartToOldNextPart.Magnitude();

	Vector3 oldNextPartToOwn =  currentOwnPosition - oldNextPartPosition;
	float distanceOldNextPartAndOwn = oldNextPartToOwn.Magnitude();

	Vector3 newDesiredPosition;
	if (distanceNextPartAndOldNextPart >= distanceBetweenBodyParts) {
		float percentageOfPosition = distanceBetweenBodyParts / distanceNextPartAndOldNextPart;
		newDesiredPosition = currentNextPartPosition + currentNextPartToOldNextPart * percentageOfPosition;
	}
	else {
		float remainingDistance = distanceBetweenBodyParts - distanceNextPartAndOldNextPart;
		float percentageOfPosition = remainingDistance / distanceOldNextPartAndOwn;
		newDesiredPosition = oldNextPartPosition + oldNextPartToOwn * percentageOfPosition;
	}

	Vector3 deltaNewPosition = newDesiredPosition - currentOwnPosition;
	const float smoothingFactor = 0.5;

	GetTransform().Move(deltaNewPosition*smoothingFactor);

	oldNextPartPosition = currentNextPartPosition;
}

void Boss_Dragon_Body::Render()
{
	Enemy::Render();

	animation.UpdateFrame();
	graphics.UpdateTexture(animation);

	Vector3 position = GetTransform().GetScreenPositionAndDepth();
	float scale = GetTransform().GetRenderingScale() * scalingFactor;

	App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::MIDDLE_CENTER, position);
}

Boss_Dragon_Body * Boss_Dragon_Body::Clone() const
{
	return new Boss_Dragon_Body(*this);
}