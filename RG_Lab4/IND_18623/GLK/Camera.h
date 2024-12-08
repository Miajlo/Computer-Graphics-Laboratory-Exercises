#pragma once

#include "Vector3.h"
#include <corecrt_math_defines.h>

class Cam {
public:
    static Cam* _instance;
    // Singleton instance
    static Cam* getInstance() {
        if (!_instance)
            _instance->initialize();
        return _instance;
    }
    Vector3 getPosition() const { return position; }
    float getTheta() const { return theta; }
    float getPhi() const { return phi; }
    // Prevent copying or assignment
    Cam(const Cam&) = delete;
    Cam& operator=(const Cam&) = delete;
    static void deleteInstance() {
        delete _instance;  // Safely delete the allocated memory
        _instance = nullptr; // Set pointer to null to avoid dangling pointer
    }
    // Initialize the camera position
    void initialize(float radius = 10.0f, float initTheta = 0.0f, float initPhi = M_PI / 2.0f) {
        this->radius = radius;
        this->theta = initTheta;
        this->phi = initPhi;
        updateCameraPosition();
    }

    // Rotate the camera
    void rotate(float deltaTheta, float deltaPhi) {
        theta += deltaTheta;
        phi += deltaPhi;

        // Clamp phi to avoid flipping at poles
        const float epsilon = 0.01f;
        phi = fmax(epsilon, fmin(M_PI - epsilon, phi));

        updateCameraPosition();
    }

private:
    Cam() : radius(10.0f), theta(0.0f), phi(M_PI / 2.0f), position(10.0f, 0.0f, 0.0f) {}

    // Update the camera position based on spherical coordinates
    void updateCameraPosition() {
        position.x = radius * sin(phi) * cos(theta);
        position.y = radius * cos(phi);
        position.z = radius * sin(phi) * sin(theta);
    }

    // Member variables
    float radius;      // Distance from the origin
    float theta;       // Horizontal angle (rotation around Y-axis)
    float phi;         // Vertical angle (rotation around X-axis)
    Vector3 position;  // Camera position in Cartesian coordinates
};

Cam* Cam::_instance = nullptr;