#pragma once
#include <gl/GL.h>
#include "GL/GL.H"


struct Color {
	float r, g, b; // RGB components

	// Constructor to initialize the color
	Color(float red=0, float green=0, float blue=0) : r(red), g(green), b(blue) {}

	// Optional: a method to apply the color using OpenGL's glColor3f
	void apply() const {
		glColor3f(r, g, b); // Use OpenGL's color function
	}
};