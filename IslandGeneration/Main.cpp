/*
 * Main.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

#include <fstream>
#include <string>
//#include "GL/gl.h"

#include "IslandGeneration.h"

/* The vertex buffers and the vertex array*/
GLuint vertexVbo;
GLuint colorVbo;;
GLuint circlesVao;
GLuint pointsVbo;
GLuint pointsColorsVbo;
GLuint pointsVao;
GLuint voronoiVbo;
GLuint voronoiVao;
GLuint voronoiColorsVbo;

/* The source code of the vertex and fragment shaders */
std::string vertexShaderCode;
std::string fragmentShaderCode;

/* References for the vertex and fragment shaders and the shader program */
GLuint vertexShader = 0;
GLuint fragmentShader = 0;
GLuint shaderProgram = 0;

// Vertices
float circleVertices[10000];
float circleColors[10000];
int circlePointCt;

// Points
float * setPoints;
int numberofPoints = 100;
float setPointsColors[100 * 3];
Mode pointMode = Random; // Mode for points scattering

float voronoiPoints[10000];
float voronoiColors[10000];
int idx = 0; // idx for putting in points for voronoiPoints

// Voronoi Object
VD vd;

// Elevation 2D Array
terrain terrainInfo[windowHeight * windowWidth];

int loadFile(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);
		text += line + "\n";
    }
	return 0;
}

float normalize(int pos, int max){
	return  2.0f * ((float) pos / (float) max) - 1.0f;
}

void initShadersVAOS(){
	/* Initialize the vertex shader (generate, load, compile and check errors) */
		loadFile("vertex.glsl", vertexShaderCode);
		const char* vertexSource = vertexShaderCode.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		GLint status = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			std::cout << "Error while compiling the vertex shader: " << std::endl << buffer << std::endl;
		}

		/* Initialize the fragment shader (generate, load, compile and check errors) */
		loadFile("fragment.glsl", fragmentShaderCode);
		const char* fragmentSource = fragmentShaderCode.c_str();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		status = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			std::cout << "Error while compiling the fragment shader: " << std::endl << buffer << std::endl;
		}

		/******************************************Points Data VAO*********************************************/
		glGenBuffers(1, &pointsVbo);
		glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
		glBufferData(GL_ARRAY_BUFFER, numberofPoints * 3 * sizeof(float), setPoints, GL_STATIC_DRAW);

		/* Initialize the Vertex Buffer Object for the colors of the vertices */
		glGenBuffers(1, &pointsColorsVbo);
		glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(setPointsColors), setPointsColors, GL_STATIC_DRAW);

		/* Define the Vertex Array Object for the points */
		glGenVertexArrays(1, &pointsVao);
		glBindVertexArray(pointsVao);
		glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		/******************************************Circle Data VAO*********************************************/
		/* Initialize the Vertex Buffer Object for the location of the vertices */
		glGenBuffers(1, &vertexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

		/* Initialize the Vertex Buffer Object for the colors of the vertices */
		glGenBuffers(1, &colorVbo);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(circleColors), circleColors, GL_STATIC_DRAW);

		/* Define the Vertex Array Object for the circles */
		glGenVertexArrays(1, &circlesVao);
		glBindVertexArray(circlesVao);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);





		/******************************************Voronoi Data VAO*********************************************/
		glGenBuffers(1, &voronoiVbo);
		glBindBuffer(GL_ARRAY_BUFFER, voronoiVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(voronoiPoints), voronoiPoints, GL_STATIC_DRAW);

		/* Initialize the Vertex Buffer Object for the colors of the vertices */
		glGenBuffers(1, &voronoiColorsVbo);
		glBindBuffer(GL_ARRAY_BUFFER, voronoiColorsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(voronoiColors), voronoiColors, GL_STATIC_DRAW);

		/* Define the Vertex Array Object for the voronoi */
		glGenVertexArrays(1, &voronoiVao);
		glBindVertexArray(voronoiVao);
		glBindBuffer(GL_ARRAY_BUFFER, voronoiVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, voronoiColorsVbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);


		/* Initialize the shader program */
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindAttribLocation(shaderProgram, 0, "inPoint");
		glBindAttribLocation(shaderProgram, 1, "inColor");
		glLinkProgram(shaderProgram);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, windowWidth, windowHeight, 0.0);
}

void init(){
	// White color
	glClearColor(1.0, 1.0, 1.0, 0.0);

	/* Enable the depth buffer */
    glEnable(GL_DEPTH_TEST);

//    glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0.0, windowWidth, windowHeight, 0.0);

	// Call random point generator
	// numberofPoints = inputter(&pointMode);
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
				voronoiPoints[idx++] = 0;
				voronoiPoints[idx++] = et->target()->point().x();
				voronoiPoints[idx++] = et->target()->point().y();
				voronoiPoints[idx++] = 0;

			}
//			else {
//				if(et->has_source){
//					voronoiPoints[idx++] = et->source()->point().x();
//					voronoiPoints[idx++] = et->source()->point().y();
//					voronoiPoints[idx++] = 0;
//
//				}
//			}
		} while ( ++et != et_start );
	}

	//fill in colors
	int i = 0;
	while(i < numberofPoints * 3){
		setPointsColors[i++] = 0.0f;
		setPointsColors[i++] = 1.0f;
		setPointsColors[i++] = 0.0f;
	}

	i = 0;
	while(i < idx){
		voronoiColors[i++] = 1.0f;
		voronoiColors[i++] = 0.0f;
		voronoiColors[i++] = 0.0f;
	}

	// Define elevation
	circlePointCt = terrainInput(terrainInfo, circleVertices, circleColors);

	initShadersVAOS();

}


void drawAll(int circlePointCt){
	glBindVertexArray(circlesVao);
	glDrawArrays(GL_TRIANGLES, 0, circlePointCt);
	glPointSize(2.0f);
	glBindVertexArray(pointsVao);
	glDrawArrays(GL_POINTS, 0, numberofPoints);
	glBindVertexArray(voronoiVao);
	glDrawArrays(GL_LINES, 0, idx / 3);

}

void display(){
	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw the line */
	/* Step 1: Enable the clients for the vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Step 2a: Set up the array for the line and draw it */

//	glPointSize(5.0);
//	glVertexPointer(2, GL_FLOAT, 0, voronoiPoints);
//	glColor3f(0.0, 0.0, 255.0);
//	glDrawArrays(GL_LINES, 0, idx / 2);
//
//	glPointSize(5.0);
//	glVertexPointer(2, GL_FLOAT, 0, setPoints);
//	glColor3f(0.0, 255.0, 0.0);
//	glDrawArrays(GL_POINTS, 0, numberofPoints);




	// Use vertex to draw intensity
	glUseProgram(shaderProgram);
	drawAll(circlePointCt);

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

	/* Init GLEW */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;


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



