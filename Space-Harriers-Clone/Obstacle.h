#pragma once

#include "Point.h"
#include "Enemy.h"

struct SDL_Texture;
struct Collider;

class Obstacle :
	public Enemy
{
public:
	Obstacle(SDL_Texture* graphics, Animation animation, bool hasShadow);
	~Obstacle();

	virtual Enemy * Clone() const override;
	virtual void Update() override;

public:
	SDL_Texture* graphics;
	Animation animation;
	Collider* collider;

private:
	float zSpeed;
	int renderingFloorId;
	
	bool hasShadow;

private:
	void MoveObstacle();
	void RenderObstacle() const;
	iPoint GetScreenRenderPosition() const;
	float GetScaleForPosition(int y) const;
};

