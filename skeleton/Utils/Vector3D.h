#pragma once
#include <PxPhysicsAPI.h>
#include <cmath>

class Vector3D
{
public:
	Vector3D() : x(), y(), z(){}
	Vector3D(float _x, float _y, float _z): x(_x), y(_y), z(_z){}
	Vector3D(const physx::PxVec3& v) : x(v.x), y(v.y), z(v.z) {}

	float magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	Vector3D normalize() const {
		return *this / magnitude();
	}

	float dot(const Vector3D& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3D cross(const Vector3D& v) const {
		return Vector3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}

	Vector3D& operator=(const Vector3D& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3D operator+=(const Vector3D& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;;
	}

	Vector3D operator+(const Vector3D& other) const {
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	Vector3D operator-(const Vector3D& other) const {
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	Vector3D operator*(const Vector3D& other) const {
		return Vector3D(x * other.x, y * other.y, z * other.z);
	}

	Vector3D operator*(const float& other) const {
		return Vector3D(x * other, y * other, z * other);
	}

	Vector3D operator*=(const float& other){
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vector3D operator/(const float& other) const {
		return Vector3D(x / other, y / other, z / other);
	}

	operator physx::PxVec3() const {
		return physx::PxVec3(x, y, z);
	}

	float x, y, z;
};

