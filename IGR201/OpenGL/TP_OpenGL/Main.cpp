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

std::vector<GLfloat> spherePositionArray;
std::vector<GLfloat> sphereNormalArray;
std::vector<GLfloat> sphereTexcoordArray;
std::vector<GLushort> sphereIndexArray;
GLuint texture; // Identifiant opengl de la texture
static float currentTime;
bool pause = false;

void glSphere(GLfloat x, GLfloat y, GLfloat z)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x,y,z);

  glDrawElements(GL_TRIANGLES, sphereIndexArray.size(),
          GL_UNSIGNED_SHORT, &sphereIndexArray[0]);

  glPopMatrix();
}

void glSphereWithMat (float x, float y, float z, float r, float difR, float difG,
        float difB, float specR, float specG, float specB, float shininess)
{
  GLfloat material_color[4] = {difR, difG, difB, 1.0f};
  GLfloat material_specular[4] = {specR, specG, specB,1.0};
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
  glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  glBindTexture(GL_TEXTURE_2D, texture);
  glSphere(x,y,z);
}

void genCheckerboard(unsigned int width, unsigned int height, unsigned char * image){
    unsigned int i, j;
    int ind = 0;
    for (i=0; i< width; i++) for (j=0;j<height;j++){
        if (((i+j)/10)%2){
          image[ind++] = 255;
          image[ind++] = 0;
          image[ind++] = 0;
        } else {
          image[ind++] = 0;
          image[ind++] = 0;
          image[ind++] = 255;
        }
    }
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
  glEnable(GL_LIGHTING);
	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color

  // Sphere array initialisation
  float radius = 1;
  unsigned int ntheta = 40;
  unsigned int nphi = 40;

  float const T = 1./(float)(ntheta);
  float const P = 1./(float)(nphi);
  unsigned int i, j;

  sphereNormalArray.resize(ntheta * nphi * 3 * 4);
  sphereTexcoordArray.resize(ntheta * nphi * 12);
  sphereIndexArray.resize(ntheta * nphi * 6);
  std::vector<GLfloat>::iterator n = sphereNormalArray.begin();
  std::vector<GLushort>::iterator d = sphereIndexArray.begin();
  std::vector<GLfloat>::iterator t = sphereTexcoordArray.begin();

  for(i = 0; i < ntheta; i++) for(j = 0; j < nphi; j++) {
          float const th0 = j*2*M_PI*T;
          float const th1 = (j+1)*2*M_PI*T;
          float const phi0 = i*2*M_PI*P;
          float const phi1 = (i+1)*2*M_PI*P;

          float const x1 = sin(phi0)*cos(th0);
          float const x2 = sin(phi0)*cos(th1);
          float const x3 = sin(phi1)*cos(th1);
          float const x4 = sin(phi1)*cos(th0);

          float const y1 = sin(phi0)*sin(th0);
          float const y2 = sin(phi0)*sin(th1);
          float const y3 = sin(phi1)*sin(th1);
          float const y4 = sin(phi1)*sin(th0);

          float const z1 = cos(phi0);
          float const z2 = cos(phi1);

          spherePositionArray.push_back(x1 * radius);
          spherePositionArray.push_back(y1 * radius);
          spherePositionArray.push_back(z1 * radius);
          int i1 = spherePositionArray.size()/3 -1;

          spherePositionArray.push_back(x2 * radius);
          spherePositionArray.push_back(y2 * radius);
          spherePositionArray.push_back(z1 * radius);
          int i2 = spherePositionArray.size()/3 -1;

          spherePositionArray.push_back(x3 * radius);
          spherePositionArray.push_back(y3 * radius);
          spherePositionArray.push_back(z2 * radius);
          int i3 = spherePositionArray.size()/3 -1;

          spherePositionArray.push_back(x4 * radius);
          spherePositionArray.push_back(y4 * radius);
          spherePositionArray.push_back(z2 * radius);
          int i4 = spherePositionArray.size()/3 -1;

          *n++ = x1;
          *n++ = y1;
          *n++ = z1;

          *n++ = x2;
          *n++ = y2;
          *n++ = z1;

          *n++ = x3;
          *n++ = y3;
          *n++ = z2;

          *n++ = x4;
          *n++ = y4;
          *n++ = z2;

          *d++ = i1;
          *d++ = i4;
          *d++ = i2;

          //Point i1
          *t++ = j*P;
          *t++ = i*T;
          //Point i4
          *t++ = (j+1)*P;
          *t++ = i*T;
          //Point i2
          *t++ = j*P;
          *t++ = (i+1)*T;

          *d++ = i2;
          *d++ = i4;
          *d++ = i3;

          //Point i2
          *t++ = j*P;
          *t++ = (i+1)*T;
          //Point i4
          *t++ = (j+1)*P;
          *t++ = i*T;
          //Point i3
          *t++ = (j+1)*P;
          *t++ = (i+1)*T;
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 3*sizeof(float), &spherePositionArray[0]);

  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 3*sizeof(float), &sphereNormalArray[0]);
  glEnable(GL_NORMALIZE);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 2*sizeof(float), &sphereTexcoordArray[0]);

  // Texture checker board
  int width=150;
  int height=150;
  unsigned char image[3*width*height];
  genCheckerboard(width,height,image);

  glEnable (GL_TEXTURE_2D); // Activation de la texturation 2D
  glGenTextures (1, &texture); // Génération d’une texture OpenGL
  glBindTexture (GL_TEXTURE_2D, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // La commande suivante remplit la texture (sur GPU) avec les données de l’image
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
  GL_UNSIGNED_BYTE, image);

  // Camera initialization
  fovAngle = 45.f;
  nearPlane = 0.01;
  farPlane = 10.0;
  camPhi = M_PI;
  camTheta = 0;
  camDist2Target = 5;
  camTargetX = 0.0;
  camTargetY = 0.0;
  camTargetZ = 0.0;

  // Lighting
  GLfloat light_position0[4]={10.0f, 10.0f, 10.0f, 1.0f};
  GLfloat color0[4] = {1.0f,1.0f,0.9f,1.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, color0);
  glLightfv (GL_LIGHT0, GL_SPECULAR, color0);

  GLfloat light_position1[4] = {-10.0f, 0.0f, -1.0f, 1.0f};
  GLfloat color1[4] = {0.0f, 0.1f, 0.3f,1.0f};
  glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
  glLightfv (GL_LIGHT1, GL_SPECULAR, color1);


  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
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

void display () { setupCamera (); glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.  // Put your drawing code (glBegin, glVertex, glCallList, glDrawArray, etc) here

    glSphereWithMat(cos(2*M_PI*currentTime/1000),sin(2*M_PI*currentTime/1000),0.0, 0.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0);

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
  case '1':
    glEnable(GL_LIGHT0);
    break;
  case 'a':
    pause = !pause;
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
    camPhi += 0.1;
    camTheta += 0.1;
    glutPostRedisplay();
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {
  if (!pause)
    currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
  glutPostRedisplay ();
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

