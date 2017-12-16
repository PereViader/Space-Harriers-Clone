#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "IRenderable.h"
#include "ICollidable.h"

#include "Texture.h"
#include "Animation.h"

#include "ScreenBoundTransform.h"
#include "Vector2.h"

class Collider;

class ModulePlayer : public Module, public ICollidable, public IRenderable
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	virtual bool Start() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

	virtual void Render() override;
	virtual void OnCollision(const Collider& own, const Collider& other) override;

	Transform& GetTransform() { return transform; }
	Vector2 GetNormalizedPosition() const;
	Vector3 GetChestPosition() const;

public:
	bool destroyed = false;

private:
	static const float PLAYER_SPEED;
	//static const float PLAYER_RECOVER_SPEED;

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


	ScreenBoundTransform transform;

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

private:
	void ShootLaser();
	void MovePlayer();
	void UpdateAnimation();
};

#endif