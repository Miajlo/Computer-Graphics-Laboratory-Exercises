#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include<cmath>
#include<corecrt_math_defines.h>
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void) {
    yellow_rot_angle = 0;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

    glClearColor(0.502, 0.753, 1.0f, 1.0f); // Blue background
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL);  // Set depth function
    glShadeModel(GL_SMOOTH); // Enable smooth shading

    glEnable(GL_NORMALIZE);

    SetupLigting();

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //---------------------------------

    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    Camera& camera = Camera::getInstance();


    float lookX = 0.0f, lookY = 7.0f, lookZ = 0.0f;

    // Up vector remains constant in this case (Y-axis)
    float upX = 0.0f, upY = 1.0f, upZ = 0.0f;
    auto camPos = camera.getPosition();
    auto forward = camera.getForward();
    auto up = camera.getUp();
    // Apply gluLookAt using the camera's position and the fixed point (origin)
    gluLookAt(
        camPos.x, camPos.y, camPos.z,                     // Eye position
        camPos.x + forward.x, camPos.y + forward.y, camPos.z + forward.z,  // Target (eye + forward)
        up.x, up.y, up.z                                  // Up vector
    );

    SetupLigting();

    double axis_length = 20, grid_width = 10.0f, grid_height = 10.0f;
    int grid_nSegW = 10, grid_nSegH = 10;
    Color grid_clr(1.0f, 1.0f, 1.0f);

    DrawWalls();

    DrawAxis(axis_length);

    DrawFigure();

    // Swap the front and back buffers to display the rendered image
    SwapBuffers(pDC->m_hDC);
    //---------------------------------

    // Release the OpenGL context
    wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    if (h == 0) h = 1; // Prevent divide by zero

    glViewport(0, 0, w, h); // Set the viewport

    glMatrixMode(GL_PROJECTION); // Switch to projection matrix
    glLoadIdentity(); // Reset projection matrix
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 100.0); // Perspective projection

    glMatrixMode(GL_MODELVIEW); // Switch back to model-view matrix
    glLoadIdentity(); // Reset model-view matrix
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawHalfSphere(double r, int nSegAlpha, int nSegBeta) {

    for (int i = nSegAlpha / 2; i < nSegAlpha; i++)
    {
        // Calculate latitude angles
        double alpha1 = M_PI * (-0.5 + (double)(i) / nSegAlpha);
        double alpha2 = M_PI * (-0.5 + (double)(i + 1) / nSegAlpha);

        double sinAlpha1 = sin(alpha1), cosAlpha1 = cos(alpha1);
        double sinAlpha2 = sin(alpha2), cosAlpha2 = cos(alpha2);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= nSegBeta; j++)
        {
            // Calculate longitude angle
            double beta = 2.0 * M_PI * (double)(j) / nSegBeta;
            double sinBeta = sin(beta), cosBeta = cos(beta);

            // Vertices
            double x1 = r * cosAlpha1 * cosBeta;
            double y1 = r * sinAlpha1;
            double z1 = r * cosAlpha1 * sinBeta;

            double x2 = r * cosAlpha2 * cosBeta;
            double y2 = r * sinAlpha2;
            double z2 = r * cosAlpha2 * sinBeta;

            glVertex3d(x1, y1, z1);
            glVertex3d(x2, y2, z2);
        }
        glEnd();
    }
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg) {
    double angleStep = 2.0 * M_PI / nSeg;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= nSeg; i++)
    {
        double angle = i * angleStep;
        double x = cos(angle);
        double z = sin(angle);

        // Compute normal for the side surface (approximate as vector pointing outwards)
        GLfloat nx = (float)x;
        GLfloat ny = 0.0f;
        GLfloat nz = (float)z;

        glNormal3f(nx, ny, nz);
        glVertex3d(r1 * x, 0.0, r1 * z);

        glNormal3f(nx, ny, nz);
        glVertex3d(r2 * x, h, r2 * z);
    }
    glEnd();

    // Bottom base normals pointing downwards (0, -1, 0)
    if (r1 > 0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3d(0.0, 0.0, 0.0); // center

        for (int i = 0; i <= nSeg; i++)
        {
            double angle = i * angleStep;
            double x = cos(angle);
            double z = sin(angle);
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3d(r1 * x, 0.0, r1 * z);
        }
        glEnd();
    }

    // Top base normals pointing upwards (0, 1, 0)
    if (r2 > 0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3d(0.0, h, 0.0); // center

        for (int i = 0; i <= nSeg; i++)
        {
            double angle = i * angleStep;
            double x = cos(angle);
            double z = sin(angle);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3d(r2 * x, h, r2 * z);
        }
        glEnd();
    }
}



