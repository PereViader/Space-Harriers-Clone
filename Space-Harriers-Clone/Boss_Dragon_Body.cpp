#include "Boss_Dragon_Body.h"



Boss_Dragon_Body::Boss_Dragon_Body() :
	Enemy(new FloorBasedTransform(), true)
{
}


Boss_Dragon_Body::~Boss_Dragon_Body()
{
}

void Boss_Dragon_Body::OnCollision(const Collider & own, const Collider & other)
{
	// nothing
}

void Boss_Dragon_Body::Init(const json & parameters)
{
}

void Boss_Dragon_Body::Update()
{
}

Boss_Dragon_Body * Boss_Dragon_Body::Clone() const
{
	return new Boss_Dragon_Body(*this);
}
