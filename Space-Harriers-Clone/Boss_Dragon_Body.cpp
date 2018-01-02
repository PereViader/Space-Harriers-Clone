#include "Boss_Dragon_Body.h"

#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"

#include "Collider.h"

Boss_Dragon_Body::Boss_Dragon_Body(const Texture & graphics, const Animation & animation, const SFX & sfx, const Size2D & size) :
	Enemy(new FloorBasedTransform(), true),
	graphics(graphics),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this))
{
}

Boss_Dragon_Body::~Boss_Dragon_Body()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Boss_Dragon_Body::OnCollision(const Collider & own, const Collider & other)
{
}

void Boss_Dragon_Body::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);

}

void Boss_Dragon_Body::Update()
{
}

Boss_Dragon_Body * Boss_Dragon_Body::Clone() const
{
	return new Boss_Dragon_Body(*this);
}
