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

// create planet class 
class Planet{
public:
	// constructors
	float Radius, Distance, Orbit, OrbitSpeed, AxialTilt, AxisAni;
	Planet(float radius, float distance, float orbit, float orbitspeed, float axialtilt, float axisani){
		Radius = radius;
		Distance = distance;
		Orbit = orbit;
		OrbitSpeed = orbitspeed;
		AxialTilt = axialtilt;
		AxisAni = axisani;
	}
};

GLUquadricObj * sphere = NULL;
int isAniEnabled = 0;
int isOrbitVisible = 1;
int changeCam = 0;
int Zoom = 50;

// global variables for lighting setup - lighting position, angle, direction, and etc
float lightingPosition[] = {0, 0, -75.0, 1.0}; // spotlight position
float lightingDirection[] = {1.0, 0, 0}; // spotlight direction
static float lightingAngle = 40; // spotlight cone half-angle
static float lightingExponent = 1.0; // spotlight exponent -> attenuation factor

// planets 
Planet Sun(5.0, 0, 0, 0, 0, 0); 
Planet Mercury(1.0, 7, 0, 4.74, 2.11, 0); 
Planet Venus(1.5, 11, 0, 3.5, 177.0, 0);
Planet Earth(2.0, 16, 0, 2.98, 23.44, 0);
Planet Mars(1.2, 21, 0, 2.41, 25.0, 0);
Planet Jupiter(3.5, 28, 0, 1.31, 3.13, 0);
Planet Saturn(3.0, 37, 0, 0.97, 26.7, 0);
Planet Uranus(2.5, 45.5, 0, 0.68, 97.77, 0);
Planet Neptune(2.3, 53.6, 0, 0.54, 28.32, 0);
Planet Pluto(0.3, 59.0, 0, 0.47, 119.6, 0);

// planets texture ids
GLuint Sun_Texture, Mercury_Texture, Venus_Texture, Earth_Texture, Mars_Texture, Jupiter_Texture, Saturn_Texture, Uranus_Texture, Neptune_Texture, Pluto_Texture;

// load the image as texture
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

void initRendering(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// load planets bmp files for texture mapping later
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	// Sun
	Image* sun = loadBMP("sun.bmp");
	Sun_Texture = loadTextureFromImage(sun);
	delete sun;
	// Mercury
	Image* mer = loadBMP("mercury.bmp");
	Mercury_Texture = loadTextureFromImage(mer);
	delete mer;
	// Venus
	Image* ven = loadBMP("venus.bmp");
	Venus_Texture = loadTextureFromImage(ven);
	delete ven;
	// Earth
	Image* ear = loadBMP("earth.bmp");
	Earth_Texture = loadTextureFromImage(ear);
	delete ear;
	// Mars
	Image* mar = loadBMP("mars.bmp");
	Mars_Texture = loadTextureFromImage(mar);
	delete mar;
	// Jupiter
	Image* jup = loadBMP("jupiter.bmp");
	Jupiter_Texture = loadTextureFromImage(jup);
	delete jup;
	// Saturn
	Image* sat = loadBMP("saturn.bmp");
	Saturn_Texture = loadTextureFromImage(sat);
	delete sat;
	//Uranus
	Image* ura = loadBMP("uranus.bmp");
	Uranus_Texture = loadTextureFromImage(ura);
	delete ura;
	// Neptune
	Image* nep = loadBMP("neptune.bmp");
	Neptune_Texture = loadTextureFromImage(nep);
	delete nep;
	// Pluto
	Image* plu = loadBMP("pluto.bmp");
	Pluto_Texture = loadTextureFromImage(plu);
	delete plu;

	// lighting initialization
	glEnable(GL_LIGHTING);
	float lightAmbi[] = {0,0,0,1.0};
	float lightDiffandSpec[] = {1.0, 1.0, 1.0, 1.0};
	float globalAmbi[] = {0.5, 0.5, 0.5, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffandSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDiffandSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbi);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightingPosition);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lightingAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightingDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lightingExponent);
}

