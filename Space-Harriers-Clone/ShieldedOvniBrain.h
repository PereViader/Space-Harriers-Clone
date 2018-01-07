#ifndef _SHIELDED_OVNI_BRAIN_H_
#define _SHIELDED_OVNI_BRAIN_H_

#include "Enemy.h"

#include <vector>

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
	ShieldedOvniBrain(const vector<Vector3>& leftPath, const vector<Vector3>& rightPath, const vector<Vector3>& topPath);
	ShieldedOvniBrain(const ShieldedOvniBrain&);
	virtual ~ShieldedOvniBrain();

	virtual void OnCollision(const Collider & own, const Collider & other) override;
	virtual void Init(const json& parameters) override;
	virtual void Update() override;
	virtual Enemy * Clone() const override;

private:
	vector<Vector3> topPath;
	vector<Vector3> leftPath;
	vector<Vector3> rightPath;
};

#endif // !_SHIELDED_OVNI_BRAIN_H_