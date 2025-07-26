#pragma once
#include<Windows.h>
#include <gl/GL.h>
#include "GL/GL.H"

struct Color {
	float red, green, blue, alpha;

	Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) { }

	void apply() const {
		glColor4f(red, green, blue, alpha);
	}
};
