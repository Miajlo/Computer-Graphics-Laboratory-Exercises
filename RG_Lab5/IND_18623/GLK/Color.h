#pragma once
#include <gl/GL.h>
#include "GL/GL.H"


struct Color {
	float r, g, b; // RGB components

	// Constructor to initialize the color
	Color(float red=0, float green=0, float blue=0) : r(red), g(green), b(blue) {}

	// Optional: a method to apply the color using OpenGL's glColor3f
	void apply() const {
		GLfloat mat_ambient[] = { r , g, b , 1.0f };  // 50% ambient
		GLfloat mat_diffuse[] = { r, g, b, 1.0f };
		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat mat_shininess[] = { 50.0f };

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	}
};