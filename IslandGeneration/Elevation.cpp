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

void terrainInput(terrain * terrainArr, float * circleVertices, float * circleColor){
	// fill in x and y
	int i, j, k;
	for(i = 0; i < windowWidth; i++){
		for(j = 0; j < windowHeight; j++){
			terrainArr[k].x = i;
			terrainArr[k++].y = j;
		}
	}

	// intensity point input (define in-program for now)
	terrain intensity1, intensity2, intensity3;
	srand((unsigned)time(0));
	intensity1.x =  (rand() % (int) windowWidth) + 1;
	intensity1.y = (rand() % (int) windowHeight) + 1;
	intensity1.intensity = 180;
	intensity2.x =  (rand() % (int) windowWidth) + 1;
	intensity2.y = (rand() % (int) windowHeight) + 1;
	intensity2.intensity = 90;
	intensity3.x =  (rand() % (int) windowWidth) + 1;
	intensity3.y = (rand() % (int) windowHeight) + 1;
	intensity3.intensity = 20;

	// Modify actual terrain information in terrainArr
	terrainArr[intensity1.x * windowHeight + intensity1.y] = intensity1;
	terrainArr[intensity2.x * windowHeight + intensity2.y] = intensity2;
	terrainArr[intensity3.x * windowHeight + intensity3.y] = intensity3;

	// Modify the intensity of nearby positions of that 'clicked' point on the terrain
	int intensity1idx = intensity1.x * windowHeight + intensity1.y;
	int intensity2idx = intensity2.x * windowHeight + intensity2.y;
	int intensity3idx = intensity3.x * windowHeight + intensity3.y;
	int allIdx[3] = {intensity1idx, intensity2idx, intensity3idx};

	int xpos, ypos, r;
	float theta;
	int vi, coli;
	int decIntensity = 255; // the intensity variable, the initial value will depend on the duration of 'click' given by the user
	for(r = 1; decIntensity > 0; r += 1){
		for(i = 0; i < 3; i++){
			vi = 0; coli = 0;
			for(theta = 0; theta < 2 * PI; theta += (PI /DELTA)){
					xpos = r * cos(theta) + terrainArr[allIdx[i]].x;
					ypos = r * sin(theta) + terrainArr[allIdx[i]].y;
					if(xpos > 0 && ypos > 0){
						terrainArr[xpos * windowHeight + ypos].intensity = decIntensity * (255 - r);
					}

					if(r == MAX_RADIUS){
						circleVertices[vi++] = xpos / windowWidth;
						circleVertices[vi++] = ypos / windowHeight;
						circleVertices[vi++] = 0.0f;
						circleColor[coli++] = (255 - terrainArr[xpos * windowHeight + ypos].intensity) / 255;
						circleColor[coli++] = (255 - terrainArr[xpos * windowHeight + ypos].intensity) / 255;
						circleColor[coli++] = (255 - terrainArr[xpos * windowHeight + ypos].intensity) / 255;
					}
			}
			verticesCt += vi;
		}
	}

}

void drawTerrainIntensity(terrain * terrainArr){
	glDrawArrays(GL_TRIANGLES, 0, verticesCt);
}

