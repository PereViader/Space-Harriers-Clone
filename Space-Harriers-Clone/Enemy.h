#pragma once

#include "ICollidable.h"
#include "IClonable.h"
#include "IRenderable.h"
#include "IDeletable.h"

#include "Application.h"
#include "ModuleShadow.h"
#include "FloorRelatedTransform.h"

#include <string>
#include <map>


using namespace std;

class Enemy :
	public ICollidable, public IClonable<Enemy*>, public IRenderable, public IDeletable
{
public:
	Enemy(FloorRelatedTransform * transform, bool hasShadow) :
		transform(transform),
		hasShadow(hasShadow)
	{}

	Enemy(const Enemy& other) :
		IDeletable(other),
		transform(other.transform->Clone()),
		hasShadow(other.hasShadow)
	{
		
	}

	virtual ~Enemy() {
		delete transform;
	};

	virtual void Init(map<string, void*> values) = 0;
	virtual void Update() = 0;
	
	virtual void Render() override {
		if (!ToDelete() && hasShadow)
			App->shadow->DrawShadow(*transform);
	}

	FloorRelatedTransform& GetTransform() const { return *transform; }

private:
	FloorRelatedTransform * transform;
	bool hasShadow;
};