#include "Explosion.h"

#include "FloorAnchoredTransform.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Pivot2D.h"

Explosion::Explosion(const Texture& graphics, const Animation& animation, float scalingFactor) :
	Enemy(new FloorAnchoredTransform(), false),
	graphics(graphics),
	animation(animation),
	scalingFactor(scalingFactor)
{
}


Explosion::~Explosion()
{
}

void Explosion::OnCollision(const Collider & own, const Collider & other)
{
	// has no collision
}

void Explosion::Init(map<string, void*> values)
{
	// nothing to init
}

void Explosion::Update()
{
	if (GetTransform().GetDepth() <= 0 || animation.Finished()) {
		OnExplosionDied();
	}
}

Explosion * Explosion::Clone() const
{
	return new Explosion(*this);
}

void Explosion::Render()
{
	Enemy::Render();

	if (!ToDelete()) {
		Vector3 screen = GetTransform().GetScreenPositionAndDepth();
		float scale = scalingFactor * GetTransform().GetRenderingScale();
		animation.UpdateFrame();
		graphics.UpdateTexture(animation);
		App->renderer->BlitWithPivotScaledZBuffer(graphics, scale, Pivot2D::BOTTOM_CENTER, screen);
	}
}

void Explosion::OnExplosionDied()
{
	MarkAsDeleted();
}
