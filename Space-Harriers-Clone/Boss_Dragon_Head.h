#pragma once
#include "Enemy.h"

#include "FloorBasedTransform.h"

class Boss_Dragon_Head :
	public Enemy
{
public:
	Boss_Dragon_Head();
	virtual ~Boss_Dragon_Head();
	virtual Boss_Dragon_Head * Clone() const override;

	virtual void Init(const json & parameters) override;
	virtual void Update() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;
};

