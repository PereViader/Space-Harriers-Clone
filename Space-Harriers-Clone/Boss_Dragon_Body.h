#pragma once
#include "Enemy.h"

#include "FloorBasedTransform.h"

class Boss_Dragon_Body :
	public Enemy
{
public:
	Boss_Dragon_Body();
	virtual ~Boss_Dragon_Body();
	virtual Boss_Dragon_Body * Clone() const override;

	virtual void Init(const json & parameters) override;
	virtual void Update() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;

};

