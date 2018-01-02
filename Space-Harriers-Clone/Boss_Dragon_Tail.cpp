#include "Boss_Dragon_Tail.h"

#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleAudio.h"
#include "Collider.h"

Boss_Dragon_Tail::Boss_Dragon_Tail(const Texture& graphics, const Animation& animation, const SFX& sfx, const Size2D& size) :
	Enemy(new FloorBasedTransform(), true),
	graphics(graphics),
	animation(animation),
	sfx(sfx),
	collider(App->collision->AddPrototypeCollider(ColliderType::Enemy, size, Pivot2D::MIDDLE_CENTER, *this))
{
}

Boss_Dragon_Tail::~Boss_Dragon_Tail()
{
	App->audio->UnloadFx(sfx);
	collider->MarkAsDeleted();
}

void Boss_Dragon_Tail::OnCollision(const Collider & own, const Collider & other)
{
	App->audio->PlayFx(sfx);
}

void Boss_Dragon_Tail::Init(const json & parameters)
{
	App->audio->RegisterFxUsage(sfx);

}

void Boss_Dragon_Tail::Update()
{
}

Boss_Dragon_Tail * Boss_Dragon_Tail::Clone() const
{
	return new Boss_Dragon_Tail(*this);
}
