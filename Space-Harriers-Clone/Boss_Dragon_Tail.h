#pragma once
#include "Enemy.h"

#include "FloorBasedTransform.h"
#include "Texture.h"
#include "Animation.h"
#include "SFX.h"

class Collider;
class Size2D;

class Boss_Dragon_Tail :
	public Enemy
{
public:
	Boss_Dragon_Tail(const Texture& graphics, const Animation& animation, const SFX& sfx, const Size2D& size);
	virtual ~Boss_Dragon_Tail();
	virtual Boss_Dragon_Tail * Clone() const override;


	virtual void Init(const json & parameters) override;

	virtual void Update() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;

private:
	Texture graphics;
	Animation animation;
	SFX sfx;

	Collider * collider;
};

