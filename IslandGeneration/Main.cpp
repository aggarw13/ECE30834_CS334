/*
 * Main.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

#include "IslandGeneration.h"


// Points
float * setPoints;
int numberofPoints;
Mode pointMode;
float voronoiPoints[1000];
int idx = 0;

// Voronoi Object
VD vd;

void init(){
	// White color
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, windowHeight, 0.0);

	// Call random point generator
	numberofPoints = inputter(&pointMode);
	switch(pointMode){
		case Spiral:
			generateSpiralPoints(&setPoints, numberofPoints);
			break;
		case Random:
			generateRandomPoints(&setPoints, numberofPoints);
			break;
		case Grid:
			generateGridPoints(&setPoints, numberofPoints);
			break;
		default: break;
	}

	vd = generateVoronoi(&setPoints, numberofPoints);

	// Fill in points for voronoi edges
	VD::Face_iterator ft = vd.faces_begin();

	for(;ft != vd.faces_end();ft++){
		Ccb_halfedge_circulator et_start = (*ft).ccb();
		Ccb_halfedge_circulator et = et_start;
		do {
			if(et->has_source() && et->has_target()){
				voronoiPoints[idx++] = et->source()->point().x();
				voronoiPoints[idx++] = et->source()->point().y();
				voronoiPoints[idx++] = et->target()->point().x();
				voronoiPoints[idx++] = et->target()->point().y();
			}
		} while ( ++et != et_start );
	}


}

void display(){
	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw the line */
	/* Step 1: Enable the clients for the vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Step 2a: Set up the array for the line and draw it */

	glPointSize(5.0);
	glVertexPointer(2, GL_FLOAT, 0, voronoiPoints);
	glColor3f(0.0, 0.0, 255.0);
	glDrawArrays(GL_LINES, 0, idx / 2);

	glPointSize(5.0);
	glVertexPointer(2, GL_FLOAT, 0, setPoints);
	glColor3f(0.0, 255.0, 0.0);
	glDrawArrays(GL_POINTS, 0, numberofPoints);




	/* Step 3: Disable the clients */
	glDisableClientState(GL_VERTEX_ARRAY);

	/* Force execution of OpenGL commands */
	glFlush();

	/* Swap buffers for animation */
	glutSwapBuffers();

}

void idle()
{
	/* Redraw the window */
	glutPostRedisplay();
}



void keyboard(unsigned char k, int x, int y)
{
	/* Show which key was pressed */
	std::cout << "Pressed \"" << k << "\" ASCII: " << (int)k << std::endl;

	/* Close application if ESC is pressed */
	if (k == 27)
	{
		exit(0);
	}
}

void freeStuffs(){
	free(setPoints);
}

int main(int argc, char ** argv){
	/* Initialize the GLUT window */
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(30, 30);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Points");

	// Initialization
	init();

	/* Callback functions */
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	/* Start the main GLUT loop */
	/* NOTE: No code runs after this */
	glutMainLoop();
	freeStuffs();
}



