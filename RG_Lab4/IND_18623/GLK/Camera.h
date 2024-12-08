#pragma once
#include "Vector3.h"
#include <cmath>
#include<corecrt_math_defines.h>

class Camera {
public:
    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }

    void initialize(const Vector3& initPosition, float initYaw = 0.0f, float initPitch = 0.0f) {
        position = initPosition;
        yaw = initYaw;
        pitch = initPitch;
    }

    void rotate(float deltaYaw, float deltaPitch) {
        yaw += deltaYaw;
        pitch += deltaPitch;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updatePosition();
    }

    void updatePosition() {
        float yawRadians = yaw * M_PI / 180.0f;
        float pitchRadians = pitch * M_PI / 180.0f;

        position.x = r * cos(pitchRadians) * sin(yawRadians);
        position.y = r * sin(pitchRadians) + 7.0f;
        position.z = r * cos(pitchRadians) * cos(yawRadians);
    }

    Vector3 getPosition() const {
        return position;
    }

    float getYaw() const {
        return yaw;
    }

    float getPitch() const {
        return pitch;
    }

private:
    Camera(float yaww=90.0f, float pitchh=0.0f, float rad = 17.0f)  {
        yaw = yaww;
        pitch = pitchh;
        r = rad;
        position = { r, 7.0f, 0.0f };
    }
    Camera(Camera const&) = delete;
    void operator=(Camera const&) = delete;

    Vector3 position;
    float yaw;
    float pitch;
    float r;
};
