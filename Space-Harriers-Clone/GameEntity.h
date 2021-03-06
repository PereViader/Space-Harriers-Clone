#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "IRenderable.h"
#include "IClonable.h"
#include "IDeletable.h"
#include "ICollidable.h"
#include "Transform.h"

class GameEntity :
	public IRenderable, public ICollidable, public IClonable<GameEntity*>, public IDeletable {
protected:
	GameEntity(Transform * transform) : transform(transform) {
	}
	GameEntity(const GameEntity& other) : transform(other.transform->Clone()) {};

public:

	virtual ~GameEntity() {
		delete transform;
	}

	virtual void Start() {

	}

	virtual void Update() {

	}

	virtual void OnDestroy() {

	}

	virtual Transform & GetTransform() const { return *transform; }

protected:
	template <typename T>
	inline T& GetTransformTypped() const { return static_cast<T&>(*transform); }

private:
	Transform * transform;
};

#endif // !_GAME_ENTITY_H_