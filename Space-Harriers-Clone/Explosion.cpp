#include "Explosion.h"

#include "FloorAnchoredTransform.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Pivot2D.h"
#include "ModuleAudio.h"

Explosion::Explosion(const Texture& graphics, const Animation& animation, float scalingFactor, const SFX& explosionSound) :
	Enemy(new FloorAnchoredTransform(), false),
	graphics(graphics),
	animation(animation),
	scalingFactor(scalingFactor),
	explosionSound(explosionSound),
	isFirstFrame(true)
{
}


Explosion::~Explosion()
{
}

void Explosion::OnCollision(const Collider & own, const Collider & other)
{
	// has no collision
}

void Explosion::Init(const json& parameters)
{
	// nothing to init
}

void Explosion::Update()
{
	if (GetTransform().GetDepth() <= 0 || animation.Finished()) {
		OnExplosionDied();
	} else if (isFirstFrame) {
		App->audio->PlayFx(explosionSound);
		isFirstFrame = false;
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
