#ifndef _BOSS_DRAGON_BODY_H
#define _BOSS_DRAGON_H_

#include "Enemy.h"

#include "ScreenBoundFloorProjectedTransform.h"
#include "Texture.h"
#include "SFX.h"
#include "Animation.h"

class Size2D;
class Collider;
class Boss_Dragon;

class Boss_Dragon_Body :
	public Enemy
{
	friend Boss_Dragon;

public:
	Boss_Dragon_Body(const Texture& graphics, const Animation& animation, const SFX& sfx, const Size2D& size, float scalingFactor);
	Boss_Dragon_Body(const Boss_Dragon_Body&);
	virtual ~Boss_Dragon_Body();
	virtual Boss_Dragon_Body * Clone() const override;

	virtual void Init(const json & parameters) override;
	virtual void Update() override;

	virtual void Render() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;

	virtual ScreenBoundFloorProjectedTransform& GetTransform() const override { return GetTransformTypped<ScreenBoundFloorProjectedTransform>(); }
	
	void DragonDied();

private:
	Texture graphics;
	float scalingFactor;
	Animation animation;
	SFX sfx;

	Collider *collider;

	Vector3 oldNextPartPosition;

	Enemy *nextPart;
	Boss_Dragon_Body *previousPart;

private:
	void OnBossDragonBodyDied();

};

#endif // !_BOSS_DRAGON_BODY_H