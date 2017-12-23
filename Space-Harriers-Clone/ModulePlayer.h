#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "GameEntity.h"

#include "Texture.h"
#include "Animation.h"

#include "FloorBasedTransform.h"
#include "Vector2.h"

class Collider;

class ModulePlayer : public Module, public GameEntity
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	virtual bool Start() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

	virtual void Render() override;
	virtual void OnCollision(const Collider& own, const Collider& other) override;

	virtual FloorBasedTransform& GetTransform() const override { return GetTransformTypped<FloorBasedTransform>(); }
	Vector2 GetNormalizedPosition() const;
	Vector3 GetChestPosition() const;

private:
	static const float PLAYER_SPEED;

	static const int MAX_HORIZONTAL_POSITION;
	static const int MIN_HORIZONTAL_POSITION;
	static const int MAX_VERTICAL_POSITION;
	static const int MIN_VERTICAL_POSITION;

	static const float TRANSITION_CENTER_RIGHT;
	static const float TRANSITION_CENTER_LEFT;
	static const float TRANSITION_LEFT;
	static const float TRANSITION_RIGHT;

	static const float RENDER_SCALE;

	static const Vector3 PLAYER_PARTICLE_VELOCITY;

	Texture graphics;

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

	bool destroyed;

private:
	Vector2 GetInputMovement() const;
	void ShootLaser();
	void MovePlayer();
	void UpdateAnimation();

	virtual ModulePlayer* Clone() const override;
};

#endif