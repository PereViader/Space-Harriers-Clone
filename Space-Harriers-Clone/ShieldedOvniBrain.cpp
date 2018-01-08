#include "ShieldedOvniBrain.h"

#include "ShieldedOvni.h"
#include "FloorBasedTransform.h"
#include "ModuleFloor.h"
#include "ModuleEnemy.h"

#include <map>

ShieldedOvniBrain::ShieldedOvniBrain(const vector<Vector3>& leftPath, const vector<Vector3>& rightPath, const vector<Vector3>& topPath) :
	Enemy(new FloorBasedTransform(), false),
	leftPath(leftPath),
	rightPath(rightPath),
	topPath(topPath)
{
}

ShieldedOvniBrain::ShieldedOvniBrain(const ShieldedOvniBrain & o) :
	Enemy(o),
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

	topOvni->SetPath(topPath);
	leftOvni->SetPath(leftPath);
	rightOvni->SetPath(rightPath);

	MarkAsDeleted();
}

Enemy * ShieldedOvniBrain::Clone() const
{
	return new ShieldedOvniBrain(*this);
}