#pragma once
#include<cmath>

struct Vector3 {
    float x, y, z;

    // Constructor
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Normalize the vector
    Vector3 normalize() const {
        float magnitude = std::sqrt(x * x + y * y + z * z);
        if (magnitude == 0) return Vector3(0, 0, 0);
        return Vector3(x / magnitude, y / magnitude, z / magnitude);
    }
};