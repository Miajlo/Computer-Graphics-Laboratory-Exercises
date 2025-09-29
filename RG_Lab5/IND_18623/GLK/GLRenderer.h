#pragma once
#include "Camera.h"
#include<corecrt_math_defines.h>
#include <windows.h>      // required before gl.h on Windows
#include <GL/gl.h> 


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


	void DrawBox(float squareSide);
	void DrawVase(float squareSide);

	void DrawRectangle(float size1, float side2);
	void UpdateCamera();
	void DrawCircle(float r, int segments);
	void DrawNormal(float x, float y, float z, float nx, float ny, float nz, float scale);
	void DrawSphere(float radius, int slices, int stacks, bool hemisphere=false);
	void DrawLightCircle(float radius);
	void DrawParallelopiped(float a, float b, float c);
	void DrawHollowCylinder(float radius, float height, int segments);
	void SetMaterial(float r, float g, float b, float shininess=0);
	Camera camera;
	bool DrawNormals;
	float NormalSize;
	const float DEG2RAD = M_PI / 180.0f;
	const float SPOT_CUTOFF = 45.0f;
	bool showRedLight;
	bool showGreenLight;
	bool showBlueLight;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 



};
