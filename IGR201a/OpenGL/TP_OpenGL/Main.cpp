// ----------------------------------------------
// Informatique Graphique 3D & R�alit� Virtuelle.
// Travaux Pratiques
// Introduction � OpenGL
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GL/glut.h>

#include <math.h>

#define X .525731112119133606 
#define Z .850650808352039932

using namespace std;

// These functions are used to draw a sphere
static GLfloat vdata[12][3] = {    
        {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
        {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
        {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};

static GLuint tindices[20][3] = { 
        {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
        {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
        {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
        {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
};

void normalize(GLfloat *a) {
        GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
        a[0]/=d; a[1]/=d; a[2]/=d;
}

void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r) {
        if (div<=0) 
        {
            glNormal3fv(a); glColor3f(fabs(a[0]*r), fabs(a[1]*r), fabs(a[2]*r)); glVertex3f(a[0]*r, a[1]*r, a[2]*r);
            glNormal3fv(b); glColor3f(fabs(b[0]*r), fabs(b[1]*r), fabs(b[2]*r)); glVertex3f(b[0]*r, b[1]*r, b[2]*r);
            glNormal3fv(c); glColor3f(fabs(c[0]*r), fabs(c[1]*r), fabs(c[2]*r)); glVertex3f(c[0]*r, c[1]*r, c[2]*r);
        } else 
        {
             GLfloat ab[3], ac[3], bc[3];
             for (int i=0;i<3;i++) 
             {
                 ab[i]=(a[i]+b[i])/2;
                 ac[i]=(a[i]+c[i])/2;
                 bc[i]=(b[i]+c[i])/2;
             }
             normalize(ab); normalize(ac); normalize(bc);
             drawtri(a, ab, ac, div-1, r);
             drawtri(b, bc, ab, div-1, r);
             drawtri(c, ac, bc, div-1, r);
             drawtri(ab, bc, ac, div-1, r);  
         }  
}

void drawSphere(int ndiv, float radius=1.0) {
        glBegin(GL_TRIANGLES);
        for (int i=0;i<20;i++)
            drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]],ndiv, radius);
        glEnd();
}

void glSphere(float x, float y, float z)
{
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glTranslatef(x,y,z);
    drawSphere(3, 0.5);
    glPopMatrix();
}

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Introduction a OpenGL");
static GLint window;
static unsigned int screenWidth;
static unsigned int screenHeight;

// Camera parameters
static float fovAngle;
static float aspectRatio;
static float nearPlane;
static float farPlane;
static float camPhi; // Expressing the camera position in polar coordinate, in the frame of the target
static float camTheta;
static float camDist2Target;
static float camTargetX;
static float camTargetY;
static float camTargetZ;

void polar2Cartesian (float phi, float theta, float r, float & x, float & y, float & z) {
	x = r * sin (theta) * cos (phi);
	y = r * sin (theta) * sin (phi);
	z = r * cos (theta);
  }

void printUsage () {
	std::cerr << std::endl // send a line break to the standard error output
		 << appTitle << std::endl
         << "Author : Tamy Boubekeur" << std::endl << std::endl
         << "Usage : ./main [<file.off>]" << std::endl
         << "Cammandes clavier :" << std::endl
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl 
         << " <drag>+<right button>: move model" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " q, <esc>: Quit" << std::endl << std::endl; 
}

void init () {
	// OpenGL initialization
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color

	// Camera initialization
	fovAngle = 45.f;
	nearPlane = 0.01;
	farPlane = 10.0;
	camPhi = M_PI/2;
	camTheta = M_PI/2;
	camDist2Target = 8;
	camTargetX = 1.0;
	camTargetY = 1.0;
	camTargetZ = 1.0;
}

void setupCamera () {
	glMatrixMode (GL_PROJECTION); // Set the projection matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	gluPerspective (fovAngle, aspectRatio, nearPlane, farPlane); // Set the current projection matrix with the camera intrinsics
	glMatrixMode (GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	float camPosX, camPosY, camPosZ;
	polar2Cartesian (camPhi, camTheta, camDist2Target, camPosX, camPosY, camPosZ);
	camPosX += camTargetX;
	camPosY += camTargetY;
	camPosZ += camTargetZ;
	gluLookAt (camPosX, camPosY, camPosZ, camTargetX, camTargetY, camTargetZ, 0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
    screenWidth = w;
	screenHeight = h;
	aspectRatio = static_cast<float>(w)/static_cast<float>(h);
	glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
	setupCamera ();
}

void display () {
    setupCamera ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.

	// Put your drawing code (glBegin, glVertex, glCallList, glDrawArray, etc) here
        glSphere(0,0,0);
        glSphere(1,0,0);
        glSphere(2,0,0);
        glSphere(0.5,0.90,0);
        glSphere(1.5,0.90,0);
        glSphere(1,1.8,0);
        glSphere(0.5,0.3,0.9);
        glSphere(1.5,0.3,0.9);
        glSphere(1,1.18,0.9);
        glSphere(1,0.66,1.8);

    glFlush (); // Ensures any previous OpenGL call has been executed
    glutSwapBuffers ();  // swap the render buffer and the displayed (screen) one
}

void keyboard (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
	case 'w':
		GLint mode[2];
		glGetIntegerv (GL_POLYGON_MODE, mode);
		glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
        break;
    case 'q':
    case 27:
        exit (0);
        break;
     default:
        printUsage ();
        break;
    }
    glutPostRedisplay ();
}

void mouse (int button, int state, int x, int y) {
}

void motion (int x, int y) {
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {
}


// Main function
int main (int argc, char ** argv) {
    glutInit (&argc, argv); // Initialize a glut app
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
    window = glutCreateWindow (appTitle.c_str ()); // create the window
    init (); // Your initialization code (OpenGL states, geometry, material, lights, etc)
    glutReshapeFunc (reshape); // Callback function executed whenever glut need to setup the projection matrix
	glutDisplayFunc (display); // Callback function executed when the window app need to be redrawn
    glutKeyboardFunc (keyboard); // Callback function executed when the keyboard is used
    glutMouseFunc (mouse); // Callback function executed when a mouse button is clicked 
	glutMotionFunc (motion); // Callback function executed when the mouse move
	glutIdleFunc (idle); // Callback function executed continuously when no other event happens (good for background procesing or animation for instance).
    printUsage (); // By default, display the usage help of the program   
    glutMainLoop ();
    return 0;
}

