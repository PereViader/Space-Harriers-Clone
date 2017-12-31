#include "ShieldedOvniBrain.h"

#include "ShieldedOvni.h"
#include "FloorBasedTransform.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"

#include <map>

const float ShieldedOvniBrain::TIME_CLOSED = 5.0f;
const float ShieldedOvniBrain::TIME_OPEN = 6.0f;

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

void ShieldedOvniBrain::Init(const json& parameters)
{
	ShieldedOvni *topOvni, *leftOvni, *rightOvni;
	topOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", parameters));
	leftOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", parameters));
	rightOvni = static_cast<ShieldedOvni*>(App->enemies->InstantiateEnemyByName("shieldedOvni", parameters));

	ovnis.push_back(topOvni);
	ovnis.push_back(leftOvni);
	ovnis.push_back(rightOvni);

	topOvni->SetPath(topPath);
	leftOvni->SetPath(leftPath);
	rightOvni->SetPath(rightPath);

	topOvni->SetShieldedOvniBrain(*this);
	leftOvni->SetShieldedOvniBrain(*this);
	rightOvni->SetShieldedOvniBrain(*this);
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

void ShieldedOvniBrain::OnShieldedOvniDied(const ShieldedOvni & shieldedOvni)
{
	for (auto it = ovnis.begin(); it != ovnis.end(); ++it) {
		if (*it == &shieldedOvni) {
			ovnis.erase(it);
			break;
		}
	}

	if (ovnis.size() == 0) {
		OnShieldedOvniBrainDied();
	}
}

void ShieldedOvniBrain::OnShieldedOvniBrainDied()
{
	MarkAsDeleted();
}
