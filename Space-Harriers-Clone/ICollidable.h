#ifndef _ICOLLIDABLE_H__
#define _ICOLLIDABLE_H__

class Collider;

class ICollidable {
public:
	virtual void OnCollision(const Collider& own, const Collider& other) = 0;
};

#endif // !_ICOLLIDABLE_H__