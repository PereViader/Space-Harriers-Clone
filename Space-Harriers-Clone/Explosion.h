#pragma once
#include "Enemy.h"

#include "FloorAnchoredTransform.h"

#include "Texture.h"
#include "Animation.h"

class Explosion :
	public Enemy
{
public:
	Explosion(const Texture& graphics, const Animation& animation, float scalingFactor);
	virtual ~Explosion();

	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual void Init(map<string, void*> values) override;
	virtual void Update() override;
	virtual Explosion * Clone() const override;
	virtual void Render() override;
	virtual FloorAnchoredTransform& GetTransform() { return GetTransformTypped<FloorAnchoredTransform>(); }

private:
	Texture graphics;
	Animation animation;
	float scalingFactor;

private:
	void OnExplosionDied();
};

