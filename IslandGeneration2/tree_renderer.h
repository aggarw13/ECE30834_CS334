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
        
		vector<part_model *> parts_list;
		tree_generator * generator;
		int treeVertexCount;
		float pitch, yaw, translate_x, translate_y, translate_z;
		float prevz_pos;
		short simCounter;
		


		void clearPartsList();
		void generateArrays();
		tuple3d calculateAngles(vec3);

	public:	
		int simThresh, iterations;
		tree_renderer(tree_generator *);
		~tree_renderer();
		void init();
		void idle();
		void display();
		void generateShaderInfo(float *, float *, int *);
		void addModel(part_model * part);
		void keyboard(unsigned char k, int x, int y);
		void special(int key, int x, int y);
		//void renderScene(int, char **);
};
