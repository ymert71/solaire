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
#include "getBMP.h" // Include the texture loader
#include <stdlib.h>
#include <GL/glut.h>
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

GLuint sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture;

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

void LoadTextures() {
    sunTexture = 0; mercuryTexture = 0; venusTexture = 0; earthTexture = 0;
    marsTexture = 0; jupiterTexture = 0; saturnTexture = 0; uranusTexture = 0; neptuneTexture = 0;

    imageFile* image;

    // Load Sun texture
    image = getBMP("sun.bmp");
    if (image) {
        glGenTextures(1, &sunTexture);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Repeat the above logic for all planets

    image = getBMP("mercury.bmp");
    if (image) {
        glGenTextures(1, &mercuryTexture);
        glBindTexture(GL_TEXTURE_2D, mercuryTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Venus
    image = getBMP("venus.bmp");
    if (image) {
        glGenTextures(1, &venusTexture);
        glBindTexture(GL_TEXTURE_2D, venusTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Earth
    image = getBMP("earth.bmp");
    if (image) {
        glGenTextures(1, &earthTexture);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Mars
    image = getBMP("mars.bmp");
    if (image) {
        glGenTextures(1, &marsTexture);
        glBindTexture(GL_TEXTURE_2D, marsTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Jupiter
    image = getBMP("jupiter.bmp");
    if (image) {
        glGenTextures(1, &jupiterTexture);
        glBindTexture(GL_TEXTURE_2D, jupiterTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Saturn
    image = getBMP("saturn.bmp");
    if (image) {
        glGenTextures(1, &saturnTexture);
        glBindTexture(GL_TEXTURE_2D, saturnTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Uranus
    image = getBMP("uranus.bmp");
    if (image) {
        glGenTextures(1, &uranusTexture);
        glBindTexture(GL_TEXTURE_2D, uranusTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }

    // Neptune
    image = getBMP("neptune.bmp");
    if (image) {
        glGenTextures(1, &neptuneTexture);
        glBindTexture(GL_TEXTURE_2D, neptuneTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        delete[] image->data;
        delete image;
    }
}

void RenderSphere(GLuint texture, float radius) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, radius, 30, 30);
    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
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
    glPushMatrix();
    RenderSphere(sunTexture, 1.0);
    glPopMatrix();

    // Render Planets
    glPushMatrix();
    glRotatef(360.0 * mercuryOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    RenderSphere(mercuryTexture, 0.2);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * venusOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(3.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(venusTexture, 0.3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * earthOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, 0.0);
    
    RenderSphere(earthTexture, 0.4);
    glPushMatrix();
    glRotatef(360.0 * 12.0 * (earthOrbitPosition - (int)earthOrbitPosition), 0.0, 1.0, 0.0);
    glTranslatef(0.5, 0.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    RenderSphere(0, 0.1); // Placeholder for moon texture
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * marsOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(6.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(marsTexture, 0.3);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * jupiterOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(8.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(jupiterTexture, 0.9);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * saturnOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(10.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(saturnTexture, 0.7);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * uranusOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(12.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(uranusTexture, 0.6);
    glPopMatrix();

    glPushMatrix();
    glRotatef(360.0 * neptuneOrbitPosition, 0.0, 1.0, 0.0);
    glTranslatef(14.0, 0.0, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    RenderSphere(neptuneTexture, 0.6);
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
    LoadTextures();
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
