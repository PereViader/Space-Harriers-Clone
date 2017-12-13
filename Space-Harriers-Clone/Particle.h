#pragma once

#include "ICollidable.h"
#include "IClonable.h"

#include "Animation.h"
#include "Vector3.h"
#include "ScreenBoundTransform.h"

class Collider;

class Particle :
	public ICollidable, public IClonable<Particle*>
{
public:
	bool to_delete;
	bool isFirstFrame;

	Animation anim;
	int sfxId;

	Vector3 velocity;

	ScreenBoundTransform transform;

	Collider* collider;

	Particle();
	Particle(const Particle& p);
	~Particle();

	void Update();

	virtual void OnCollision(const Collider * own, const Collider * other) override;
	virtual Particle* Clone() const override;
private:
	void MoveParticle();
};