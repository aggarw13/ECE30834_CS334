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

#include "IslandGeneration.h"

int verticesCt = 0;
float elevation[windowWidth][windowHeight] = {0};
gradient gradientInfo[windowWidth][windowHeight] = {0};
int vIdx = 0, colorIdx;

int terrainInput(terrain * terrainArr, float * circleVertices, float * circleColor){

	// intensity point input (define in-program for now)
	terrain intensity1, intensity2, intensity3, intensity4, intensity5, intensity6;
	srand((unsigned)time(0));
	intensity1.x =  (rand() % (int) windowWidth) + 1;
	intensity1.y = (rand() % (int) windowHeight) + 1;
	intensity1.intensity = 0.8;
	intensity2.x =  (rand() % (int) windowWidth) + 1;
	intensity2.y = (rand() % (int) windowHeight) + 1;
	intensity2.intensity = 0.9;
	intensity3.x =  200;//(rand() % (int) windowWidth) + 1;
	intensity3.y = 300;//(rand() % (int) windowHeight) + 1;
	intensity3.intensity = 0.9;
	intensity4.x =  400;//(rand() % (int) windowWidth) + 1;
	intensity4.y = 100;//(rand() % (int) windowHeight) + 1;
	intensity4.intensity = 0.9;
	intensity5.x =  400;//(rand() % (int) windowWidth) + 1;
	intensity5.y = 500;//(rand() % (int) windowHeight) + 1;
	intensity5.intensity = 0.9;
	intensity6.x =  600;//(rand() % (int) windowWidth) + 1;
	intensity6.y = 300;//(rand() % (int) windowHeight) + 1;
	intensity6.intensity = 0.9;


	int circlesCt = 6;
	terrain allIntensity[] = {intensity1, intensity2, intensity3, intensity4, intensity5, intensity6};

	float xpos0, xpos1, ypos0, ypos1, r;
	float theta = 0;
	int vi = 0, coli = 0, i;
	r = 180;

	int x,y;
	i = 0;
	float distFromCenter;
	for(x = 0; x < windowWidth; x++){
		for(y = 0; y < windowHeight; y++){
			for(i = 0; i < circlesCt; i++){
				distFromCenter = sqrt( (double) pow(allIntensity[i].x - x, 2) + (double) pow(allIntensity[i].y - y, 2));
				distFromCenter = ((distFromCenter / r) > 1) ? 1.0f : (distFromCenter / r);
				if(distFromCenter < 1.000000f){
					// the further you are, the less intensity, lower elevation
					elevation[x][y] +=  allIntensity[i].intensity * (1.0f - distFromCenter);
				}
				else {
					elevation[x][y] +=  0.0f;
				}
			}
		}
	}


	i = 0;
	int j = 0;
	int xtemp, ytemp;
	while(j < windowHeight){
		while(i < windowWidth * 3){
			xtemp = i / 3;
			circleVertices[i + j * windowWidth * 3] = normalize(xtemp, windowWidth);
			circleColor[i + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];


			circleVertices[(i + 1) + j * windowWidth * 3] = normalize(j, windowHeight);
			circleColor[(i + 1) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];


			circleVertices[(i + 2) + j * windowWidth * 3] = 0.0f;
			circleColor[(i + 2) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];


			i += 3;
		}
		i = 0;
		j++;
	}

	return windowWidth * windowHeight;

}

int fillWater(float thres, float ** waterVertices, float ** waterColors, int leftbound, int rightbound, int upperbound, int lowerbound){
	int x,y;
	int offsetRange = 300;
//	(*waterVertices) = (float * ) realloc(*waterVertices, (vIdx + (rightbound - leftbound) * (lowerbound - upperbound) * 3) * sizeof(float));
//	(*waterColors) = (float * ) realloc(*waterVertices, (colorIdx + (rightbound - leftbound) * (lowerbound - upperbound) * 3) * sizeof(float));
	for(y = upperbound; y < lowerbound; y++){
		for(x = leftbound; x < rightbound; x++){
			if(x > 0 && x < windowWidth && y > 0 && y < windowHeight){
				if(elevation[x][y] < thres){
					// provide memory
					(*waterVertices) = (float *) realloc( (*waterVertices), (vIdx + 3) * sizeof(float) );

					// Noise


					// fill in vertices
					(*waterVertices)[vIdx++] = normalize(x, windowWidth);
					(*waterVertices)[vIdx++] = normalize(y, windowHeight);
					(*waterVertices)[vIdx++] = 0.0f;



					// provide memory
					(*waterColors) = (float *) realloc( (*waterColors), (colorIdx + 3) * sizeof(float) );

					// fill in colors, if elevation at that point is lower than a certain threshold
					(*waterColors)[colorIdx++] = 0.0f;
					(*waterColors)[colorIdx++] = 0.0f;
					(*waterColors)[colorIdx++] = 0.7f;
				}
			}
		}
	}

	x = 0;
	return vIdx;

}

int findPotentialWaterSpots(float thres, float ** waterVertices, float ** waterColors){

	int x,y;
	int xpos, ypos;
	float theta;
	int boundR = 90;
	float aveBoundHeight = 0;
	int boundaryPtCt = (2*PI) / (PI/DELTA);
	int waterPtCt = 0;
	for(x = 0; x < windowWidth; x++){
		for(y = 0; y < windowHeight; y++){

			// find average of height of the boundary points
			for(theta = 0; theta < (2 * PI); theta += (PI /DELTA)){
				xpos = (int) round(boundR * cos(theta) + x);
				ypos = (int) round(boundR * sin(theta) + y);
				if(xpos > 0 && xpos < windowWidth && ypos > 0 && ypos < windowHeight){
					aveBoundHeight += elevation[xpos][ypos];
				}
			}
			aveBoundHeight /= (float) boundaryPtCt;

			// fill in water color
			if(aveBoundHeight - elevation[x][y] > elevDiffThreshold){
				waterPtCt = fillWater(thres, waterVertices, waterColors, x - boundR, x + boundR, y - boundR, y + boundR);
			}
		}
	}
	return waterPtCt / 3;
}



