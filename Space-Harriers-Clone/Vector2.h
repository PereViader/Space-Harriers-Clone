#ifndef _VECTOR2_H
#define _VECTOR2_H

#include <cmath>
#include <algorithm>

class Vector2;
class Vector3;

using namespace std;

Vector2& operator+=(Vector2& lhs, const Vector2& rhs);
Vector2& operator-=(Vector2& lhs, const Vector2& rhs);
Vector2& operator/=(Vector2& lhs, float value);
Vector2& operator*=(Vector2& lhs, float value);

bool operator==(const Vector2& lhs, const Vector2& rhs);
bool operator!=(const Vector2& lhs, const Vector2& rhs);

Vector2 operator*(float d, const Vector2& v);
Vector2 operator*(const Vector2& v, float d);

Vector2 operator/(const Vector2& v, float d);
Vector2 operator/(float d, const Vector2& v);

float Dot(const Vector2& a, const Vector2& b);

Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t);

Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta);


class Vector2 {
public:
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const Vector3&);

	float x;
	float y;

	Vector2 operator+(const Vector2& other) const {
		Vector2 ret(*this);
		ret.x += other.x;
		ret.y += other.y;
		return ret;
	}

	Vector2 operator-(const Vector2& other) const {
		Vector2 ret(*this);
		ret.x -= other.x;
		ret.y -= other.y;
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

	Vector2 Normalized() const {
		return operator/(*this, Magnitude());
	}

	static const Vector2 UP;
	static const Vector2 RIGHT;
	static const Vector2 ZERO;
};

inline Vector2& operator+=(Vector2& lhs, const Vector2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Vector2& operator-=(Vector2& lhs, const Vector2& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

inline Vector2& operator*=(Vector2& lhs, float value) {
	lhs.x *= value;
	lhs.y *= value;
	return lhs;
}

inline Vector2& operator/=(Vector2& lhs, float value) {
	lhs.x /= value;
	lhs.y /= value;
	return lhs;
}

inline bool operator==(const Vector2& lhs, const Vector2& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Vector2& lhs, const Vector2& rhs) {
	return lhs.x != rhs.x || lhs.y != rhs.y;
}

inline Vector2 operator*(float d, const Vector2& v) {
	Vector2 ret(v);
	ret.x *= d;
	ret.y *= d;
	return ret;
}

inline Vector2 operator*(const Vector2& v, float d) {
	return operator*(d, v);
}

inline Vector2 operator/(float d, const Vector2& v) {
	Vector2 ret(v);
	ret.x /= d;
	ret.y /= d;
	return ret;
}

inline Vector2 operator/(const Vector2& v, float d) {
	return operator/(d, v);
}


inline float Dot(const Vector2& a, const Vector2& b) {
	return a.x*b.x + a.y*b.y;
}

inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
	t = max(0.0f, min(1.0f, t));
	return LerpUnclamped(a, b, t);
}

inline Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t) {
	return a*(1.0f - t) + b*t;
}

#endif // !_VECTOR2_H