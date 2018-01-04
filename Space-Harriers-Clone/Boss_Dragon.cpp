#include "Boss_Dragon.h"

#include "Application.h"
#include "ModuleEnemy.h"

#include "Boss_Dragon_Head.h"
#include "Boss_Dragon_Body.h"

#include "Vector3.h"

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
	Boss_Dragon_Body * dragonBody[NUMBER_OF_BODY_PARTS+1];

	dragonHead = static_cast<Boss_Dragon_Head*>(App->enemies->InstantiateEnemyByName("bossDragonHead", parameters));

	for (int i = 0; i < NUMBER_OF_BODY_PARTS; ++i) {
		dragonBody[i] = static_cast<Boss_Dragon_Body*>(App->enemies->InstantiateEnemyByName("bossDragonBody", parameters));
	}

	dragonBody[NUMBER_OF_BODY_PARTS] = static_cast<Boss_Dragon_Body*>(App->enemies->InstantiateEnemyByName("bossDragonTail", parameters));

	//Set references between dragon parts
	dragonHead->previousPart = dragonBody[0];
	dragonBody[0]->nextPart = dragonHead;

	for (int i = 0; i < NUMBER_OF_BODY_PARTS; ++i) {
		dragonBody[i]->previousPart = dragonBody[i+1];
		dragonBody[i+1]->nextPart = dragonBody[i];
	}

	//Set starting position
	Vector3 startingPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 700);
	Vector3 startingDelta(0, 0, -30);

	for (int i = NUMBER_OF_BODY_PARTS; i >= 0; --i) {
		dragonBody[i]->GetTransform().SetPosition(startingPosition);
		startingPosition += startingDelta;
	}

	dragonHead->GetTransform().SetPosition(startingPosition);

	//Set next part delta
	dragonBody[0]->oldNextPartPosition = dragonHead->GetTransform().GetScreenPositionAndDepth();

	for (int i = 1; i < NUMBER_OF_BODY_PARTS+1; ++i) {
		dragonBody[i]->oldNextPartPosition = dragonBody[i - 1]->GetTransform().GetScreenPositionAndDepth();
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