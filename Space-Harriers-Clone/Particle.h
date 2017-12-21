#pragma once

#include "ICollidable.h"
#include "GameEntity.h"

#include "Animation.h"
#include "Vector3.h"
#include "Texture.h"
#include "SFX.h"
#include "ModuleCollision.h"
#include "ScreenBoundFloorProjectedTransform.h"

class Collider;
class Size2D;

class Particle :
	public GameEntity
{
public:
	Particle(const ColliderType& particleType, const Animation& animation, const Size2D & size, const SFX& sfx, const Texture& texture);
	Particle(const Particle& p);
	~Particle();
	void Update();

	virtual void OnCollision(const Collider& own, const Collider& other) override;
	virtual Particle* Clone() const override;
	virtual void Render() override;
	virtual ScreenBoundFloorProjectedTransform& GetTransform() const override { return GetTransformTypped<ScreenBoundFloorProjectedTransform>(); }

	void SetVelocity(const Vector3& velocity);
private:
	Texture graphics;
	SFX sfx;
	Animation animation;

	Collider* collider;

	bool isFirstFrame;
	Vector3 velocity;

	static const float MINIMUM_HEIGHT_FOR_SHADOW;
private:
	void MoveParticle();
};