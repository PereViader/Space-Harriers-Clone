#ifndef _BOSS_DRAGON_H_
#define _BOSS_DRAGON_H_

#include "Enemy.h"
#include "FloorBasedTransform.h"

class Boss_Dragon :
	public Enemy
{
public:
	Boss_Dragon();
	virtual ~Boss_Dragon();
	virtual Enemy * Clone() const override;


	virtual void Init(const json& parameters) override;
	virtual void Update() override;

	virtual FloorBasedTransform& GetTransform() const override { return GetTransformTypped<FloorBasedTransform>(); }

	virtual void OnCollision(const Collider & own, const Collider & other) override;

private:
	static const int NUMBER_OF_BODY_PARTS;

};

#endif // !_BOSS_DRAGON_H_