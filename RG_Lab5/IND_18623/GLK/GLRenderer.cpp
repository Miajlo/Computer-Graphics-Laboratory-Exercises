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
    DrawNormals = true;
    NormalSize = 3.0f;
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


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //---------------------------------

    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    UpdateCamera();
    glColor3f(1, 0, 0);

    glPushMatrix();
    glTranslatef(0, 0, -10);
    DrawRectangle(10, 10);
    glPopMatrix();

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

void CGLRenderer::DrawRectangle(float size1, float side2) {

    float mid1 = size1 / 2, mid2 = side2 / 2;

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);

    glVertex3f(-mid1, - mid2, 0);
    glVertex3f(mid1, -mid2, 0);
    glVertex3f(mid1, mid2, 0);
    glVertex3f(-mid1, mid2, 0);

    glEnd();

    if (DrawNormals) {
        glColor3f(1, 1, 0);


        float nx = 0, ny = 0, nz = 1;
        float scale = NormalSize;

        glBegin(GL_LINES);
        DrawNormal(-mid1, -mid2, 0, nx, ny, nz, scale);
        DrawNormal(mid1, -mid2, 0, nx, ny, nz, scale);
        DrawNormal(mid1, mid2, 0, nx, ny, nz, scale);
        DrawNormal(-mid1, mid2, 0, nx, ny, nz, scale);
        glEnd();
    }

}

void CGLRenderer::UpdateCamera() {
    float radianPitch = camera.pitch * DEG2RAD;
    float radianYaw = camera.yaw * DEG2RAD;

    // Calculate forward vector
    float dirX = cosf(radianPitch) * sinf(radianYaw);
    float dirY = sinf(radianPitch);
    float dirZ = -cosf(radianPitch) * cosf(radianYaw);

    float targetX = camera.posX + dirX;
    float targetY = camera.posY + dirY;
    float targetZ = camera.posZ + dirZ;

    // Up is fixed (Y-up)
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera.posX, camera.posY, camera.posZ,
        targetX, targetY, targetZ,
        upX, upY, upZ
    );


}

void CGLRenderer::DrawCircle(float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
}

void CGLRenderer::DrawNormal(float x, float y, float z, float nx, float ny, float nz, float scale)
{

    glVertex3f(x, y, z);
    glVertex3f(x + nx * scale, y + ny * scale, z + nz * scale);

}




