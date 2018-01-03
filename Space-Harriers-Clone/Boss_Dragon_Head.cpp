#include "Boss_Dragon_Head.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "ModuleRender.h"
#include "ModuleEnemy.h"
#include "Collider.h"
#include "Explosion.h"

#include "Boss_Dragon_Body.h"

Boss_Dragon_Head::Boss_Dragon_Head(const Texture & graphics, const Animation & animation, const SFX & sfx, const Size2D & size, float scalingFactor) :
	Enemy(new ScreenBoundFloorProjectedTransform(), true),
	graphics(graphics),
	scalingFactor(scalingFactor),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this)),
	previousPart(nullptr)
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

void Boss_Dragon_Head::Update()
{
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
