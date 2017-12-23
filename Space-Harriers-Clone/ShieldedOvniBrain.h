#pragma once
#include "Enemy.h"

#include <list>

#include "Vector3.h"

using namespace std;

class ShieldedOvni;
class Size2D;
class Animation;
class Texture;

class ShieldedOvniBrain :
	public Enemy
{
public:
	ShieldedOvniBrain(float speed, const Texture& graphics, const Animation& animation, const Size2D& size, float scalingFactor);
	ShieldedOvniBrain(const list<Vector3>& leftPath, const list<Vector3>& rightPath, const list<Vector3>& topPath);
	ShieldedOvniBrain(const ShieldedOvniBrain&);
	virtual ~ShieldedOvniBrain();

	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual void Init(map<string, void*> values) override;
	virtual void Update() override;
	virtual Enemy * Clone() const override;

	void OnShieldedOvniDied(const ShieldedOvni&);

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

	list<Vector3> topPath;
	list<Vector3> leftPath;
	list<Vector3> rightPath;

private:

	void OnShieldedOvniBrainDied();
};

