/*
 * Elevation.cpp
 *
 *  Created on: Nov 30, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glut.h"
#include "math.h"
#include "stdlib.h"
#include "PerlinNoise.h"
#include "IslandGeneration.h"


// GLM stuffs
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int verticesCt = 0;
double waterIntensity[IslandWidth][IslandHeight] = {0};

int vIdx = 0, colorIdx;


int terrainInput(double elevation[IslandWidth][IslandHeight], Biome biomesInformation[IslandWidth][IslandHeight], double * circleVertices, glm::vec3 circleColor[IslandWidth][IslandHeight], glm::vec3 perlinOffsets[IslandWidth][IslandHeight], Mode pointMode, bool IslandMode){

	int waterIdx;
	int waterLocCount = 15;
	terrain  waterLocations[waterLocCount];
	for(waterIdx = 0; waterIdx < waterLocCount; waterIdx++){
		waterLocations[waterIdx].x = (rand() % (int) IslandWidth) + 1;
//		waterLocations[waterIdx].x = 700;
		waterLocations[waterIdx].y = (rand() % (int) IslandHeight) + 1;
//		waterLocations[waterIdx].y = 600;
		waterLocations[waterIdx].waterValue = 1.0f;
	}


	int circleIdx;

	int * circleLocations;
	int circlesCt;
	double xpos0, xpos1, ypos0, ypos1, r, rmax, myPow;
	rmax = 100; // for circles
	r = 160;

	switch(pointMode){
		case Spiral:
			circlesCt = 180;
			if(IslandMode) circlesCt = 300;
			generateSpiralPoints(&circleLocations, circlesCt);
			break;
		case Random:
			circlesCt = 100;
			if(IslandMode) circlesCt = 300;
			generateRandomPoints(&circleLocations, circlesCt);
			break;
		case Grid:
			circlesCt = 80;
//			r = 100;
			if(IslandMode) circlesCt = 300;
			generateGridPoints(&circleLocations, circlesCt);
			break;
		default: break;
	}
	terrain allIntensity[circlesCt];

	int heightRand;
	int maxHeight = 500;
	int i =0;
	for(circleIdx = 0; circleIdx < circlesCt; circleIdx++){
//		allIntensity[circleIdx].x = (rand() % (int) IslandWidth) + 1;
		allIntensity[circleIdx].x = circleLocations[i++];
//		allIntensity[circleIdx].y = (rand() % (int) IslandHeight) + 1;
		allIntensity[circleIdx].y = circleLocations[i++];
		i++;
		heightRand = (rand() % maxHeight);
		// allIntensity[circleIdx].intensity = 0.2f;
		allIntensity[circleIdx].intensity = ((double) heightRand / maxHeight) * 0.4f;
	}


	double theta = 0;
	int vi = 0, coli = 0;

	double rrand = (rand() % (int) r);

	int x,y;
	i = 0;
	glm::vec2 center;
	glm::vec2 loc;
	center.x = IslandWidth /2;
	center.y = IslandHeight /2;
	double dIslandCtr;

	double distFromCenter;
	for(x = 0; x < IslandWidth; x++){
		for(y = 0; y < IslandHeight; y++){
			loc.x = x;
			loc.y = y;
			dIslandCtr = glm::length(loc -center);
			dIslandCtr /= glm::length(center);
			for(i = 0; i < circlesCt; i++){
//				r = (rand() % (int) (rmax)) + rmax /2;

				distFromCenter = sqrt( (double) pow(allIntensity[i].x - x, 2) + (double) pow(allIntensity[i].y - y, 2));
				distFromCenter = ((distFromCenter / r) > 1) ? 1.0f : (distFromCenter / r);
//				distFromCenter = ((distFromCenter / (r + rrand) ) > 1) ? 1.0f : (distFromCenter / r + rrand);

				if(distFromCenter <= 1.000000f){
					// the further you are, the less intensity, lower elevation

					if((1.0 - distFromCenter) < (1.0 /3.0)){
						myPow = 2;
					} else if((1.0 - distFromCenter) < (2.0 * 1.0 /3.0)){
						myPow = 1;
					} else {
						myPow = 0.5;
					}

					if(IslandMode){
						elevation[x][y] +=  allIntensity[i].intensity * pow(1.0 - distFromCenter, myPow) * pow(1.0 - dIslandCtr, 3);
					} else{
						elevation[x][y] +=  allIntensity[i].intensity * pow(1.0 - distFromCenter, myPow);
					}
				}
				else {
					elevation[x][y] +=  0.0f;
				}
			}

		}
	}


	// Perlin, may or may not be needed
	generatePerlinNoise(perlinOffsets, elevation);

	biomesGeneration(circleColor, elevation, waterLocations, waterLocCount, biomesInformation, IslandMode);

	return IslandWidth * IslandHeight;

}

