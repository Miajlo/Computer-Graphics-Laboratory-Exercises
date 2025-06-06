#pragma once
#include"Camera.h"
#include"Color.h"


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

	void DrawHalfSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);

	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);

	void DrawFigure(double angle);

	void DrawCactusArm(double angle, Color first_clr);
	void DrawExtCactusArm(double angle, Color first_clr);
	void DrawPot();
	void DrawFigure();
	void DrawWalls();
	void DrawVaseCylinder(float h, float r1, float r2, Color color);
	float yellow_rot_angle;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	const Color light_green{ 0.0f, 1.0f, 0.0f };
	const Color dark_green{ 0.0f, 0.67f, 0.0f };
	const Color yellow{ 1.0f,1.0f,0.0f };
	const Color pot_clr{ 0.8667f, 0.4941f, 0.1176f };
};
