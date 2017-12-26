#pragma once

#include "ICollidable.h"
#include "GameEntity.h"

#include "Application.h"
#include "ModuleShadow.h"
#include "FloorRelatedTransform.h"

#include <string>
#include <map>


using namespace std;

class Enemy :
	public GameEntity
{
public:
	Enemy(FloorRelatedTransform * transform, bool hasShadow) :
		GameEntity(transform),
		hasShadow(hasShadow)
	{}

	virtual ~Enemy() {};

	virtual void Init(map<string, void*> values) = 0;
	virtual void Update() = 0;
	
	virtual void Render() override {
		if (!ToDelete() && hasShadow)
			App->shadow->DrawShadow(GetTransform());
	}

	virtual FloorRelatedTransform& GetTransform() const override { 
		return GetTransformTypped<FloorRelatedTransform>();
	}

	virtual Enemy * Clone() const override = 0;

private:
	bool hasShadow;
};