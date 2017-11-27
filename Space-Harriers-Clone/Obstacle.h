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
	Obstacle(const Obstacle&);
	~Obstacle();

	virtual Enemy * Clone() const override;
	virtual void Update() override;

	virtual void Init(map<string, void*> values) override;

public:
	float scalingFactor;

	SDL_Texture* graphics;
	Animation animation;
	Collider* collider;

private:
	float zSpeed;
	int renderingFloorId;
	float xPositionOffset;
	
private:
	void MoveObstacle();
	void RenderObstacle();
	iPoint GetScreenRenderPosition() const;
	float GetScaleForPosition(float y) const;	
};

