#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "ICollidable.h"
#include "GameEntity.h"

#include "Application.h"
#include "ModuleShadow.h"
#include "FloorRelatedTransform.h"

#include <string>
#include <map>

#include "json.hpp"
#include "json_serializer.h"

using nlohmann::json;

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

	virtual void Init(const json& parameters) = 0;

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

#endif // !_ENEMY_H_