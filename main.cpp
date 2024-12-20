/*
 * Solar.c
 *
 * Program to demonstrate how to use a local
 * coordinate method to position parts of a
 * model in relation to other model parts.
 *
 * Draws a simple solar system, with a sun, planet and moon.
 * Based on sample code from the OpenGL programming guide
 *        by Woo, Neider, Davis.  Addison-Wesley.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book
 *        3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *        by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 * USAGE:
 *    Press "r" key to toggle (off and on) running the animation
 *    Press "s" key to single-step animation
 *    The up arrow key and down array key control the
 *            time step used in the animation rate.  Each key
 *            press multiplies or divides the times by a factor
 *            of two (2).
 *      Press ESCAPE to exit.
 *
 */

#include "Solar.hpp"
#include <stdlib.h>
#include <GL/glut.h>  // OpenGL Graphics Utility Library
#include <iostream>

static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;

static float HourOfDay = 0.0;
static float AnimateIncrement = 6.0;  // Time step for animation (hours)
static float viewAngle = 15.0;

const float MERCURY_ORBIT_DAYS = 88.0, MERCURY_ROTATION_HOURS = 1407.6;
const float VENUS_ORBIT_DAYS = 225.0, VENUS_ROTATION_HOURS = 5832.5;
const float EARTH_ORBIT_DAYS = 365.0, EARTH_ROTATION_HOURS = 24.0;
const float MARS_ORBIT_DAYS = 687.0, MARS_ROTATION_HOURS = 24.6;
const float JUPITER_ORBIT_DAYS = 4332.0, JUPITER_ROTATION_HOURS = 9.8;
const float SATURN_ORBIT_DAYS = 10759.0, SATURN_ROTATION_HOURS = 10.25;
const float URANUS_ORBIT_DAYS = 30685.0, URANUS_ROTATION_HOURS = 17.25;
const float NEPTUNE_ORBIT_DAYS = 60190.0, NEPTUNE_ROTATION_HOURS = 16.1;

static float mercuryOrbitPosition = 0.0;
static float venusOrbitPosition = 0.0;
static float earthOrbitPosition = 0.0;
static float marsOrbitPosition = 0.0;
static float jupiterOrbitPosition = 0.0;
static float saturnOrbitPosition = 0.0;
static float uranusOrbitPosition = 0.0;
static float neptuneOrbitPosition = 0.0;

void updateOrbitPositions() {
    mercuryOrbitPosition += AnimateIncrement / MERCURY_ORBIT_DAYS;
    venusOrbitPosition += AnimateIncrement / VENUS_ORBIT_DAYS;
    earthOrbitPosition += AnimateIncrement / EARTH_ORBIT_DAYS;
    marsOrbitPosition += AnimateIncrement / MARS_ORBIT_DAYS;
    jupiterOrbitPosition += AnimateIncrement / JUPITER_ORBIT_DAYS;
    saturnOrbitPosition += AnimateIncrement / SATURN_ORBIT_DAYS;
    uranusOrbitPosition += AnimateIncrement / URANUS_ORBIT_DAYS;
    neptuneOrbitPosition += AnimateIncrement / NEPTUNE_ORBIT_DAYS;
}

static void KeyPressFunc(unsigned char Key, int x, int y) {
    switch (Key) {
        case 'R':
        case 'r':
            if (singleStep) {
                singleStep = GL_FALSE;
                spinMode = GL_TRUE;
            } else {
                spinMode = !spinMode;
            }
            break;
        case 's':
        case 'S':
            singleStep = GL_TRUE;
            spinMode = GL_TRUE;
            break;
        case '+':
            viewAngle += 5.0;
            if (viewAngle > 90.0) viewAngle = 90.0;
            break;
        case '-':
            viewAngle -= 5.0;
            if (viewAngle < -90.0) viewAngle = -90.0;
            break;
        case 27:
            exit(1);
    }
}

static void Key_up(void) {
    AnimateIncrement *= 2.0;
}

static void Key_down(void) {
    AnimateIncrement /= 2.0;
}

static void SpecialKeyFunc(int Key, int x, int y) {
    switch (Key) {
        case GLUT_KEY_UP:
            Key_up();
            break;
        case GLUT_KEY_DOWN:
            Key_down();
            break;
    }
}

static void Animate(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (spinMode) {
        HourOfDay += AnimateIncrement;
        updateOrbitPositions();
        HourOfDay = HourOfDay - ((int)(HourOfDay / 24)) * 24;
    }

    glLoadIdentity();
    glTranslatef(0.0, 0.0, -20.0);
    glRotatef(viewAngle, 1.0, 0.0, 0.0);

    // Draw the Sun
    glColor3f(1.0, 1.0, 0.0);
    glutWireSphere(1.0, 15, 15);

    // Define planet colors
    float mercuryColor[3] = {0.5, 0.5, 0.5};
    float venusColor[3] = {1.0, 0.7, 0.0};
    float earthColor[3] = {0.2, 0.2, 1.0};
    float marsColor[3] = {1.0, 0.3, 0.3};
    float jupiterColor[3] = {0.9, 0.6, 0.3};
    float saturnColor[3] = {0.8, 0.7, 0.5};
    float uranusColor[3] = {0.5, 0.8, 1.0};
    float neptuneColor[3] = {0.3, 0.3, 0.8};

    glPushMatrix();
    glRotatef(360.0 * mercuryOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    glColor3f(mercuryColor[0], mercuryColor[1], mercuryColor[2]);
    glutWireSphere(0.2, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * venusOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(3.0, 0.0, 0.0);
    glColor3f(venusColor[0], venusColor[1], venusColor[2]);
    glutWireSphere(0.3, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * earthOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, 0.0);
    glColor3f(earthColor[0], earthColor[1], earthColor[2]);
    glutWireSphere(0.4, 10, 10);

    glPushMatrix();
    glRotatef(360.0 * 12.0 * (earthOrbitPosition - (int)earthOrbitPosition), 0.0, 1.0, 0.0);
    glTranslatef(0.5, 0.0, 0.0);
    glColor3f(0.7, 0.7, 0.7);
    glutWireSphere(0.1, 5, 5);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * marsOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(6.0, 0.0, 0.0);
    glColor3f(marsColor[0], marsColor[1], marsColor[2]);
    glutWireSphere(0.3, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(360.0 * jupiterOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(8.0, 0.0, 0.0);
    glColor3f(jupiterColor[0], jupiterColor[1], jupiterColor[2]);
    glutWireSphere(0.9, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(360.0 * saturnOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(10.0, 0.0, 0.0);
    glColor3f(saturnColor[0], saturnColor[1], saturnColor[2]);
    glutWireSphere(0.7, 10, 10);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(360.0 * uranusOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(12.0, 0.0, 0.0);
    glColor3f(uranusColor[0], uranusColor[1], uranusColor[2]);
    glutWireSphere(0.6, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * neptuneOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(14.0, 0.0, 0.0);
    glColor3f(neptuneColor[0], neptuneColor[1], neptuneColor[2]);
    glutWireSphere(0.6, 10, 10);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();

    if (singleStep) {
        spinMode = GL_FALSE;
    }

    glutPostRedisplay();
}

void OpenGLInit(void) {
    printf("to change the POV press + or -");
    fflush(stdout);
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
}

static void ResizeWindow(int w, int h) {
    float aspectRatio = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspectRatio, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("TP2 Solar System");

    OpenGLInit();

    glutKeyboardFunc(KeyPressFunc);
    glutSpecialFunc(SpecialKeyFunc);
    glutReshapeFunc(ResizeWindow);
    glutDisplayFunc(Animate);

    glutMainLoop();
    return 0;
}
