#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "imageloader.h"	

using namespace std;

//Globals
//float camPos[] = {0, 0, 3.42f};	//where the camera is

const float SIZE = 10.0f;
#define M_PI (3.14159265)
GLUquadricObj * sphere = NULL;


//float _angle = 0;
float CurrentEarthRotation = 0.00;
float EarthDaysTranspired = 0.00;
float EarthDayIncrement = 0.01;
GLuint _textureID;

GLuint loadTextureFromImage(Image* image) {
	GLuint textureID;
	glGenTextures(1, &textureID); // make room for texture
	glBindTexture(GL_TEXTURE_2D, textureID); // tell OpenGL which texture to edit
	// Maps the image to the texture
	// GL_RGB -> format OpenGL uses for image
	// image->width -> width
	// image->height -> height
	// second 0 -> image border
	// second GL_RGB -> pixels are stored in RGB format
	// GL_UNSIGNED_BYTE -> pixels are stored as unsighed numbers
	// image-> pixels -> actual pixel data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureID; // returns the texture id
}

void initRendering(const char* imageName){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	Image* img = loadBMP(imageName); // load image file
	_textureID = loadTextureFromImage(img);
	delete img;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-7, 0, 0, 1);
	glRotatef(360*(EarthDaysTranspired/365),0,1,0);
	glRotatef(360*CurrentEarthRotation, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);

	gluSphere(sphere, 5.0, 20, 20); // draw the sphere
	glPopMatrix();
	glutSwapBuffers();
}
// Calls the update function every 20 milliseconds
void update(int value) {
	CurrentEarthRotation += EarthDayIncrement;
	EarthDaysTranspired += EarthDayIncrement;
	if(EarthDaysTranspired == 356){
		EarthDaysTranspired = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(20, update, 1);
}

//OpenGL functions
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
	}
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Planet");	//creates the window
	initRendering("earth.bmp");

	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutTimerFunc(20, update, 0);

	//glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);

	//init();

	//instanciate our own class on stack
	//Boilerplate b;
	//b.hello();	//call the hello function

	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
