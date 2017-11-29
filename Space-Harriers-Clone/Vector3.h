
class Vector3 {
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(float x, float y) : x(x), y(y), z(0) {}

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

};

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