#pragma once

#include "Enemy.h"

#include "Point.h"
#include "Animation.h"
#include "Texture.h"


class Collider;
class Size2D;

class Obstacle :
	public Enemy
{
public:
	Obstacle(const Texture& graphics, const Animation& animation, bool hasShadow, const Size2D& size, float scalingFactor, bool isNonDamaging);
	Obstacle(const Obstacle&);
	~Obstacle();

	virtual Obstacle* Clone() const override;
	virtual void Update() override;

	virtual void Init(map<string, void*> values) override;

	virtual void OnCollision(const Collider& own, const Collider& other) override;
	virtual void Render() override;

private:
	Texture graphics;
	Animation animation;
	Collider* collider;
	float scalingFactor;

private:
	Vector3 GetMovement() const;
	void OnObstacleDied();

};