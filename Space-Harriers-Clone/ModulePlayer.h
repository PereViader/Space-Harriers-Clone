#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "GameEntity.h"

#include "Texture.h"
#include "Animation.h"

#include "Player.h"

class Collider;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	virtual bool Init() override;
	virtual bool Start() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

	Player& GetPlayer() const;
private:
	Player * player;

};

#endif