#ifndef _OVNI_H_
#define _OVNI_H_

#include "Enemy.h"

#include "Texture.h"
#include "Animation.h"
#include "SFX.h"

#include "Vector3.h"

#include <vector>
#include <map>
#include <set>

class Size2D;

using namespace std;

class Ovni :
	public Enemy
{
public:
	Ovni(float speed, float particleSpeed, const Texture& texture, const Animation& animation, const Size2D& size, const SFX& sfx, float scalingFactor);
	Ovni(const Ovni&);
	~Ovni();

	virtual void Init(const json& parameters) override;
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollision(const Collider& own, const Collider& other) override;
	virtual Ovni* Clone() const override;
	virtual void Render() override;

	void SetPathAndBullets(const vector<Vector3>& path, const set<unsigned int>& particleSpawnsIndex);

private:
	Texture graphics;
	Animation animation;
	float scalingFactor;

	Collider* collider;

	int currentTarget;
	vector<Vector3> path;
	set<unsigned int> particleSpawnsIndex;

	float speed;
	float particleSpeed;

	SFX sfx;

private:
	void OnOvniDied();
	void FollowPath();

};

#endif // !_OVNI_H_