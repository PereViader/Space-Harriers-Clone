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

	virtual Obstacle* Clone() const override;
	virtual void Update() override;

	virtual void Init(map<string, void*> values) override;

	virtual void OnCollision(const Collider * own, const Collider * other) override;

public:
	SDL_Texture* graphics;
	Animation animation;
	Collider* collider;

private:
	float scalingFactor;
	
private:
	float GetScaleForPosition(float y) const;
};

