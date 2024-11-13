#pragma once
#include<wingdi.h>
#include <corecrt_math_defines.h>
#include<cmath>

namespace transforms {

	XFORM get_translation_matrix(int &x, int &y) {
        XFORM xForm;
        xForm.eM11 = 1.0f; // Scale X
        xForm.eM12 = 0.0f; // Shear X
        xForm.eM21 = 0.0f; // Shear Y
        xForm.eM22 = 1.0f; // Scale Y
        xForm.eDx = x; // Translate X
        xForm.eDy = y; // Translate Y

        return xForm;
	}

    XFORM get_rotational_matrix(float &angle) {
        XFORM xForm;

        auto rad_angle = angle * M_PI / 180;

        auto cos_value = std::cos(rad_angle);
        auto sin_value = std::sin(rad_angle);

        xForm.eM11 = cos_value;
        xForm.eM12 = sin_value;
        xForm.eM21 = -sin_value;
        xForm.eM22 = cos_value;
        xForm.eDx = 0.0f;
        xForm.eDy = 0.0f;

        return xForm;
    }

    XFORM get_scaling_matrix(int &sx, int &sy) {
        XFORM xForm;

        xForm.eM11 = sx;
        xForm.eM12 = 0;
        xForm.eM21 = 0;
        xForm.eM22 = sy;
        xForm.eDx = 0.0f;
        xForm.eDy = 0.0f;

        return xForm;
    }
}