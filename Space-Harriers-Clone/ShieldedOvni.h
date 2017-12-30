#pragma once
#include "Enemy.h"

#include <list>

#include "Texture.h"
#include "Animation.h"
#include "Vector3.h"
#include "FloorBasedTransform.h"

class Size2D;
class Collider;
class ShieldedOvniBrain;

class ShieldedOvni :
	public Enemy
{
public:
	ShieldedOvni(float speed, float projectileSpeed, const Texture& graphics, const Animation& animation, const Size2D& size, float scalingFactor);
	ShieldedOvni(const ShieldedOvni&);
	virtual ~ShieldedOvni();
	
	virtual void Init(const json& parameters) override;
	virtual void Update() override;


	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual Enemy * Clone() const override;
	virtual FloorBasedTransform& GetTransform() { return GetTransformTypped<FloorBasedTransform>(); }
	virtual void Render() override;

	void SetOpen(bool);
	void SetPath(const list<Vector3>&);
	void SetShieldedOvniBrain(ShieldedOvniBrain&);

private:
	Texture graphics;
	Animation animationOpenClose;
	float scalingFactor;

	Collider * collider;
	bool isOpen;

	float speed;
	list<Vector3> path;

	float projectileSpeed;

	ShieldedOvniBrain* owner;
private:
	void ShootPlayer();
	void OnShieldedOvniDied();
};

