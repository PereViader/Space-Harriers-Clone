#pragma once

#include "Point.h"
#include "Animation.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;

class Obstacle :
	public Enemy
{
public:
	Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow, float scalingFactor=1);
	~Obstacle();

	virtual Enemy * Clone() const override;
	virtual void Update() override;

public:
	float scalingFactor;

	SDL_Texture* graphics;
	Animation animation;
	Collider* collider;

private:
	float zSpeed;
	int renderingFloorId;
	
	bool hasShadow;

private:
	void MoveObstacle();
	void RenderObstacle();
	iPoint GetScreenRenderPosition() const;
	float GetScaleForPosition(int y) const;
};

