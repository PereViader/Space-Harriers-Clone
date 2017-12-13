#pragma once

#include "Enemy.h"

#include "Point.h"
#include "Animation.h"
#include "Texture.h"

class Collider;

class Obstacle :
	public Enemy
{
public:
	Obstacle(const Texture& graphics, const Animation& animation, bool hasShadow, float scalingFactor=1);
	~Obstacle();

	virtual Obstacle* Clone() const override;
	virtual void Update() override;

	virtual void Init(map<string, void*> values) override;

	virtual void OnCollision(const Collider * own, const Collider * other) override;
	virtual void Render() override;

public:
	Texture graphics;
	Animation animation;
	Collider* collider;

private:
	float scalingFactor;
};