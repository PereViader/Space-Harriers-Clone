#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "Enemy.h"

#include "Point.h"
#include "Animation.h"
#include "Texture.h"
#include "FloorAnchoredTransform.h"

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
	
	virtual void Init(const json& parameters) override;
	virtual void Update() override;
	virtual void OnDestroy() override;
	virtual void Render() override;

	virtual void OnCollision(const Collider& own, const Collider& other) override;
	virtual FloorAnchoredTransform& GetTransform() { return GetTransformTypped<FloorAnchoredTransform>(); }

private:
	Texture graphics;
	Animation animation;
	Collider* collider;
	float scalingFactor;

private:
	Vector3 GetMovement() const;

};

#endif // !_OBSTACLE_H_