#pragma once
#include "Enemy.h"

#include "Texture.h"
#include "Animation.h"

#include "Vector3.h"

#include <vector>
#include <map>
#include <set>

using namespace std;

class Ovni :
	public Enemy
{
public:
	Ovni(const Vector3 startingPosition, float speed, float particleSpeed, const Texture& texture, const Animation& animation, const vector<Vector3>& path, const set<unsigned int>& particleSpawnsIndex, float scalingFactor = 1);
	~Ovni();

	virtual void Init(map<string, void*> values) override;
	virtual void Update() override;

	virtual void OnCollision(const Collider& own, const Collider& other) override;
	virtual Ovni* Clone() const override;
	virtual void Render() override;

public:
	Collider* collider;

private:

	Texture graphics;
	Animation animation;
	float scalingFactor;

	int currentTarget;
	vector<Vector3> path;
	set<unsigned int> particleSpawnsIndex;

	float speed;
	float particleSpeed;
};

