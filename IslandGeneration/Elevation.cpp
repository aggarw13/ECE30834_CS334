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
//			circleColor[i + j * windowWidth * 3] = 0;

			circleVertices[(i + 1) + j * windowWidth * 3] = normalize(j, windowHeight);
			circleColor[(i + 1) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];
//			circleColor[i + j * windowWidth * 3] = 0;

			circleVertices[(i + 2) + j * windowWidth * 3] = 0.0f;
			circleColor[(i + 2) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];


			i += 3;
		}
		i = 0;
		j++;
	}


//	i = 0; j = 0;
//	while(j < windowHeight){
//		while(i < windowWidth){
//			printf(" %.2f ", elevation[i][j]);
//			i++;
//		}
//		i = 0;
//		j++;
//		printf("\n");
//	}
//	i = 0; j = 0;
//	while(j < windowHeight){
//		printf("[ ");
//		while(i < windowWidth * 3){
//			if(circleVertices[i + j * windowWidth * 3] < -1 || circleVertices[i + j * windowWidth * 3] < -1){
//				printf("%.4f ", circleVertices[i + j * windowWidth * 3]);
//			}
//			i++;
//		}
//		i = 0;
//		j++;
//		printf("]\n");
//	}
//
//	terrain center;
//	int rtheta = 1;
//	for(rtheta = 1; rtheta < r; rtheta += 1){
//		for(i = 0; i < circlesCt; i++){
//			center = allIntensity[i];
//			for(theta = 0; theta < (2 * PI); theta += (PI /DELTA)){
//				xpos0 = rtheta * cos(theta) + center.x;
//				ypos0 = rtheta * sin(theta) + center.y;
//				xpos1 = rtheta * cos(theta + (PI /DELTA)) + center.x;
//				ypos1 = rtheta * sin(theta + (PI /DELTA)) + center.y;
//
//				// center
//				circleVertices[vi++] = normalize(center.x, windowWidth);
//				circleVertices[vi++] = normalize(center.y, windowHeight);
//				circleVertices[vi++] = 0;
//				circleColor[coli++] = center.intensity;
//				circleColor[coli++] = center.intensity;
//				circleColor[coli++] = center.intensity;
//
//				// first vertex of triangle
//				circleVertices[vi++] = normalize(xpos0, windowWidth);
//				circleVertices[vi++] = normalize(ypos0, windowHeight);
//				circleVertices[vi++] = 0;
//				circleColor[coli++] = 1.0f;
//				circleColor[coli++] = 1.0f;
//				circleColor[coli++] = 1.0f;
//
//				// second vertex of triangle
//				circleVertices[vi++] = normalize(xpos1, windowWidth);
//				circleVertices[vi++] = normalize(ypos1, windowHeight);
//				circleVertices[vi++] = 0;
//				circleColor[coli++] = 1.0f;
//				circleColor[coli++] = 1.0f;
//				circleColor[coli++] = 1.0f;
//
//			}
//		}
//	}
	return windowWidth * windowHeight; //vi / 3;

}

void fillWater(float thres, float * waterVertices, float * waterColors, int leftbound, int rightbound, int upperbound, int lowerbound){
	int x,y;
//	(*waterVertices) = (float * ) realloc(*waterVertices, (waterPtCt + (rightbound - leftbound) * (lowerbound - upperbound)) * sizeof(float) * 3);
//	(*waterColors) = (float * ) realloc(*waterVertices, (waterPtCt + (rightbound - leftbound) * (lowerbound - upperbound) * sizeof(float) * 3);
	for(y = upperbound; y < lowerbound; y++){
		for(x = leftbound * 3; x < rightbound * 3; x += 3){
			if(x > 0 && x < windowWidth * 3 && y > 0 && y < windowHeight){
				// fill in vertices
				(waterVertices)[x + windowWidth * y * 3] = normalize(x, windowWidth);
				(waterVertices)[(x + 1)  + windowWidth * y * 3] = normalize(y, windowHeight);
				(waterVertices)[(x + 2)  + windowWidth * y * 3] = 0.0f;

				// fill in colors, if elevation at that point is lower than a certain threshold
				if(elevation[x][y] < thres){
					//(waterColors)[(x + 0)  + windowWidth * y * 3] = 0.0f;
					(waterColors)[(x + 2)  + windowWidth * y * 3] = 1.0f;
				}
			}
		}
	}

	x = 0;
	//return (rightbound - leftbound) * (lowerbound - upperbound);

}

int findPotentialWaterSpots(float thres, float * waterVertices, float * waterColors, terrain * terrainInfo){

	int x,y;
	int xpos, ypos;
	float theta;
	int boundR = 90;
	float aveBoundHeight = 0;
	int boundaryPtCt = (2*PI) / (PI/DELTA);
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
				fillWater(thres, waterVertices, waterColors, x - boundR, x + boundR, y - boundR, y + boundR);
			}
		}
	}
	return windowWidth * windowHeight;
}



