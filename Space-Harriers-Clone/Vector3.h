#ifndef _VECTOR3_H
#define _VECTOR3_H

#include <cmath>
#include <algorithm>

class Vector3;
class Vector2;

using namespace std;

Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
Vector3& operator*=(Vector3& lhs, float value);
Vector3& operator/=(Vector3& lhs, float value);

bool operator==(const Vector3& lhs, const Vector3& rhs);
bool operator!=(const Vector3& lhs, const Vector3& rhs);

Vector3 operator*(float d, const Vector3& v);
Vector3 operator*(const Vector3& v, float d);

Vector3 operator/(const Vector3& v, float d);
Vector3 operator/(float d, const Vector3& v);

float Dot(const Vector3& a, const Vector3& b);

Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t);

Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta);


class Vector3 {
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(float x, float y) : x(x), y(y), z(0) {}
	Vector3(const Vector2&);

	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& other) const {
		Vector3 ret(*this);
		ret.x += other.x;
		ret.y += other.y;
		ret.z += other.z;
		return ret;
	}

	Vector3 operator-(const Vector3& other) const {
		Vector3 ret(*this);
		ret.x -= other.x;
		ret.y -= other.y;
		ret.z -= other.z;
		return ret;
	}

	float Magnitude() const {
		return sqrt(SqrMagnitude());
	}

	float SqrMagnitude() const {
		return Dot(*this, *this);
	}

	void Normalize() {
		*this = Normalized();
	}

	Vector3 Normalized() const {
		return operator/(*this, Magnitude());
	}

	static const Vector3 UP;
	static const Vector3 FORWARD;
	static const Vector3 RIGHT;
	static const Vector3 ZERO;

};

inline Vector3& operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

inline Vector3& operator-=(Vector3& lhs, const Vector3& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

inline Vector3& operator*=(Vector3& lhs, float value) {
	lhs.x *= value;
	lhs.y *= value;
	lhs.z *= value;
	return lhs;
}

inline Vector3& operator/=(Vector3& lhs, float value) {
	lhs.x /= value;
	lhs.y /= value;
	lhs.z /= value;
	return lhs;
}

inline bool operator==(const Vector3& lhs, const Vector3& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs) {
	return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

inline Vector3 operator*(float d, const Vector3& v) {
	Vector3 ret(v);
	ret.x *= d;
	ret.y *= d;
	ret.z *= d;
	return ret;
}

inline Vector3 operator*(const Vector3& v, float d) {
	return operator*(d, v);
}

inline Vector3 operator/(float d, const Vector3& v) {
	Vector3 ret(v);
	ret.x /= d;
	ret.y /= d;
	ret.z /= d;
	return ret;
}

inline Vector3 operator/(const Vector3& v, float d) {
	return operator/(d, v);
}


inline float Dot(const Vector3& a, const Vector3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	t = max(0.0f, min(1.0f, t));
	return LerpUnclamped(a, b, t);
}

inline Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t) {
	return a*(1.0f - t) + b*t;
}

#endif // !_VECTOR3_H