#pragma once
#include<iostream>
#include"common_types.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm/mat4x4.hpp"
#include "part_model.h"
#include "tree_generator.h"
#include<list>
#include<vector>

class tree_generator;

using namespace std;
class tree_renderer
{
	private:
		static struct pos
		{
			float x;
			float y;
			float z;
		};
		
		list<pos> vertices_list;
		vector<part_model *> parts_list;
		tree_generator * generator;
		int treeVertexCount;
		float pitch, yaw, translate_x, translate_y, translate_z;
		float prevz_pos;
		short simCounter;
		
		/************Shader Rendering Parameters****************/
		//* The transformation matrices */
		GLuint cylinderVerticesVbo;
		GLuint coneVerticesVbo;
		GLuint lineVerticesVbo;
		GLuint colorVbo;
		GLuint modelVao, lineVao;
		GLfloat * cylinderVertices, * cylinderCoords, * cylinderNormals;
		GLfloat * coneVertices, * coneCoords, * coneNormals;
		GLfloat * lineVertices, * colors;

		int cylinderVerticesCount, coneVerticesCount, lineVerticesCount;

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
		
		std::string vertexShaderCode;
		std::string fragmentShaderCode;

		void clearPartsList();
		void generateArrays();
		tuple3d calculateAngles(vec3);

	public:	
		int simThresh, iterations;
		bool firstRender;
		tree_renderer(tree_generator *);
		~tree_renderer();
		void init();
		void idle();
		void display();
		void generateShaderInfo();
		void addModel(part_model * part);
		void keyboard(unsigned char k, int x, int y);
		void special(int key, int x, int y);
		//void renderScene(int, char **);
};