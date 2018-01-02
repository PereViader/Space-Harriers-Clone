#ifndef _BOSS_DRAGON_BODY_H
#define _BOSS_DRAGON_H_

#include "Enemy.h"

#include "FloorBasedTransform.h"
#include "Texture.h"
#include "SFX.h"
#include "Animation.h"

class Size2D;
class Collider;

class Boss_Dragon_Body :
	public Enemy
{
public:
	Boss_Dragon_Body(const Texture& graphics, const Animation& animation, const SFX& sfx, const Size2D& size);
	virtual ~Boss_Dragon_Body();
	virtual Boss_Dragon_Body * Clone() const override;

	virtual void Init(const json & parameters) override;
	virtual void Update() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;

	virtual FloorBasedTransform& GetTransform() const override { return GetTransformTypped<FloorBasedTransform>(); }


private:
	Texture graphics;
	Animation animation;
	SFX sfx;

	Collider * collider;

};

#endif // !_BOSS_DRAGON_BODY_H