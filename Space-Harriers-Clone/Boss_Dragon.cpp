#include "Boss_Dragon.h"

#include "Application.h"

Boss_Dragon::Boss_Dragon() :
	Enemy(new FloorBasedTransform(), false)
{
}


Boss_Dragon::~Boss_Dragon()
{
}
