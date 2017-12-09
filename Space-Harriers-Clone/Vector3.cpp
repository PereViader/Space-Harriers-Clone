#include "Vector3.h"

Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta) {
	Vector3 totalMovementVector = target - current;
	float distance = totalMovementVector.Magnitude();
	float clampedDistance = min(maxDistanceDelta, distance);
	Vector3 unitMovementVector = totalMovementVector.Normalized();
	return current + unitMovementVector * clampedDistance;
}