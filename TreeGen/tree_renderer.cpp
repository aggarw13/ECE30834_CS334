/*
	File for rendering all objects and triangles
*/
#include "tree_renderer.h"
#include "glutCallBacks.h"
#include "part_model.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "obj_parser.h"
#include<fstream>

#define WINDOW_H 800
#define WINDOW_W 600

//bool tree_renderer::firstRender = true;

tree_renderer::tree_renderer(tree_generator * generator)
{
	/* Window information */
	treeVertexCount = 0;

	this->simCounter = 0;
	this->generator = generator;
	yaw = 0.0;
	pitch = 0.0;
	translate_x = 0.0;
	prevz_pos = -5.00;
	translate_y = 0.0;
	translate_z = 0.0;

}

tree_renderer::~tree_renderer()
{
//	//delete vertices;
//	delete lineVertices;
//	delete colors;
//	delete colors;
}


tuple3d tree_renderer::calculateAngles(vec3 dir)
{
	tuple3d angles;
	angles.first = (180 * atan(dir.y / dir.z) / PI < 0)?  180 * (atan(dir.y / dir.z) / PI + 0.5) : 180 * atan(dir.y / dir.z) / PI;
	angles.second = (180 * atan(dir.x / dir.z) / PI < 0)? 180 * (atan(dir.x / dir.z) / PI + 1) : 180 * atan(dir.x / dir.z) / PI;
	angles.third = (180 * atan(dir.y / dir.x) / PI < 0 )? 180 * (atan(dir.y / dir.x) / PI + 1) : 180 * atan(dir.y / dir.x) / PI;
	
	return angles;
}


/**
*	Function invoked when window system events are not being received
*/
void tree_renderer::idle()
{
	/* Redraw the window */
	glutPostRedisplay();
}

/**
*	Function invoked when an event on regular keys occur
*/
void tree_renderer::keyboard(unsigned char k, int x, int y)
{
	/* Show which key was pressed */
	//std::cout << "Pressed \"" << k << "\" ASCII: " << (int)k << std::endl;

	if (int(k) == 98)
		this->translate_z -= 2.0;
	else if (k == 102)
		translate_z +=2.0;
	else if (k == 'l')
		translate_x +=2.0;
	else if (k == 'r')
		translate_x -=2.0;
	else if (k == 'd')
		translate_y +=2.0;
	else if (k == 'u')
		translate_y -=2.0;
	else if (k == 27)
		exit(0);
}

/**
*	Function invoked when an event on a special keys occur
*/
void tree_renderer::special(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		pitch -= 10.0;
	else if (key == GLUT_KEY_DOWN)
		pitch += 10.0;
	else if (key == GLUT_KEY_RIGHT)
		yaw -= 10.0;
	else if (key == GLUT_KEY_LEFT)
		yaw += 10.0;
}

/**
*	Set OpenGL initial state
*/
void tree_renderer::init()
{
 


}

void tree_renderer::generateShaderInfo(float * lineVertices, float * colors, int * lineVerticesCount)
{
	*lineVerticesCount = this->parts_list.size() * 2;
	lineVertices = new GLfloat[*lineVerticesCount * 3];
	colors = new GLfloat[*lineVerticesCount * 3];
	int vertex_no = 0;
	vec3 vertex;

	for (part_model * part : this->parts_list)
	{
		vertex = part->getBasePos().getVector() + (part->getDir() * part->getLength()).getVector();

		printf("Line Vertices : (%f, %f, %f) (%f, %f, %f)  %f\n", part->getBasePos().first, part->getBasePos().second, part->getBasePos().third, vertex.x, vertex.y, vertex.z);

		lineVertices[vertex_no++] = (part->getBasePos().first / WINDOW_W) * 2 - 1;
		lineVertices[vertex_no++] = (part->getBasePos().second / WINDOW_H) * 2 - 1;
		lineVertices[vertex_no++] = -0.5; // part->getBasePos().third;

		colors[vertex_no - 3] = part->getColor().first;
		colors[vertex_no - 2] = part->getColor().second;
		colors[vertex_no - 1] = part->getColor().third;

		lineVertices[vertex_no++] = (vertex.x / WINDOW_W) * 2 - 1;
		lineVertices[vertex_no++] = (vertex.y / WINDOW_H) * 2 - 1;
		lineVertices[vertex_no++] = -0.5;// vertex.z;

		colors[vertex_no - 3] = part->getColor().first;
		colors[vertex_no - 2] = part->getColor().second;
		colors[vertex_no - 1] = part->getColor().third;

		printf("Vertices position : (%f, %f, %f) (%f, %f, %f)", lineVertices[vertex_no - 6], lineVertices[vertex_no - 5], lineVertices[vertex_no - 4], lineVertices[vertex_no - 3], lineVertices[vertex_no - 2], lineVertices[vertex_no - 1]);

	//printf("Co-ordinate : %f  %f\n", x, y);
	//vertices_list.push_back(pos{x, y + 0.2f, -2 });
	//treeVertexCount++;
	}
}
void tree_renderer::addModel(part_model* part)
{
  this->parts_list.push_back(part);
}

void tree_renderer::clearPartsList()
{
	for (part_model * model : this->parts_list)
	{
		delete model;
	}
	this->parts_list.clear();


}

/*void tree_renderer::renderScene(int argc, char ** argv )
{
	/* Initialize the GLUT window 
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(30, 30);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	windowID = glutCreateWindow("OpenGL/FreeGLUT - Example 2: Hello Square");

	/* Set OpenGL initial state *
	init();

	/* Callback functions 
	glutDisplayFunc([](){display(); });
	glutIdleFunc(glutCallBacks.idle);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);

	/* Start the main GLUT loop *
	glutMainLoop();
}*/
