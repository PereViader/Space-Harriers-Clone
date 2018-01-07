#pragma once
#include "Enemy.h"

#include <vector>

#include "Texture.h"
#include "Animation.h"
#include "Vector3.h"
#include "FloorBasedTransform.h"
#include "SFX.h"

class Size2D;
class Collider;

using namespace std;

class ShieldedOvni :
	public Enemy
{
public:
	ShieldedOvni(float speed, float projectileSpeed, const SFX& sfx, const SFX& hitClosedSFX, const Texture& graphics, const Animation& animation, const Size2D& size, float scalingFactor, float timeOpen, float timeClosed, int stateSwitchesToLeave);
	ShieldedOvni(const ShieldedOvni&);
	virtual ~ShieldedOvni();
	
	virtual void Init(const json& parameters) override;
	virtual void Update() override;


	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual Enemy * Clone() const override;
	virtual FloorBasedTransform& GetTransform() { return GetTransformTypped<FloorBasedTransform>(); }
	virtual void Render() override;

	void SwitchState();
	void SetPath(const vector<Vector3>&);

private:
	enum class behaviour_state {
		In,
		Shoot,
		Out
	};

	behaviour_state state;

	Texture graphics;
	Animation animation;
	float scalingFactor;

	Collider * collider;
	bool isOpen;
	int stateSwitchesToLeave;
	int nStateSwitches;
	float currentTime;
	float timeOpen;
	float timeClosed;

	float speed;
	vector<Vector3> path;
	size_t nextPositionIndex;

	float projectileSpeed;

	SFX sfx;
	SFX hitClosedSFX;
	bool isFirstFrame;

private:
	void ShootPlayer();
	void OnShieldedOvniDied();
};