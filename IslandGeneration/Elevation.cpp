/*
 * Elevation.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glut.h"

#include "IslandGeneration.h"

int verticesCt = 0;
float intensity[windowWidth][windowHeight];

int terrainInput(terrain * terrainArr, float * circleVertices, float * circleColor){
//	// fill in x and y
//	int i, j, k;
//	for(i = 0; i < windowWidth; i++){
//		for(j = 0; j < windowHeight; j++){
//			terrainArr[k].x = i;
//			terrainArr[k++].y = j;
//		}
//	}

	// intensity point input (define in-program for now)
	terrain intensity1, intensity2, intensity3;
	srand((unsigned)time(0));
	intensity1.x =  (rand() % (int) windowWidth) + 1;
	intensity1.y = (rand() % (int) windowHeight) + 1;
	intensity1.intensity = 0.8;
	intensity2.x =  (rand() % (int) windowWidth) + 1;
	intensity2.y = (rand() % (int) windowHeight) + 1;
	intensity2.intensity = 0.5;
	intensity3.x =  (rand() % (int) windowWidth) + 1;
	intensity3.y = (rand() % (int) windowHeight) + 1;
	intensity3.intensity = 0.2;



//	// Modify actual terrain information in terrainArr
//	terrainArr[intensity1.x * windowHeight + intensity1.y] = intensity1;
//	terrainArr[intensity2.x * windowHeight + intensity2.y] = intensity2;
//	terrainArr[intensity3.x * windowHeight + intensity3.y] = intensity3;
//
//	// Modify the intensity of nearby positions of that 'clicked' point on the terrain
//	int intensity1idx = intensity1.x * windowHeight + intensity1.y;
//	int intensity2idx = intensity2.x * windowHeight + intensity2.y;
//	int intensity3idx = intensity3.x * windowHeight + intensity3.y;
//	int allIdx[3] = {intensity1idx, intensity2idx, intensity3idx};

	int circlesCt = 3;
	terrain allIntensity[] = {intensity1, intensity2, intensity3};



	float xpos0, xpos1, ypos0, ypos1, r;
	float theta = 0;
	int vi = 0, coli = 0, i;

	int x,y;
	i = 0;
	for(x = 0; x < windowWidth; x++){
		for(y = 0; y < windowHeight; y++){
			for(i = 0; i < circlesCt; i++){
				if(x == allIntensity[i].x && y == allIntensity[i].y){
					intensity[x][y] = allIntensity[i].intensity;
				}
				else{
					intensity[x][y] = 0.0f;
				}
			}
		}
	}



	r = 120;
	terrain center;
	for(i = 0; i < circlesCt; i++){
		center = allIntensity[i];
		for(theta = 0; theta < (2 * PI); theta += (PI /DELTA)){
			xpos0 = r * cos(theta) + center.x;
			ypos0 = r * sin(theta) + center.y;
			xpos1 = r * cos(theta + (PI /DELTA)) + center.x;
			ypos1 = r * sin(theta + (PI /DELTA)) + center.y;

			// center
			circleVertices[vi++] = normalize(center.x, windowWidth);
			circleVertices[vi++] = normalize(center.y, windowHeight);
			circleVertices[vi++] = 0;
			circleColor[coli++] = center.intensity;
			circleColor[coli++] = center.intensity;
			circleColor[coli++] = center.intensity;

			// first vertex of triangle
			circleVertices[vi++] = normalize(xpos0, windowWidth);
			circleVertices[vi++] = normalize(ypos0, windowHeight);
			circleVertices[vi++] = 0;
			circleColor[coli++] = 0.9f;
			circleColor[coli++] = 0.9f;
			circleColor[coli++] = 0.9f;

			// second vertex of triangle
			circleVertices[vi++] = normalize(xpos1, windowWidth);
			circleVertices[vi++] = normalize(ypos1, windowHeight);
			circleVertices[vi++] = 0;
			circleColor[coli++] = 0.9f;
			circleColor[coli++] = 0.9f;
			circleColor[coli++] = 0.9f;

		}
	}
	return vi / 3;

}



