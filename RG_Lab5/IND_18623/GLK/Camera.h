#pragma once
#include "vec3.h"
#include <cmath>
#include <corecrt_math_defines.h>

class Camera {
public:
    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }

    void initialize(const vec3& initPosition, float initYaw = 0.0f, float initPitch = 0.0f) {
        position = initPosition;
        yaw = initYaw;
        pitch = initPitch;
        updateDirectionVectors(); // Calculate forward, right, up
    }

    void rotate(float deltaYaw, float deltaPitch) {
        yaw += deltaYaw;
        pitch += deltaPitch;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateDirectionVectors();
    }

    void move(const vec3& direction, float amount) {
        position += direction * amount;
    }

    void moveForward(float amount) {
        position += forward * amount;
    }

    void moveRight(float amount) {
        position += right * amount;
    }

    void moveUp(float amount) {
        position += up * amount;
    }

    vec3 getForward() const { return forward; }
    vec3 getRight()   const { return right; }
    vec3 getUp()      const { return up; }
    vec3 getPosition() const { return position; }

    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }

private:
    Camera(float yaww = 90.0f, float pitchh = 0.0f) {
        yaw = yaww;
        pitch = pitchh;
        position = vec3(0.0f, 0.0f, 0.0f);
        updateDirectionVectors();
    }

    void updateDirectionVectors() {
        float yawRad = yaw * M_PI / 180.0f;
        float pitchRad = pitch * M_PI / 180.0f;

        forward.x = cos(pitchRad) * sin(yawRad);
        forward.y = sin(pitchRad);
        forward.z = cos(pitchRad) * cos(yawRad);
        forward = forward.normalize();

        // World up
        vec3 worldUp(0.0f, 1.0f, 0.0f);

        // Correct cross product order
        right = forward.cross(worldUp).normalize();   // Right = forward × up
        up = right.cross(forward).normalize();        // Up = right × forward
    }


    vec3 position;
    vec3 forward;
    vec3 right;
    vec3 up;
    float yaw;
    float pitch;
};
