#pragma once
namespace Engine {

struct Vector3 {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3();
	Vector3(float _x, float _y, float _z);

	bool operator==(const Vector3& other) const {
		return (x == other.x) && (y == other.y) && (z == other.z);
	}
	bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}
};

}
