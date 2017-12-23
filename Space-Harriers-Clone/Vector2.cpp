#include "Vector2.h"
#include "Vector3.h"

const Vector2 Vector2::UP(0, 1);
const Vector2 Vector2::RIGHT(1, 0);
const Vector2 Vector2::ZERO(0,0);

Vector2::Vector2(const Vector3 & v) : Vector2(v.x, v.y)
{
}

Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta) {
	Vector2 ret;
	if (current == target) {
		ret = target;
	}
	else {
		Vector2 totalMovementVector = target - current;
		float distance = totalMovementVector.Magnitude();
		float clampedDistance = min(maxDistanceDelta, distance);
		Vector2 unitMovementVector = totalMovementVector.Normalized();
		ret = current + unitMovementVector * clampedDistance;
	}
	return ret;
}