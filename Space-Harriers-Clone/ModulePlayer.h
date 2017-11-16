#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start() override;
	update_status Update() override;
	bool CleanUp() override;

	const fPoint& GetPosition() const;
public:

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


	bool destroyed = false;

private:
	fPoint position;
	float speed;
	float recoverSpeed;
};

#endif