void CGLRenderer::DrawCone(double h, double r, int nSeg) {
    double angleStep = 2.0 * M_PI / nSeg;

    // Draw the sides of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0.0, h, 0.0); // Apex of the cone
    for (int i = 0; i <= nSeg; i++) {
        double angle = i * angleStep;
        double x = r * cos(angle);
        double z = r * sin(angle);
        glVertex3d(x, 0.0, z); // Base circle vertices
    }
    glEnd();

    // Draw the base of the cone
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(0.0, 0.0, 0.0); // Center of the base
    for (int i = 0; i <= nSeg; i++) {
        double angle = i * angleStep;
        double x = r * cos(angle);
        double z = r * sin(angle);
        glVertex3d(x, 0.0, z); // Base circle vertices
    }
    glEnd();
}

void CGLRenderer::DrawAxis(double width) {
    glLineWidth(2.0); // Set line thickness

    // Draw the X-axis in red
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0); // Origin
    glVertex3d(width, 0.0, 0.0); // End of the X-axis
    glEnd();

    // Draw the Y-axis in green
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0); // Origin
    glVertex3d(0.0, width, 0.0); // End of the Y-axis
    glEnd();

    // Draw the Z-axis in blue
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0); // Origin
    glVertex3d(0.0, 0.0, width); // End of the Z-axis
    glEnd();

    glLineWidth(1.0); // Reset line thickness to default
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH) {
    double stepW = width / nSegW; // Step size for width
    double stepH = height / nSegH; // Step size for height

    glBegin(GL_LINES);
    // Draw vertical lines (parallel to Z-axis)
    for (int i = 0; i <= nSegW; i++) {
        double x = -width / 2.0 + i * stepW;
        glVertex3d(x, 0.0, -height / 2.0); // Start point
        glVertex3d(x, 0.0, height / 2.0);  // End point
    }

    // Draw horizontal lines (parallel to X-axis)
    for (int j = 0; j <= nSegH; j++) {
        double z = -height / 2.0 + j * stepH;
        glVertex3d(-width / 2.0, 0.0, z); // Start point
        glVertex3d(width / 2.0, 0.0, z);  // End point
    }
    glEnd();
}

void CGLRenderer::DrawFigure(double angle) {
    float r = 0.5, cyl_h = 2.2;
    //segmet count
    int sph_seg = 30, cyl_seg = 8, cone_seg = 6;


    DrawPot();

    light_green.apply();

    DrawCylinder(2, 0.65, 0.65, cyl_seg);

    glTranslatef(0, 2 + r, 0);

    dark_green.apply();

    DrawHalfSphere(r, 30, sph_seg);

    DrawCactusArm(45.0f, light_green);
    
    glPopMatrix();

    DrawExtCactusArm(-45.0f, light_green);

    glPopMatrix();

    glPushMatrix();

    // FIRST UP BRANCH

    glTranslatef(0, r, 0);

    DrawCylinder(cyl_h, r, r, cyl_seg);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawHalfSphere(r, 30, sph_seg);

    light_green.apply();

    DrawCactusArm(-45.0f, light_green);

    glPopMatrix();
    // YELLOW BRANCH

    DrawCactusArm(45.0f + angle, yellow);

    glPopMatrix();

    glPopMatrix();

}

void CGLRenderer::DrawCactusArm(double angle, Color first_clr) {
    float r = 0.5, cyl_h = 2.2;
    //segmet count
    int sph_seg = 30, pot_seg = 6, cyl_seg = 8, cone_seg = 6;


    glPushMatrix();

    // FIRST LEFT BRANCH

    glRotatef(angle, 1.0f, 0.0f, 0.0f);

    glTranslatef(0, r, 0);


    first_clr.apply();

    // Draw the rotated cylinder
    DrawCylinder(cyl_h, r, r, cyl_seg);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawHalfSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, cone_seg);

    dark_green.apply();

    glTranslatef(0, cyl_h + r, 0);

    DrawHalfSphere(r, 30, sph_seg);

    light_green.apply();

    
}

void CGLRenderer::DrawExtCactusArm(double angle, Color first_clr) {
    float r = 0.5, cyl_h = 2.2;
    //segmet count
    int sph_seg = 30, pot_seg = 6, cyl_seg = 8, cone_seg = 6;

    DrawCactusArm(angle, first_clr);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, cone_seg);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawHalfSphere(r, 30, sph_seg);

    light_green.apply();

}

