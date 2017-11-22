#pragma once

#include "Point.h"
#include "Animation.h"

struct Collider;

class Enemy
{
public:
	virtual Enemy* Clone() const = 0;

public:
	float positionX;
	float positionY;
	float positionZ;

	float speedX;
	float speedY;
	float speedZ;

	bool hasShadow;
	bool to_delete;

	bool canBeDestroyed;
	Collider* collider;

	Animation anim;

protected:
	Enemy(int x, int y, int z, int vx, int vy, int vz, bool hasShadow, bool canBeDestroyed);
	Enemy(const Enemy&);
	virtual ~Enemy();
};

