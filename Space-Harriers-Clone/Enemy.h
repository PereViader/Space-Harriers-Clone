#pragma once

#include "ICollidable.h"
#include "IClonable.h"

#include "Point.h"
#include "Animation.h"
#include "Transform.h"


#include <string>
#include <map>


using namespace std;

class Enemy :
	public ICollidable, public IClonable<Enemy*>
{
public:
	Enemy(Transform * transform, bool hasShadow, bool toDelete = false) :
		transform(transform),
		toDelete(toDelete),
		hasShadow(hasShadow)
	{}

	virtual ~Enemy() {
		delete transform;
	};

	virtual void Init(map<string, void*> values) = 0;
	virtual void Update() = 0;
	
	bool ToDelete() const { return toDelete; }

protected:
	Transform * transform;
	bool toDelete;
	bool hasShadow;
};

