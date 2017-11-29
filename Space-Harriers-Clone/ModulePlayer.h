#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "ICollidable.h"

#include "Animation.h"
#include "Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module, public ICollidable
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	void UpdateAnimation();

	const fPoint& GetNormalizedPosition() const;
	iPoint GetScreenPosition() const;

	// Inherited via ICollidable
	virtual void OnCollision(const Collider * own, const Collider * other) override;

public:

	

	bool destroyed = false;

private:
	fPoint position;
	static const float PLAYER_SPEED;
	static const float PLAYER_RECOVER_SPEED;

	static const int MAX_HORIZONTAL_POSITION;
	static const int MIN_HORIZONTAL_POSITION;
	static const int MAX_VERTICAL_POSITION;
	static const int MIN_VERTICAL_POSITION;

	static const float TRANSITION_CENTER_RIGHT;
	static const float TRANSITION_CENTER_LEFT;
	static const float TRANSITION_LEFT;
	static const float TRANSITION_RIGHT;

	static const float RENDER_SCALE;


	SDL_Texture* graphics = nullptr;

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
	void MoveCollider();
	void RenderPlayer();

	
};

#endif