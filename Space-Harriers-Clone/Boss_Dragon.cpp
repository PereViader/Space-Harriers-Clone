#include "Boss_Dragon.h"

#include "Application.h"
#include "ModuleEnemy.h"

#include "Boss_Dragon_Head.h"
#include "Boss_Dragon_Body.h"

const int Boss_Dragon::NUMBER_OF_BODY_PARTS = 4;

Boss_Dragon::Boss_Dragon() :
	Enemy(new FloorBasedTransform(), false)
{
}


Boss_Dragon::~Boss_Dragon()
{
	//nothing
}

void Boss_Dragon::Init(const json & parameters)
{
	Boss_Dragon_Head * dragonHead;
	Boss_Dragon_Body * dragonBody[NUMBER_OF_BODY_PARTS];

	dragonHead = static_cast<Boss_Dragon_Head*>(App->enemies->InstantiateEnemyByName("dragonHead", parameters));

	for (Boss_Dragon_Body *& dragonBodyPart : dragonBody) {
		dragonBodyPart = static_cast<Boss_Dragon_Body*>(App->enemies->InstantiateEnemyByName("dragonBody", parameters));
	}

	//Set references between dragon parts
	dragonHead->previousPart = dragonBody[0];
	dragonBody[0]->nextPart = dragonHead;

	for (int i = 0; i < NUMBER_OF_BODY_PARTS - 1; ++i) {
		dragonBody[i]->previousPart = dragonBody[i+1];
		dragonBody[i+1]->nextPart = dragonBody[i];
	}

	MarkAsDeleted();
}

void Boss_Dragon::OnCollision(const Collider & own, const Collider & other)
{
	//nothing
}

void Boss_Dragon::Update()
{
	//nothing
}

Enemy * Boss_Dragon::Clone() const
{
	return new Boss_Dragon(*this);
}