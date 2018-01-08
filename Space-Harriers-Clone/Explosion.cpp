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
	explosionSound(explosionSound)
{
}


Explosion::~Explosion()
{
	App->audio->UnloadFx(explosionSound);
}

void Explosion::OnCollision(const Collider & own, const Collider & other)
{
	// has no collision
}

void Explosion::Init(const json& parameters)
{
	App->audio->RegisterFxUsage(explosionSound);
	// nothing to init
}

void Explosion::Start()
{
	App->audio->PlayFx(explosionSound);
}

void Explosion::Update()
{
	if (GetTransform().GetDepth() <= 0 || animation.Finished()) {
		OnExplosionEnded();
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

void Explosion::OnExplosionEnded()
{
	MarkAsDeleted();
}
