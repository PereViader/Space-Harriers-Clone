#include "Explosion.h"

#include "FloorAnchoredTransform.h"

Explosion::Explosion(const Texture& graphics, const Animation& animation) :
	Enemy(new FloorAnchoredTransform(), false),
	graphics(graphics),
	animation(animation)
{
}


Explosion::~Explosion()
{
}

void Explosion::OnCollision(const Collider & own, const Collider & other)
{
}

void Explosion::Init(map<string, void*> values)
{
}

void Explosion::Update()
{
}

Explosion * Explosion::Clone() const
{
	return new Explosion(*this);
}
