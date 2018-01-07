#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameEntity.h"

#include "FloorBasedTransform.h"
#include "Texture.h"
#include "SFX.h"
#include "Animation.h"

class Player :
	public GameEntity
{

public:
	Player();
	~Player();

	virtual Player* Clone() const override;
	void Update();
	virtual void Render() override;
	virtual void OnCollision(const Collider& own, const Collider& other) override;

	Vector2 GetNormalizedPosition() const;
	Vector3 GetChestPosition() const;

	int GetHealthPoints() const;
	bool IsAlive() const;

	virtual FloorBasedTransform& GetTransform() const override { return GetTransformTypped<FloorBasedTransform>(); }


private:
	//Screen Bounds
	static const int MAX_HORIZONTAL_POSITION;
	static const int MIN_HORIZONTAL_POSITION;
	static const int MAX_VERTICAL_POSITION;
	static const int MIN_VERTICAL_POSITION;

	//Animation transitions
	static const float TRANSITION_CENTER_RIGHT;
	static const float TRANSITION_CENTER_LEFT;
	static const float TRANSITION_LEFT;
	static const float TRANSITION_RIGHT;

	// Position and rendering details
	static const float RENDER_SCALE;
	static const Vector3 PLAYER_GUN_POSITION_DELTA;
	static const Vector3 PLAYER_CHEST_POSITION_DELTA;

	//Behaviour "parameters"
	static const float PLAYER_SPEED;
	static const Vector3 PLAYER_PARTICLE_VELOCITY;
	static const float INVINCIBLE_TIME_AFTER_INTERACTION;
	static const float FALL_SPEED;
	static const int STARTING_HEALTH_POINTS;


	Texture graphics;

	SFX ouchSFX;
	SFX aaaaarghSFX;

	Animation * currentAnimation;
	Animation ground_running;
	Animation hover_center;
	Animation hover_left;
	Animation hover_left_most;
	Animation hover_right;
	Animation hover_right_most;
	Animation takeDamage;
	Animation tripOverHazzard;

	Collider* collider;

	float invincibleTime;
	int healthPoints;

	bool destroyed;
	bool isInvincible;
	bool isFallingToTheFloor;

private:
	Vector2 GetInputMovement() const;
	void ShootLaser();
	void MovePlayer();
	void FallToTheFloorPlayerMovement();
	void RegularPlayerMovement();
	void UpdateAnimation();
	void UpdateInvincibility();
};

#endif // !_PLAYER_H_