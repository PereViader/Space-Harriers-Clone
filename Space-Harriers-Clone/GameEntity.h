#pragma once

#include "IRenderable.h"
#include "IClonable.h"
#include "IDeletable.h"
#include "ICollidable.h"
#include "Transform.h"

class GameEntity : 
	public IRenderable, public ICollidable, public IClonable<GameEntity*>, public IDeletable {
protected:
	GameEntity(Transform * transform) : transform(transform) {}
	GameEntity(const GameEntity& other) : transform(other.transform->Clone()) {};

public:

	virtual ~GameEntity() {
		delete transform;
	}

	virtual Transform & GetTransform() const { return *transform; }

protected:
	template <typename T>
	T& GetTransformTypped() const { return static_cast<T&>(*transform); }

private:
	Transform * transform;
};