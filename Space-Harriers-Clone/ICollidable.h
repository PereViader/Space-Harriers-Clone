#pragma once

class Collider;

class ICollidable {
public:
	virtual void OnCollision(const Collider* own, const Collider* other) = 0;
};