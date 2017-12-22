#pragma once
#include "Enemy.h"

#include <vector>

#include "Vector3.h"

class ShieldedOvni;
class Size2D;
class Animation;
class Texture;

class ShieldedOvniBrain :
	public Enemy
{
public:
	ShieldedOvniBrain(float speed, const Texture& graphics, const Animation& animation, const Size2D& size, float scalingFactor);
	ShieldedOvniBrain(const ShieldedOvniBrain&);
	virtual ~ShieldedOvniBrain();

	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual void Init(map<string, void*> values) override;
	virtual void Update() override;
	virtual Enemy * Clone() const override;
private:
	vector<ShieldedOvni*> ovnis;

	bool areOpen;
	float currentTime;
	static const float TIME_CLOSED;
	static const float TIME_OPEN;

	static const Vector3 HORIZON_START;
	static const Vector3 HORIZON_CLOSER;
	static const Vector3 OVNI_LEFT;
	static const Vector3 OVNI_RIGHT;
	static const Vector3 OVNI_TOP;
};

