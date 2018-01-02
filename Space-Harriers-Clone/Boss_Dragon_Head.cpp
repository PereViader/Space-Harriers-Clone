#include "Boss_Dragon_Head.h"



Boss_Dragon_Head::Boss_Dragon_Head() :
	Enemy(new FloorBasedTransform(), true)
{
}


Boss_Dragon_Head::~Boss_Dragon_Head()
{
}

void Boss_Dragon_Head::OnCollision(const Collider & own, const Collider & other)
{
}

void Boss_Dragon_Head::Init(const json & parameters)
{
}

void Boss_Dragon_Head::Update()
{
}

Boss_Dragon_Head * Boss_Dragon_Head::Clone() const
{
	return new Boss_Dragon_Head(*this);
}
