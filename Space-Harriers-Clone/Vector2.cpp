#include "Vector2.h"

Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta) {
	Vector2 totalMovementVector = target - current;
	float distance = totalMovementVector.Magnitude();
	float clampedDistance = min(maxDistanceDelta, distance);
	Vector2 unitMovementVector = totalMovementVector.Normalized();
	return current + unitMovementVector * clampedDistance;
}