#include "ShieldedOvniBrain.h"

#include "ShieldedOvni.h"
#include "FloorBasedTransform.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"

#include <map>

const float ShieldedOvniBrain::TIME_CLOSED = 5.0f;
const float ShieldedOvniBrain::TIME_OPEN = 6.0f;

const Vector3 ShieldedOvniBrain::HORIZON_START(SCREEN_WIDTH/2.0f, 500, Z_MAX);
const Vector3 ShieldedOvniBrain::HORIZON_CLOSER(SCREEN_WIDTH / 2.0f, 500, Z_MAX - 400);
const Vector3 ShieldedOvniBrain::OVNI_LEFT(300, 500, Z_MAX-400);
const Vector3 ShieldedOvniBrain::OVNI_RIGHT(SCREEN_WIDTH - 300, 500, Z_MAX - 400);
const Vector3 ShieldedOvniBrain::OVNI_TOP(SCREEN_WIDTH / 2.0f, 800, Z_MAX - 400);

ShieldedOvniBrain::ShieldedOvniBrain(const list<Vector3>& leftPath, const list<Vector3>& rightPath, const list<Vector3>& topPath) :
	Enemy(new FloorBasedTransform(), false),
	areOpen(false),
	currentTime(0),
	leftPath(leftPath),
	rightPath(rightPath),
	topPath(topPath)
{
}

ShieldedOvniBrain::ShieldedOvniBrain(const ShieldedOvniBrain & o) :
	Enemy(o),
	areOpen(o.areOpen),
	currentTime(o.currentTime),
	leftPath(o.leftPath),
	rightPath(o.rightPath),
	topPath(o.topPath)
{
}

ShieldedOvniBrain::~ShieldedOvniBrain()
{
}

void ShieldedOvniBrain::OnCollision(const Collider & own, const Collider & other)
{
	//Has no collision
}

void ShieldedOvniBrain::Init(map<string, void*> values)
{
	ShieldedOvni *topOvni, *leftOvni, *rightOvni;
	topOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", map<string, void*>()));
	leftOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", map<string, void*>()));
	rightOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", map<string, void*>()));

	ovnis.push_back(topOvni);
	ovnis.push_back(leftOvni);
	ovnis.push_back(rightOvni);


	topOvni->SetPath(topPath);
	leftOvni->SetPath(leftPath);
	rightOvni->SetPath(rightPath);
}

void ShieldedOvniBrain::Update()
{
	currentTime += App->time->GetDeltaTime();

	float TIME_CHECK = areOpen ? TIME_OPEN : TIME_CLOSED;

	if (currentTime >= TIME_CHECK) {
		currentTime -= TIME_CHECK;
 		areOpen = !areOpen;
		for (ShieldedOvni* ovni : ovnis) {
			ovni->SetOpen(areOpen);
		}
	}
}

Enemy * ShieldedOvniBrain::Clone() const
{
	return new ShieldedOvniBrain(*this);
}
