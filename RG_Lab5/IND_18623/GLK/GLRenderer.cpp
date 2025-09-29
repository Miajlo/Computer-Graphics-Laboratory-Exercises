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
    showLightSources = true;
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

    glClearColor(0.502, 0.753, 1.0f, 1.0f);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.502, 0.753, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // Enable three lights
    glEnable(GL_LIGHT0);  // Red
    glEnable(GL_LIGHT1);  // Green
    glEnable(GL_LIGHT2);  // Blue

    // Global ambient (very low)
    GLfloat global_ambient[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    // RED LIGHT (LIGHT0)
    GLfloat red_ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
    GLfloat red_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat red_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, red_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, red_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 15.0f);

    // GREEN LIGHT (LIGHT1)
    GLfloat green_ambient[] = { 0.0f, 0.1f, 0.0f, 1.0f };
    GLfloat green_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    GLfloat green_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, green_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, green_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, green_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0f);

    // BLUE LIGHT (LIGHT2)
    GLfloat blue_ambient[] = { 0.0f, 0.0f, 0.1f, 1.0f };
    GLfloat blue_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat blue_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT2, GL_AMBIENT, blue_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, blue_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, blue_specular);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);

    /*glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);*/

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //---------------------------------

    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    UpdateCamera();

    GLfloat red_pos[] = { -4.0f, 5.0f, 0.0f, 1.0f };
    GLfloat red_dir[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, red_pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, red_dir);

    // GREEN light - center
    GLfloat green_pos[] = { 4.0f, 5.0f, 0.0f, 1.0f };
    GLfloat green_dir[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, green_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, green_dir);

    // BLUE light - other side
    GLfloat blue_pos[] = { 0.0f, 5.0f, 0.0f, 1.0f };
    GLfloat blue_dir[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, blue_pos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, blue_dir);

    float lightSourceRadius = 0.1;

    glDisable(GL_LIGHTING);
        glTranslatef(0, -5, 0);
        glPushMatrix();
            glTranslatef(0, 10, 0);
            glColor3f(0, 0, 1);
            DrawSphere(lightSourceRadius, 16, 16);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(4, 5, 0);
            glColor3f(0, 1, 0);
            DrawSphere(lightSourceRadius, 16, 16);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-4, 5, 0);
            glColor3f(1, 0, 0);
            DrawSphere(lightSourceRadius, 16, 16);
        glPopMatrix();
    glEnable(GL_LIGHTING);

    float squareSide = 10.0f, halfSquareSize = squareSide/2.0f;

    
    SetMaterial(0.8f, 0.8f, 0.8f, 32.0f);

    glPushMatrix();
        glRotatef(90, -1, 0 , 0);
        DrawRectangle(squareSide, squareSide);
    glPopMatrix();

    SetMaterial(0.8f, 0.8f, 0.8f, 32.0f);

    glPushMatrix();
    glTranslatef(0, halfSquareSize, 0);
    glPushMatrix();
        glTranslatef(0, 0, halfSquareSize);
        DrawRectangle(squareSide, squareSide);
    glPopMatrix();

    SetMaterial(0.8f, 0.8f, 0.8f, 32.0f);

    glPushMatrix();
        glTranslatef(0, 0, -halfSquareSize);
        DrawRectangle(squareSide, squareSide);
    glPopMatrix();

    SetMaterial(0.8f, 0.8f, 0.8f, 32.0f);

    glPushMatrix();
        glRotatef(90, 0, 1, 0);
        glTranslatef(0, 0, halfSquareSize);
        DrawRectangle(squareSide, squareSide);
    glPopMatrix();

    SetMaterial(0.8f, 0.8f, 0.8f, 32.0f);

    glPushMatrix();
        glRotatef(90, 0, -1, 0);
        glTranslatef(0, 0, halfSquareSize);
        DrawRectangle(squareSide, squareSide);
    glPopMatrix();
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

void CGLRenderer::DrawRectangle(float width, float height) {
    
    float nx = 0.0f, ny = 0.0f, nz = 1.0f;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    glBegin(GL_QUADS);

    glNormal3f(nx, ny, nz);

    glVertex3f(-halfWidth, -halfHeight, 0.0f);

    glVertex3f(halfWidth, -halfHeight, 0.0f);

    glVertex3f(halfWidth, halfHeight, 0.0f);

    glVertex3f(-halfWidth, halfHeight, 0.0f);

    glEnd();
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

void CGLRenderer::DrawCircle(float r, int segments)
{
}

void CGLRenderer::DrawNormal(float x, float y, float z, float nx, float ny, float nz, float scale)
{
}

void CGLRenderer::DrawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; ++i) {
        float phi1 = M_PI * i / stacks;
        float phi2 = M_PI * (i + 1) / stacks;

        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= slices; ++j) {
            float tetha = 2.0 * M_PI * j / slices;

            float x1 = radius * sinf(phi1) * cosf(tetha);
            float y1 = radius * cosf(phi1);
            float z1 = radius * sinf(phi1) * sinf(tetha);

            float nx1 = x1 / radius;
            float ny1 = y1 / radius;
            float nz1 = z1 / radius;

            glNormal3f(nx1, ny1, nz1);
            glVertex3f(x1, y1, z1);

            float x2 = radius * sinf(phi2) * cosf(tetha);
            float y2 = radius * cosf(phi2);
            float z2 = radius * sinf(phi2) * sinf(tetha);

            float nx2 = x2 / radius;
            float ny2 = y2 / radius;
            float nz2 = z2 / radius;

            glNormal3f(nx2, ny2, nz2);
            glVertex3f(x2, y2, z2);
        }

        glEnd();
    }

}

void CGLRenderer::DrawLightCircle(float radius)
{
}

void CGLRenderer::SetMaterial(float r, float g, float b, float shininess) {
     GLfloat ambient[] = { r * 0.5f, g * 0.5f, b * 0.5f, 1.0f };  // Was 0.2, now 0.5
    GLfloat diffuse[] = { r, g, b, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}
