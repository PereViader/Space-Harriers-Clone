#include "Enemy.h"


Enemy::Enemy(int x, int y, int z, int vx, int vy, int vz, bool hasShadow, bool canBeDestroyed) :
	positionX(x), positionY(y), positionZ(z),
	speedX(vx), speedY(vy), speedZ(vz),
	hasShadow(hasShadow),
	to_delete(false),
	canBeDestroyed(canBeDestroyed)
{
}

Enemy::Enemy(const Enemy & other) :
	positionX(other.positionX), positionY(other.positionY), positionZ(other.positionZ),
	speedX(other.speedX), speedY(other.speedY), speedZ(other.speedZ),
	hasShadow(other.hasShadow), 
	to_delete(other.to_delete),
	canBeDestroyed(other.canBeDestroyed),
	collider(nullptr), //TODO
	anim(other.anim)

{
}


Enemy::~Enemy()
{
}


