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

    glClearColor(0.502, 0.753, 1.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL);  // Set depth function
    glShadeModel(GL_SMOOTH); // Enable smooth shading
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

    // Apply gluLookAt using the camera's position and the fixed point (origin)
    gluLookAt(
        camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, // Camera position
        lookX, lookY, lookZ,                                                   // Look-at point (the origin or fixed point)
        upX, upY, upZ                                                          // Up vector
    );

    double axis_length = 20, grid_width = 10.0f, grid_height = 10.0f;
    int grid_nSegW = 10, grid_nSegH = 10;
    Color grid_clr(1.0f, 1.0f, 1.0f);

    DrawAxis(axis_length);

    grid_clr.apply();

    DrawGrid(grid_width, grid_height, grid_nSegW, grid_nSegH);

    DrawFigure(yellow_rot_angle);

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

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta) {

    for (int i = 0; i < nSegAlpha; i++)
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

    // Draw the sides of the cylinder
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= nSeg; i++)
    {
        double angle = i * angleStep;
        double x = cos(angle);
        double z = sin(angle);

        // Bottom circle vertex
        glVertex3d(r1 * x, 0.0, r1 * z);

        // Top circle vertex
        glVertex3d(r2 * x, h, r2 * z);
    }
    glEnd();

    // Draw the bottom base
    if (r1 > 0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(0.0, 0.0, 0.0); // Center of the bottom base
        for (int i = 0; i <= nSeg; i++)
        {
            double angle = i * angleStep;
            double x = cos(angle);
            double z = sin(angle);
            glVertex3d(r1 * x, 0.0, r1 * z);
        }
        glEnd();
    }

    // Draw the top base
    if (r2 > 0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3d(0.0, h, 0.0); // Center of the top base
        for (int i = 0; i <= nSeg; i++)
        {
            double angle = i * angleStep;
            double x = cos(angle);
            double z = sin(angle);
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

    //glColor3f(0.7f, 0.7f, 0.7f); // Light gray color for the grid lines

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

    Color pot_clr(0.8667f, 0.4941f, 0.1176f), dark_green(0.0, 0.67f, 0.0f), light_green(0,1,0);
    Color yellow(1.0f, 1.0f, 0.0f);
    float r = 0.5, cyl_h = 2.2, bcyl_h, pot_bot_h = 1.9f, pot_top_h=1.0f;
    int sph_seg = 30;


    pot_clr.apply();

    DrawCylinder(pot_bot_h, 1.2, 1.6, 6);

    glTranslatef(0, pot_bot_h, 0);

    DrawCylinder(pot_top_h, 2, 2, 6);

    glTranslatef(0, pot_top_h, 0);

    light_green.apply();

    DrawCylinder(2, 0.65, 0.65, 8);

    glTranslatef(0, 2 + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    glPushMatrix();

    // FIRST LEFT BRANCH

    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);

    glTranslatef(0, r, 0);


    light_green.apply();
    
    // Draw the rotated cylinder
    DrawCylinder(cyl_h, r, r, 8);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, 6);

    dark_green.apply();

    glTranslatef(0, cyl_h + r, 0);

    DrawSphere(r, 30, sph_seg);

    light_green.apply();
    
    glPopMatrix();
    glPushMatrix();

    // FIRST RIGHT BRANCH
    glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0, r, 0);

    DrawCylinder(cyl_h, r, r, 8);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, 6);

    dark_green.apply();

    glTranslatef(0, cyl_h + r, 0);

    DrawSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, 6);
        
    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();
    
    DrawSphere(r, 30, sph_seg);

    light_green.apply();

    glPopMatrix();
    glPushMatrix();

    // FIRST UP BRANCH

    glTranslatef(0, r, 0);

    DrawCylinder(cyl_h, r, r, 8);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    light_green.apply();

    glPushMatrix();

    // SECOND RIGHT BRANCH

    glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);

    glTranslatef(0, r, 0);

    DrawCylinder(cyl_h, r, r, 8);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, 6);

    dark_green.apply();

    glTranslatef(0, cyl_h + r, 0);

    DrawSphere(r, 30, sph_seg);

    light_green.apply();

    glPopMatrix();

    glPushMatrix();

    yellow.apply();

    // YELLOW BRANCH

    glRotatef(45.0f + angle, 1.0f, 0.0f, 0.0f);

    glTranslatef(0, r, 0);

    DrawCylinder(cyl_h, r, r, 8);

    glTranslatef(0, cyl_h + r, 0);

    dark_green.apply();

    DrawSphere(r, 30, sph_seg);

    glTranslatef(0, r, 0);

    light_green.apply();

    DrawCone(cyl_h, r, 6);

    dark_green.apply();

    glTranslatef(0, cyl_h + r, 0);

    DrawSphere(r, 30, sph_seg);

    light_green.apply();

    glPopMatrix();

    glPopMatrix();

}
