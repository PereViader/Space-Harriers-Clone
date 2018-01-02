#ifndef _BOSS_DRAGON_HEAD_
#define _BOSS_DRAGON_HEAD_

#include "Enemy.h"

#include "FloorBasedTransform.h"
#include "Texture.h"
#include "Animation.h"
#include "SFX.h"

class Collider;
class Size2D;

class Boss_Dragon_Head :
	public Enemy
{
public:
	Boss_Dragon_Head(const Texture& graphics, const Animation& animation, const SFX& sfx, const Size2D& size);
	virtual ~Boss_Dragon_Head();
	virtual Boss_Dragon_Head * Clone() const override;

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

#endif // !_BOSS_DRAGON_HEAD_