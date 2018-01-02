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

	virtual FloorBasedTransform& GetTransform() const override { return GetTransformTypped<FloorBasedTransform>(); }
};

#endif // !_BOSS_DRAGON_H_



