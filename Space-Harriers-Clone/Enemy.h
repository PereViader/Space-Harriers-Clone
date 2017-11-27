#pragma once

#include "Point.h"
#include "Animation.h"

#include <string>
#include <map>

using namespace std;

struct Collider;

class Enemy
{
public:
	Enemy(bool hasShadow, bool toDelete) :
		toDelete(toDelete),
		hasShadow(hasShadow) 
	{}

	virtual ~Enemy() {};

	virtual void Init(map<string, void*> values) = 0;
	virtual Enemy* Clone() const = 0;
	virtual void Update() = 0;
	
	bool ToDelete() const { return toDelete; }

protected:
	bool toDelete;
	bool hasShadow;

};