void CGLRenderer::DrawPot(){
    float pot_bb_r = 1.2f, pot_bot_h = 1.9f, pot_top_h = 1.0f;
    float pot_bt_r = 1.6f, pot_top_r = 2.0f;
    //segmet count
    int pot_seg = 6;


    pot_clr.apply();

    DrawCylinder(pot_bot_h, pot_bb_r, pot_bt_r, pot_seg);

    glTranslatef(0, pot_bot_h, 0);

    DrawCylinder(pot_top_h, pot_top_r, pot_top_r, pot_seg);

    glTranslatef(0, pot_top_h, 0);
}

void CGLRenderer::DrawFigure()
{
    float r = 2.0f, segAlpha = 30, segBeta = 30, h = 2, cR = 1, sR = 3;
    float cylR = 1.5, cylH = 0.4f, cylR2 = cylR / 2, cylR34 = 3 * cylR / 4;
    Color gray(0.46f, 0.46f, 0.46f);
    Color purple(1.0, 0, 1.0), teal(0, 1.0, 1.0);

    gray.apply();

    DrawHalfSphere(r, segAlpha, segBeta);

    glPushMatrix();

    glTranslatef(0, 0.8 * r, 0);

    DrawCylinder(h, cR, cR, 8);

    glTranslatef(0, h, 0);

    glPushMatrix();

    glRotatef(45, 0, 1.0, 0);

    DrawCylinder(h / 2, sR, sR, 4);

    glPopMatrix();

    glTranslatef(0, h / 2, 0);

    teal.apply();

    DrawCylinder(cylH, cylR, 3*cylR/4, segAlpha);

    DrawVaseCylinder(cylH, cylR34, cylR2, purple);

    DrawVaseCylinder(cylH, cylR2, cylR2, teal);

    DrawVaseCylinder(cylH, cylR2, cylR2, purple);

    DrawVaseCylinder(cylH, cylR2, cylR34, teal);

    DrawVaseCylinder(cylH, cylR34, cylR2, purple);

    DrawVaseCylinder(cylH, cylR2, cylR34, teal);

    DrawVaseCylinder(cylH, cylR34, cylR, purple);

    DrawVaseCylinder(cylH, cylR, cylR34, teal);

    DrawVaseCylinder(cylH, cylR34, cylR2, purple);

    DrawVaseCylinder(cylH, cylR2, cylR34, teal);

    DrawVaseCylinder(cylH, cylR34, cylR2, purple);

    DrawVaseCylinder(cylH, cylR2, cylR34, teal);

    DrawVaseCylinder(cylH, cylR34, cylR, purple);
    
    glPopMatrix();
}



void CGLRenderer::DrawWalls()
{
    Color gray(0.836f, 0.836f, 0.836f);
    float sqrSize = 30;
    gray.apply();

    glBegin(GL_QUADS);

    glVertex3f(-sqrSize, 0, -sqrSize);
    glVertex3f(sqrSize, 0, -sqrSize);  
    glVertex3f(sqrSize, 0, sqrSize);   
    glVertex3f(-sqrSize, 0,sqrSize);  

    glVertex3f(sqrSize, 0, sqrSize);
    glVertex3f(sqrSize, sqrSize, sqrSize);
    glVertex3f(sqrSize, sqrSize, -sqrSize);
    glVertex3f(sqrSize, 0, -sqrSize);
    
    glVertex3f(sqrSize, 0, sqrSize);
    glVertex3f(sqrSize, sqrSize, sqrSize);
    glVertex3f(-sqrSize, sqrSize, sqrSize);
    glVertex3f(-sqrSize, 0, sqrSize);

    glVertex3f(-sqrSize, 0, -sqrSize);
    glVertex3f(-sqrSize, sqrSize, -sqrSize);
    glVertex3f(sqrSize, sqrSize, -sqrSize);
    glVertex3f(sqrSize, 0, -sqrSize);

    glVertex3f(-sqrSize, 0, -sqrSize);
    glVertex3f(-sqrSize, sqrSize, -sqrSize);
    glVertex3f(-sqrSize, sqrSize, sqrSize);
    glVertex3f(-sqrSize, 0, sqrSize);


    glEnd();
}

void CGLRenderer::DrawVaseCylinder(float h, float r1, float r2, Color color)
{
    int segAlpha = 30;
    glTranslatef(0, h, 0);

    color.apply();

    DrawCylinder(h, r1, r2, segAlpha);

}

void CGLRenderer::SetupLigting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ambient[] = { 0.2f, 0.0f, 0.0f, 1.0f };
    GLfloat specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);


    GLfloat position[] = { 10.0f, 10.0f, 10.0f, 1.0f };  // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}
