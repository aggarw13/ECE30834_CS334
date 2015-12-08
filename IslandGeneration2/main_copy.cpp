
#include <cstdlib>
#include <stdlib.h>
#include "grammar_parser.h"
#include "tree_generator.h"
#include "glutCallBacks.h"
#include "obj_parser.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define WINDOW_W  800
#define WINDOW_H  600

GLuint cylinderVerticesVbo;
GLuint coneVerticesVbo;
GLuint lineVerticesVbo;
GLuint colorVbo;
GLuint modelVao, lineVao, cylinderVao;
GLfloat * cylinderVertices, *cylinderCoords, *cylinderNormals;
GLfloat * coneVertices, *coneCoords, *coneNormals;
GLfloat * lineVertices, *colors;

int cylinderVertexCount, coneVertexCount, lineVerticesCount;
const char *  fragmentSource, *vertexSource;


glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projMatrix;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

/* The location of the transformation matrices */
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projMatrixLocation;
GLint colorLocation;
std::string vertexShaderCode;
std::string fragmentShaderCode;

grammar_parser * parser;
tree_generator generator;
tree_renderer * rend_obj;

void init()
{
	/* Set clear color */
	glClearColor(0.0, 0.0, 0.0, 1.0);

	/* Set 2D orthogonal projection */

	//gluOrtho2D(0.0,800,600, 0.0);
	std::ifstream ifs; std::string line = "", text = "";

	/* Initialize the vertex shader (generate, load, compile and check errors) */
	ifs.open("	x.glsl", std::ios::in);

	while (ifs.good()) {
		getline(ifs, line);
		text += line + "\n";
	}

	vertexSource = text.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint status = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << "Error while compiling the vertex shader: " << std::endl << buffer << std::endl;
	}


	/* Initialize the fragment shader (generate, load, compile and check errors) */
	ifs.open("tree_fragment.glsl", std::ios::in);
	line = "", text = "";
	while (ifs.good()) {
		getline(ifs, line);
		text += line + "\n";
	}

	fragmentSource = text.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	status = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		std::cout << "Error while compiling the fragment shader: " << std::endl << buffer << std::endl;
	}

	/* Load the information of the cylinder */
	load_obj_file("sphere.obj", cylinderVertices, cylinderCoords, cylinderNormals, cylinderVertexCount);

	/* Load the information of the 3D cone */
	load_obj_file("cone.obj", coneVertices, coneCoords, coneNormals, coneVertexCount);

	/* Initialize the Vertex Buffer Object for the vertices */
	glGenBuffers(1, &cylinderVerticesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVerticesVbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * cylinderVertexCount * sizeof(GLfloat), cylinderVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &coneVerticesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, coneVerticesVbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * coneVertexCount * sizeof(GLfloat), coneVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &lineVerticesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, lineVerticesVbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * lineVerticesCount * sizeof(GLfloat), lineVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * lineVerticesCount * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	/* Define the Vertex Array Object of the 3D model */
	/*	glGenVertexArrays(1, &lineVao);
	glBindVertexArray(lineVao);
	glBindBuffer(GL_ARRAY_BUFFER, lineVerticesVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	*/
	glGenVertexArrays(1, &cylinderVao);
	glBindVertexArray(cylinderVao);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVerticesVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	/* Initialize the shader program */
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 0, "inPoint");
	glBindAttribLocation(shaderProgram, 1, "inColor");
	glLinkProgram(shaderProgram);

	/* Get the location of the uniform variables */
	modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	projMatrixLocation = glGetUniformLocation(shaderProgram, "projMatrix");
	colorLocation = glGetUniformLocation(shaderProgram, "inColor");
	/* Set the shader program in the pipeline */
	glUseProgram(shaderProgram);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	for (part_model * part : rend_obj->parts_list)
	{
		tuple3d orient = rend_obj->calculateAngles(part->getDir().getVector());

		/* Set the view matrix */
		//part->getBasePos().first, part->getBasePos().second, -part->getBasePos().third
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), part->getBasePos().getVector());
		viewMatrix = glm::rotate(translation, 30.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		/* Set the model matrix */
		modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.0f));

		/* Set the projection matrix */
		projMatrix = glm::perspective(glm::radians(45.0f), float(600 / 800), 0.1f, 100.0f);

		/* Send matrix to shader */
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
		//glUniform3f(colorLocation, part->getColor().first, part->getColor().second, part->getColor().third);
		glUniform3f(colorLocation, part->getColor().first, part->getColor().second, part->getColor().third);
		//glUniform3fv(colorLocation, 1, part->getColor().getVector());
		glUseProgram(shaderProgram);
		glBindVertexArray(cylinderVao);
		glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

	}


	glFlush();

	/*if (simCounter <= simThresh)
	simCounter++;


	/* Swap buffers for animation */
	glutSwapBuffers();

}


int main(int argc, char **argv)
{

	/**********************Data Input from Island Map**************/
	/*float elevation[WINDOW_H][WINDOW_W];
	float tree_count;
	float colors[WINDOW_H][WINDOW_W], surface_normals[WINDOW_H][WINDOW_W];
	*/
	//Test parser
	 parser = new grammar_parser(std::string("test_grammar.txt")) ;
	 parser->parseFile();

	//Test string generator
	//generator.parser = parser;
	generator.setParser(parser);
	tuple3d base_location = tuple3d(0.0, 0.0, -5.0f);

	//generator.setTreeBaseLocation(base_location);
	generator.setBaseOrientation(base_location, tuple3d(0, 1, 0));
	//generator.generateTree(parser->model_data->find(std::string("iterations"))->second));
	//_sleep(10000);

	//Instantiate Renderer
	rend_obj = new tree_renderer(&generator);
	generator.setRenderer(rend_obj);
	generator.generateTree(1);
	generator.printTree();
	generator.traverseGeneratedTree();
	rend_obj->generateShaderInfo();
	rend_obj->simThresh = 999;
	rend_obj->iterations = parser->iterations;
	
	//Defining renderer pointer
	glutCallBacks::renderer = rend_obj;

	//Initlaizing Glut Window
	/* Initialize the GLUT window */
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitWindowPosition(30, 30);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	auto windowID = glutCreateWindow("Rendered Test Tree");

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	/* Set OpenGL initial state */
	init();

	/* Callback functions */
	glutDisplayFunc(display);
	glutIdleFunc(glutCallBacks::idle);
	glutKeyboardFunc(glutCallBacks::keyboardHandler);
	glutMouseFunc(glutCallBacks::mouseHandler);
	glutSpecialFunc(glutCallBacks::specialFunc);
	/* Start the main GLUT loop */
	/* NOTE: No code runs after this */
	glutMainLoop();
}