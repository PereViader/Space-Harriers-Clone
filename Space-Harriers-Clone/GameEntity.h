#pragma once

#include "IRenderable.h"
#include "IClonable.h"
#include "IDeletable.h"
#include "Transform.h"

class GameEntity : 
	public IRenderable, public IClonable<GameEntity*>, public IDeletable {
protected:
	GameEntity(Transform * transform) : transform(transform) {}
	GameEntity(const GameEntity& other) : transform(other.transform->Clone()) {};

public:

	virtual ~GameEntity() {
		delete transform;
	}

	virtual Transform & GetTransform() const { return *transform; }

private:
	Transform * transform;
};