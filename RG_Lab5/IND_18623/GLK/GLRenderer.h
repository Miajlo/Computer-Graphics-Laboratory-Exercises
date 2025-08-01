#pragma once
#include "Camera.h"
#include<corecrt_math_defines.h>

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,


	void DrawRectangle(float size1, float side2);
	void UpdateCamera();
	void DrawCircle(float r, int segments);
	void DrawNormal(float x, float y, float z, float nx, float ny, float nz, float scale);
	Camera camera;
	bool DrawNormals;
	float NormalSize;
	const float DEG2RAD = M_PI / 180.0f;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 



};
