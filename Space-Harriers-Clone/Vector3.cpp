#include "Vector3.h"
#include "Vector2.h"

const Vector3 Vector3::UP(0, 1, 0);
const Vector3 Vector3::FORWARD(0, 0, 1);
const Vector3 Vector3::RIGHT(1, 0, 0);
const Vector3 Vector3::ZERO(0,0,0);

Vector3::Vector3(const Vector2 & v) : Vector3(v.x, v.y)
{
}

Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta) {
	Vector3 ret;
	if (current == target) {
		ret = target;
	}
	else {
		Vector3 totalMovementVector = target - current;
		float distance = totalMovementVector.Magnitude();
		float clampedDistance = min(maxDistanceDelta, distance);
		Vector3 unitMovementVector = totalMovementVector.Normalized();
		ret = current + unitMovementVector * clampedDistance;
	}
	return ret;
}


