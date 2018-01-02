#include "Boss_Dragon_Head.h"

#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleCollision.h"
#include "Collider.h"

Boss_Dragon_Head::Boss_Dragon_Head(const Texture & graphics, const Animation & animation, const SFX & sfx, const Size2D & size) :
	Enemy(new FloorBasedTransform(), true),
	graphics(graphics),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this))
{
}

Boss_Dragon_Head::~Boss_Dragon_Head()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Boss_Dragon_Head::OnCollision(const Collider & own, const Collider & other)
{
	App->audio->PlayFx(sfx);
}

void Boss_Dragon_Head::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);
}

void Boss_Dragon_Head::Update()
{
}

Boss_Dragon_Head * Boss_Dragon_Head::Clone() const
{
	return new Boss_Dragon_Head(*this);
}
