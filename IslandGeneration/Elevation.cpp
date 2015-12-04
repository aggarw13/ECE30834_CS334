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

int verticesCt = 0;
double elevation[windowWidth][windowHeight] = {0};
//<vector<vector<double>> elevation(windowWidth, windowHeight);
double waterIntensity[windowWidth][windowHeight] = {0};
double biomesInformation[windowWidth][windowHeight] = {0};
int vIdx = 0, colorIdx;

//void generatePerlinNoise(double elevation[][windowHeight], double waterValues[][windowHeight]){
//
//	PerlinNoise pn;
//	int x,y;
//	double n, i, j;
//	for(x = 0; x < windowWidth; x++){
//		for(y = 0; y < windowHeight; y++){
//			i = (double) x / windowWidth;
//			j = (double) y / windowHeight;
//			n = 20 * pn.noise((double) i, (double) j, 0.0);
//			n = n - floor(n);
//			if(waterValues[x][y] == 0){
//				elevation[x][y] = n;
//			}
//		}
//	}
//}

int terrainInput(terrain * terrainArr, double * circleVertices, double * circleColor){

	int waterIdx;
	int waterLocCount = 10;
	terrain  waterLocations[waterLocCount];
	for(waterIdx = 0; waterIdx < waterLocCount; waterIdx++){
		waterLocations[waterIdx].x = (rand() % (int) windowWidth) + 1;
		waterLocations[waterIdx].y = (rand() % (int) windowHeight) + 1;
		//printf("Water location %d -> x: %d y: %d\n", waterIdx, waterLocations[waterIdx].x, waterLocations[waterIdx].y);
		waterLocations[waterIdx].waterValue = 1.0f;
	}

	int circlesCt = 40;
	int circleIdx;
	//terrain allIntensity[circlesCt] = {intensity1, intensity2, intensity3, intensity4, intensity5, intensity6};
	terrain allIntensity[circlesCt];
	for(circleIdx = 0; circleIdx < circlesCt; circleIdx++){
		allIntensity[circleIdx].x = (rand() % (int) windowWidth) + 1;
		allIntensity[circleIdx].y = (rand() % (int) windowHeight) + 1;;
		allIntensity[circleIdx].intensity = 0.5f;
	}

	double xpos0, xpos1, ypos0, ypos1, r;
	double theta = 0;
	int vi = 0, coli = 0, i;
	r = 100; // for circles

	int x,y;
	i = 0;
	double distFromCenter;
	for(x = 0; x < windowWidth; x++){
		for(y = 0; y < windowHeight; y++){
			for(i = 0; i < circlesCt; i++){
				distFromCenter = sqrt( (double) pow(allIntensity[i].x - x, 2) + (double) pow(allIntensity[i].y - y, 2));
				distFromCenter = ((distFromCenter / r) > 1) ? 1.0f : (distFromCenter / r);
				if(distFromCenter < 1.000000f){
					// the further you are, the less intensity, lower elevation
					elevation[x][y] +=  allIntensity[i].intensity * (1.0f - distFromCenter);
					if(elevation[x][y] > 1.0){ elevation[x][y] = 1.0; }
				}
				else {
					elevation[x][y] +=  0.0f;
				}
			}
			elevation[x][y] *= 0.6;
		}
	}

	r = 80; // for water
	for(x = 0; x < windowWidth; x++){
			for(y = 0; y < windowHeight; y++){
				for(i = 0; i < waterLocCount; i++){
					distFromCenter = sqrt( (double) pow(waterLocations[i].x - x, 2) + (double) pow(waterLocations[i].y - y, 2));
					distFromCenter = ((distFromCenter / r) > 1) ? 1.0f : (distFromCenter / r);
					if(distFromCenter < 1.000000f){
						// the further you are, the less intensity, less water
						waterIntensity[x][y] +=  waterLocations[i].waterValue * (1.0f - distFromCenter) - (elevation[x][y]);
						if(waterIntensity[x][y] > 1.0){waterIntensity[x][y] = 1.0f;}
						if(waterIntensity[x][y] < 0.0){waterIntensity[x][y] = 0.0f;}
					}
					else {
						waterIntensity[x][y] +=  0.0f;
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
//			circleColor[i + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];
			circleColor[i + j * windowWidth * 3] = (waterIntensity[xtemp][j] > 0) ? 0.0f : 1.0f - elevation[xtemp][j];


			circleVertices[(i + 1) + j * windowWidth * 3] = normalize(j, windowHeight);
//			circleColor[(i + 1) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];
			circleColor[(i + 1) + j * windowWidth * 3] = (waterIntensity[xtemp][j] > 0) ? 0.0f : 1.0f - elevation[xtemp][j];

			circleVertices[(i + 2) + j * windowWidth * 3] = 0.0f;

			circleColor[(i + 2) + j * windowWidth * 3] = (waterIntensity[xtemp][j] > 0) ? waterIntensity[xtemp][j]: 1.0f - elevation[xtemp][j]; // BLUE value
//			circleColor[(i + 2) + j * windowWidth * 3] = 1.0f - elevation[xtemp][j];


			i += 3;
		}
		i = 0;
		j++;
	}

	// Perlin, may or may not be needed
	// generatePerlinNoise(circleVertices);

	biomesGeneration(circleColor, elevation, waterLocations, waterLocCount, biomesInformation);

	return windowWidth * windowHeight;

}

int fillWater(double thres, double ** waterVertices, double ** waterColors, int leftbound, int rightbound, int upperbound, int lowerbound){
	int x,y;
	int offsetRange = 300;
//	(*waterVertices) = (double * ) realloc(*waterVertices, (vIdx + (rightbound - leftbound) * (lowerbound - upperbound) * 3) * sizeof(double));
//	(*waterColors) = (double * ) realloc(*waterVertices, (colorIdx + (rightbound - leftbound) * (lowerbound - upperbound) * 3) * sizeof(double));
	for(y = upperbound; y < lowerbound; y++){
		for(x = leftbound; x < rightbound; x++){
			if(x > 0 && x < windowWidth && y > 0 && y < windowHeight){
				if(elevation[x][y] < thres){
					// provide memory
					(*waterVertices) = (double *) realloc( (*waterVertices), (vIdx + 3) * sizeof(double) );

					// Noise


					// fill in vertices
					(*waterVertices)[vIdx++] = normalize(x, windowWidth);
					(*waterVertices)[vIdx++] = normalize(y, windowHeight);
					(*waterVertices)[vIdx++] = 0.0f;



					// provide memory
					(*waterColors) = (double *) realloc( (*waterColors), (colorIdx + 3) * sizeof(double) );

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

//int findPotentialWaterSpots(double thres, double ** waterVertices, double ** waterColors){
int findPotentialWaterSpots(double thres, double * circleVertices, double * circleColors){
	// Use low contours property
	int x,y;
	int xpos, ypos;
	double theta;
	int boundR = 90;
	double aveBoundHeight = 0;
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
			aveBoundHeight /= (double) boundaryPtCt;

			// fill in water color
			if(aveBoundHeight - elevation[x][y] > elevDiffThreshold){
				//waterPtCt = fillWater(thres, waterVertices, waterColors, x - boundR, x + boundR, y - boundR, y + boundR);
			}
		}
	}

//	// Use random selection of water starting points
//	int numberofWater = 5;
//	int xcoords[numberofWater];
//	int ycoords[numberofWater];
//	int i =0;
//	int offIx, offIy;
//	int off[] = {-1, 1, 0};
//	int xfol, yfol; // water head trail
//	double gradient;
//	double maxGradient;
//	int dirwithlargestGradient[] = {0, 0};
//	int length = 100;
//
//	for(i = 0; i < numberofWater; i++){
//		xcoords[i] = (rand() % (int) windowWidth) + 1;
//		ycoords[i] = (rand() % (int) windowHeight) + 1;
//
//		xfol = xcoords[i];
//		yfol = ycoords[i];
//
//		while(elevation[xfol][yfol] > thres || length != 0){
//
//			for(offIx = 0; offIx < 3; offIx++){
//				for(offIy = 0; offIy < 3; offIy++){
//					// for each direction, find out which one has the highest gradient
//					gradient = elevation[xfol][yfol] - elevation[xfol + off[offIx]][yfol + off[offIy]];
//					if(gradient > maxGradient){
//						maxGradient = gradient;
//						dirwithlargestGradient[0] = xfol + off[offIx];
//						dirwithlargestGradient[1] = yfol + off[offIy];
//					}
//				}
//			}
//
//			// update xfol and yfol to the direction with the largest gradient
//			xfol = dirwithlargestGradient[0];
//			yfol = dirwithlargestGradient[1];
//
//			// fill in vertices for that particular point
//
//			// provide memory
//			(*waterVertices) = (double *) realloc( (*waterVertices), (vIdx + 3) * sizeof(double) );
//
//			// fill in vertices
//			(*waterVertices)[vIdx++] = normalize(xfol, windowWidth);
//			(*waterVertices)[vIdx++] = normalize(yfol, windowHeight);
//			(*waterVertices)[vIdx++] = 0.0f;
//
//
//
//			// provide memory
//			(*waterColors) = (double *) realloc( (*waterColors), (colorIdx + 3) * sizeof(double) );
//
//			// fill in colors
//			(*waterColors)[colorIdx++] = 0.0f;
//			(*waterColors)[colorIdx++] = 0.0f;
//			(*waterColors)[colorIdx++] = 0.6f;
//
//			length--;
//
//		}
//
//		// fill in water once low level area reached
//		waterPtCt = fillWater(thres, waterVertices, waterColors, xfol - boundR, xfol + boundR, yfol - boundR, yfol + boundR);
//
//	}










	return waterPtCt / 3;
}