void drawSun(){
	sphere = gluNewQuadric();
	//gluQuadricDrawStyle(sphere, GLU_FILL);
	
	//gluQuadricNormals(sphere, GLU_SMOOTH);
	glPushMatrix();
		glRotatef(Sun.Orbit, 0.0, 1.0, 0.0);
		glTranslatef(Sun.Distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(Sun.AxialTilt, 1.0, 0.0, 0.0);
			glRotatef(Sun.AxisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Sun_Texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				gluQuadricTexture(sphere, GL_TRUE);
				gluSphere(sphere, Sun.Radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
			gluDeleteQuadric(sphere);
		glPopMatrix();
	glPopMatrix();
}
// generic function to draw planets
void drawPlanet(float planetOrbit, float planetDistance, float planetAxialTilt, float planetAxisAni, float planetRadius, GLuint planetTexture){
	sphere = gluNewQuadric();
	//gluQuadricDrawStyle(sphere, GLU_FILL);
	
	//gluQuadricNormals(sphere, GLU_SMOOTH);
	glPushMatrix();
		glRotatef(planetOrbit, 0.0, 1.0, 0.0);
		glTranslatef(planetDistance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(planetAxialTilt, 1.0, 0.0, 0.0);
			glRotatef(planetAxisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, planetTexture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				gluQuadricTexture(sphere, GL_TRUE);
				gluSphere(sphere, planetRadius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
			gluDeleteQuadric(sphere);
		glPopMatrix();
	glPopMatrix();
}
// individual function for Saturn since it has the ring around it
void drawSaturn(){
	sphere = gluNewQuadric();
	//gluQuadricDrawStyle(sphere, GLU_FILL);
	
	//gluQuadricNormals(sphere, GLU_SMOOTH);
	glPushMatrix();
		glRotatef(Saturn.Orbit, 0.0, 1.0, 0.0);
		glTranslatef(Saturn.Distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(Saturn.AxialTilt, 1.0, 0.0, 0.0);
			glRotatef(Saturn.AxisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, Saturn_Texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				gluQuadricTexture(sphere, GL_TRUE);
				gluSphere(sphere, Saturn.Radius, 20.0, 20.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		gluDeleteQuadric(sphere);
			// Saturn ring
		glPushMatrix();
			glColor3ub(158, 145, 137);
			glRotatef(-63.0, 1.0, 0.0, 0.0);
			glutWireTorus(0.2, 6.0, 30.0, 30.0);
			glutWireTorus(0.4, 5.0, 30.0, 30.0);
		glPopMatrix();
	glPopMatrix();
}
// make calls to the drawPlanet() function to draw all the planets except for the Saturn
void drawALL(){
	drawPlanet(Mercury.Orbit, Mercury.Distance, Mercury.AxialTilt, Mercury.AxisAni, Mercury.Radius, Mercury_Texture);
	drawPlanet(Venus.Orbit, Venus.Distance, Venus.AxialTilt, Venus.AxisAni, Venus.Radius, Venus_Texture);
	drawPlanet(Earth.Orbit, Earth.Distance, Earth.AxialTilt, Earth.AxisAni, Earth.Radius, Earth_Texture);
	drawPlanet(Mars.Orbit, Mars.Distance, Mars.AxialTilt, Mars.AxisAni, Mars.Radius, Mars_Texture);
	drawPlanet(Jupiter.Orbit, Jupiter.Distance, Jupiter.AxialTilt, Jupiter.AxisAni, Jupiter.Radius, Jupiter_Texture);
	drawPlanet(Uranus.Orbit, Uranus.Distance, Uranus.AxialTilt, Uranus.AxisAni, Uranus.Radius, Uranus_Texture);
	drawPlanet(Neptune.Orbit, Neptune.Distance, Neptune.AxialTilt, Neptune.AxisAni, Neptune.Radius, Neptune_Texture);
	drawPlanet(Pluto.Orbit, Pluto.Distance, Pluto.AxialTilt, Pluto.AxisAni, Pluto.Radius, Pluto_Texture);
}
// draw the orbit trails
void drawOrbit(){
	glPushMatrix();
		glColor3ub(255, 255, 255);
		glTranslatef(0.0, 0.0, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, Mercury.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Venus.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Earth.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Mars.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Jupiter.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Saturn.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Uranus.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Neptune.Distance, 100.0, 100.0);
		glutWireTorus(0.001, Pluto.Distance, 100.0, 100.0);
	glPopMatrix();
}
void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}
void drawScene(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set up the properties of the viewing camera.
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(changeCam == 0)gluLookAt(0.0, Zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if(changeCam == 1)gluLookAt(0.0, 0.0, Zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if(changeCam == 2)gluLookAt(0.0, Zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	if(isOrbitVisible == 1) drawOrbit();
	// Render scene
	drawSun();
	drawSaturn();
	drawALL();

	//glDisable(GL_LIGHTING);
	glutSwapBuffers();
	//glFlush();
}

void update(int value){
	if(isAniEnabled){
		Mercury.Orbit += Mercury.OrbitSpeed;
		Venus.Orbit += Venus.OrbitSpeed;
		Earth.Orbit += Earth.OrbitSpeed;
		Mars.Orbit += Mars.OrbitSpeed;
		Jupiter.Orbit += Jupiter.OrbitSpeed;
		Saturn.Orbit += Saturn.OrbitSpeed;
		Uranus.Orbit += Uranus.OrbitSpeed;
		Neptune.Orbit += Neptune.OrbitSpeed;
		Pluto.Orbit += Pluto.OrbitSpeed;
		if(Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.Orbit > 360.0){
			Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.Orbit -= 360.0;
		}
		Mercury.AxisAni += 10.0;
		Venus.AxisAni += 10.0;
		Earth.AxisAni += 10.0;
		Mars.AxisAni += 10.0;
		Jupiter.AxisAni += 10.0;
		Saturn.AxisAni += 10.0;
		Uranus.AxisAni += 10.0;
		Neptune.AxisAni += 10.0;
		Pluto.AxisAni += 10.0;
		if(Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.AxisAni > 360.0){
			Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto.AxisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, update, 1);
	}
}

// keyboard function
void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		case ' ': if (isAniEnabled) isAniEnabled = 0; else{isAniEnabled = 1; update(1);} break;
		case '1': changeCam = 0; glutPostRedisplay(); break;
		case '2': changeCam = 1; glutPostRedisplay(); break;
		case '3': changeCam = 2; glutPostRedisplay(); break;
		case 'o': if (isOrbitVisible) isOrbitVisible = 0; else isOrbitVisible = 1; glutPostRedisplay(); break;
		case 'd': if(Zoom < 100) Zoom++; glutPostRedisplay(); break;
		case 'u': if(Zoom > -75) Zoom--; glutPostRedisplay(); break;
	}
}
// mouse control to zoom in and zoom out -> currently is not working
//void mouse(int btn, int state, int x, int y){

	//if(state == GLUT_UP){
		//if(btn == 3){
			//Zoom++;
		//}else if(btn == 4){
			//Zoom--;
		//}
	//}

	//glutPostRedisplay();
//}
// display the instruction
void instruction(void){
	cout << "SPACE to play/pause the simulation." << endl;
	cout << "1, 2 and 3 to change camera angles." << endl;
	cout << "Press u to Zoom in." << endl;
	cout << "Press d to Zoom out." << endl;
	cout << "Press O to make orbit trail invisible/visible." << endl;
	cout << "ESC to exit the simulation." << endl;
}
// main function
int main(int argc, char** argv)
{
	instruction();
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_DEPTH );
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Solar System");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);

	initRendering(); 
	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
