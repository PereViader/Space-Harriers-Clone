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
	previousPart(nullptr),
	delta()
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
	previousPart(o.previousPart),
	delta(o.delta)
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

void Boss_Dragon_Body::SetCurrentDelta(const Vector3 & delta)
{
	if (previousPart) {
		previousPart->SetCurrentDelta(this->delta);
	}

	this->delta = delta;
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
	/*assert(nextPart);
	float speed = 200;
	Vector3 currentPosition = GetTransform().GetScreenPositionAndDepth();
	Vector3 newPosition = MoveTowards(currentPosition, oldNextPartPosition, speed*App->time->GetDeltaTime());
	if (newPosition == oldNextPartPosition) {
		oldNextPartPosition = nextPart->GetTransform().GetScreenPositionAndDepth();
	}
	GetTransform().SetPosition(newPosition);*/
	GetTransform().Move(delta);
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