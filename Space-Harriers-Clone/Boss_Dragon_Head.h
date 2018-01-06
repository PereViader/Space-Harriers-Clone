#ifndef _BOSS_DRAGON_HEAD_
#define _BOSS_DRAGON_HEAD_

#include "Enemy.h"

#include "ScreenBoundFloorProjectedTransform.h"
#include "Texture.h"
#include "Animation.h"
#include "SFX.h"
#include "Vector2.h"
#include "Vector3.h"

class Collider;
class Size2D;
class Boss_Dragon_Body;
class Boss_Dragon;

class Boss_Dragon_Head :
	public Enemy
{
	friend Boss_Dragon;
	friend Boss_Dragon_Body;

public:
	Boss_Dragon_Head(const Texture & graphics, const vector<Animation>& forwardAnimations, const vector<Animation>& backwardAnimations, int healthPoints, const SFX & sfx, const Size2D & size, float scalingFactor, const Vector3& speed, float timeBetweenRounds, float timeBetweenBullets, int bulletsForRound, float bulletSpeed);
	Boss_Dragon_Head(const Boss_Dragon_Head&);

	virtual ~Boss_Dragon_Head();
	virtual Boss_Dragon_Head * Clone() const override;

	virtual void Init(const json & parameters) override;
	virtual void Update() override;


	virtual void Render() override;

	virtual void OnCollision(const Collider & own, const Collider & other) override;

	virtual ScreenBoundFloorProjectedTransform& GetTransform() const override { return GetTransformTypped<ScreenBoundFloorProjectedTransform>(); }
	
private:
	Texture graphics;
	float scalingFactor;
	vector<Animation> forwardAnimations;
	vector<Animation> backwardAnimations;
	SFX sfx;

	Collider * collider;

	int healthPoints;

	Boss_Dragon_Body *previousPart;

	int forward;

	Vector3 speed;
	Vector2 positionAngle;


	const float TIME_BETWEEN_ROUNDS;
	const float TIME_BETWEEN_BULLETS;

	float roundTimer;
	float bulletTimer;
	const int BULLETS_FOR_ROUND;
	int bulletsFired;

	float fireballSpeed;
	
private:
	void UpdatePosition();
	void ShootPlayer();
	void ShootFireballToPlayer();

	void OnBossDragonHeadDied();
	bool IsAlive() const;


	static const int MIN_HEIGHT;
	static const int MAX_HEIGHT;
	static const int MIN_WIDTH;
	static const int MAX_WIDTH;
	static const int MIN_DEPTH;
	static const int MAX_DEPTH;


};

#endif // !_BOSS_DRAGON_HEAD_