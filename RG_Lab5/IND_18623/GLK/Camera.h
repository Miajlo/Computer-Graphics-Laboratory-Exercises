#pragma once
#include<algorithm>
#include<corecrt_math_defines.h>
#include <cmath>

struct Camera {

	float posX, posY, posZ;
	float yaw, pitch;
	const float MAX_ABS_PITCH = 89;
	const float DEG2RAD = M_PI / 180.0f;

	Camera() : posX(0), posY(0), posZ(5), yaw(0), pitch(0) {}

	void SetPosition(float x, float y, float z) {
		posX = x;
		posY = y;
		posZ = z;
	}

	void SetRotation(float _yaw, float _pitch) {
		yaw = _yaw;
		pitch = _pitch;
		if (pitch > MAX_ABS_PITCH)
			pitch = MAX_ABS_PITCH;
		if (pitch < -MAX_ABS_PITCH)
			pitch = -MAX_ABS_PITCH;
	}

	void Rotate(float dYaw, float dPitch) {
		SetRotation(yaw + dYaw, pitch + dPitch);
	}

	void Move(float right, float up, float forward) {
		float radPitch = pitch * DEG2RAD;
		float radYaw = yaw * DEG2RAD;
		
		float dirX = cos(radPitch) * sin(radYaw);
		float dirY = sin(radPitch);
		float dirZ = -cos(radPitch) * cos(radYaw);

		// Right vector (cross product of forward and world up)
		float rightX = cos(radYaw);
		float rightZ = sin(radYaw);

		// Apply movement
		posX += forward * dirX + right * rightX;
		posY += up;
		posZ += forward * dirZ + right * rightZ;
	}


};
