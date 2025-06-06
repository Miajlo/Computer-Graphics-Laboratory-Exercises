#pragma once
#include<cmath>

struct vec3 {
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}


    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }


    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    vec3 normalize() const {
        float magnitude = std::sqrt(x * x + y * y + z * z);
        if (magnitude == 0) return vec3(0, 0, 0);
        return vec3(x / magnitude, y / magnitude, z / magnitude);
    }

    vec3 cross(const vec3& other) const {
        return vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    static vec3 unitX() {
        return vec3(1.0f, 0, 0);
    }

    static vec3 unitY() {
        return vec3(0, 1.0f, 0);
    }

    static vec3 unitZ() {
        return vec3(0, 0, 1.0f);
    }
};