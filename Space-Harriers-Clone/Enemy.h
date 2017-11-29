#pragma once

#include "ICollidable.h"

#include "Point.h"
#include "Animation.h"

#include <string>
#include <map>

using namespace std;

class Enemy :
	public ICollidable
{
public:
	Enemy(bool hasShadow, bool toDelete = false) :
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

