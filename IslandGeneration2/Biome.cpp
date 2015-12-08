/*
 * Biome.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: aimanuslim
 */

#include "IslandGeneration.h"
#include "PerlinNoise.h"
#include "math.h"

// GLM stuffs
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


int biomeColors[biomeMax][3] = {
		{248,248,248}, // snow
		{221,221,187}, // tundra
		{187,187,187}, // bare
		{146,146,146}, // scorched
		{204,212,187}, // taiga
		{196,204,187}, // shurbland
		{228,232,202}, // tempdesert
		{164,196,168}, // temprainf
		{180,201,169}, // tempdecidf
		{196,212,170}, // grassland
		{156,187,169}, // troprainf
		{169,204,164}, // tropseasonf
		{233,221,199}, // subtropdesert

};

 Biome biomeTable[4][6] = {
		{SUBTROPDESERT, GRASSLAND, TROPSEASONF, TROPSEASONF, TROPRAINF, TROPRAINF},
		{TEMPDESERT, GRASSLAND, GRASSLAND, TEMPDECIDF, TEMPDECIDF, TEMPRAINF},
		{TEMPDESERT, TEMPDESERT, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW},
};

double elevationZoneDiv = (double) 1 / 4;
double moistureZoneDiv = (double) 1 / 6;
int category[2] = {0, 0};
double maxElevation = 0;
double minElevation = 50;

void findCategory(double elevation, double moisture, int * category){
	double  highThreshold = 0.7;
	elevationZoneDiv = highThreshold / 4.0f;

	if((elevation / highThreshold) < (1.0f * elevationZoneDiv)){
		(category)[0] = 0;
	}

	else if((elevation / highThreshold) < (2.0f * elevationZoneDiv))
	{
		(category)[0] = 1;
	}

	else if((elevation / highThreshold) < (3.0f * elevationZoneDiv)){
		(category)[0] = 2;
	} else {
		(category)[0] = 3;
	}


	if(moisture < (1.0 * moistureZoneDiv)){
		(category)[1] = 0;
	}
	else if(moisture < (2.0 * moistureZoneDiv))
	{
		(category)[1] = 1;
	}
	else if(moisture < (3.0 * moistureZoneDiv)){
		(category)[1] = 2;
	}
	else if(moisture < (4.0 * moistureZoneDiv)){
		(category)[1] = 3;
	}
	else if(moisture < (5.0 * moistureZoneDiv)){
		(category)[1] = 4;
	}
	else {
		(category)[1] = 5;
	}

}


void biomesGeneration(glm::vec3 colors[IslandWidth][IslandHeight], double elevation[IslandWidth][IslandHeight], terrain * waterLocations, int waterCt, Biome biomesInfo[IslandWidth][IslandHeight], bool IslandMode){
	int x, y, xidx, waterIdx;
	double r, g, b;
	int maxrad = 1000, rad;

	maxElevation = 0.0;
	minElevation = 50;
	for(y = 0; y < IslandHeight; y++){
			for(x = 0; x < IslandWidth; x++){
				if(minElevation > elevation[x][y]){ minElevation = elevation[x][y];}
			}
	}

	printf("Min elevation: %f\n", minElevation);

	PerlinNoise pn;
	float n;
	double moisture, distFromCenter;
	for(y = 0; y < IslandHeight; y++){
		for(x = 0; x < IslandWidth; x++){
			xidx = x;

			moisture = 0;
			for(waterIdx = 0; waterIdx < waterCt; waterIdx++){
				n = pn.noise( (double) x / IslandWidth, (double) y / IslandHeight, 0.0);
				rad = maxrad * (n - floor(n));
				// find moisture
				distFromCenter = sqrt( (double) pow(waterLocations[waterIdx].x - xidx, 2) + (double) pow(waterLocations[waterIdx].y - y, 2));
				distFromCenter = ((distFromCenter / rad) > 1) ? 1.0f : (distFromCenter / rad);
//				printf("Distance: %.3f\n", distFromCenter);
				moisture += waterLocations[waterIdx].waterValue * (1.0 - distFromCenter);

				if(moisture > 1.0){moisture = 1.0f;}
				if(moisture < 0.0){moisture = 0.0f;}
			}


			// find category
			findCategory(elevation[xidx][y], moisture, category);
			Biome thisBiome = biomeTable[category[0]][category[1]];
			biomesInfo[xidx][y] = thisBiome;


			// define colors using biomeColors
			r = (double) biomeColors[thisBiome][0] / 255;
			g = (double) biomeColors[thisBiome][1] / 255;
			b = (double) biomeColors[thisBiome][2] / 255;
//
//			switch(thisBiome){
//				case TROPRAINF: printf("Elevation: %.3f, Moisture: %.3f -> TROPRAINF idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TEMPRAINF: printf("Elevation: %.3f, Moisture: %.3f -> TEMPRAINF idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TROPSEASONF: printf("Elevation: %.3f, Moisture: %.3f -> TROPSEASONF idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TEMPDECIDF: printf("Elevation: %.3f, Moisture: %.3f -> TEMPDECIDF idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case GRASSLAND: printf("Elevation: %.3f, Moisture: %.3f -> GRASSLAND idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case SHRUBLAND: printf("Elevation: %.3f, Moisture: %.3f -> SHRUBLAND idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TAIGA: printf("Elevation: %.3f, Moisture: %.3f -> TAIGA idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TUNDRA: printf("Elevation: %.3f, Moisture: %.3f -> TUNDRA idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case BARE: printf("Elevation: %.3f, Moisture: %.3f -> BARE idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case SCORCHED: printf("Elevation: %.3f, Moisture: %.3f -> SCORCHED idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case SUBTROPDESERT: printf("Elevation: %.3f, Moisture: %.3f -> SUBTROPDESERT idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case SNOW:printf("Elevation: %.3f, Moisture: %.3f -> SNOW idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//				case TEMPDESERT: printf("Elevation: %.3f, Moisture: %.3f -> TEMPDESERT idx: e: %d m: %d\n", elevation[xidx][y], moisture, category[0], category[1]); break;
//			}


			// change colors
			glm::vec2 center;
			glm::vec2 loc;
			loc.x = x;
			loc.y = y;
			center.x = IslandWidth /2;
			center.y = IslandHeight /2;
			double distanceFromCtr;
			distanceFromCtr = (double) glm::length(loc - center);
			n = pn.noise( (double) x / IslandWidth, (double) y / IslandHeight, 0.0);
			rad = maxrad * (n - floor(n));
			//n = sin(x + y);
			n *= 100;


//			if(x % 3 == 0){
//				colors[x][y].r = ((moisture > 0.8) && (elevation[xidx][y] == 0.00000000000000000000000f) ) ? 0.0f : r * 0.8;
//				colors[x][y].g = ((moisture > 0.8) && (elevation[xidx][y] == 0.00000000000000000000000f) ) ? 0.0f : g * 0.8;
//				colors[x][y].b = ((moisture > 0.8) && (elevation[xidx][y] == 0.00000000000000000000000f) ) ? moisture - 0.5 : b * 0.8;
				colors[x][y].r = ((moisture > 0.8) && (elevation[xidx][y] < (minElevation + 0.005) ) ) ? 0.0f : r * 0.8;
				colors[x][y].g = ((moisture > 0.8) && (elevation[xidx][y] < (minElevation + 0.005) ) ) ? 0.0f : g * 0.8;
				colors[x][y].b = ((moisture > 0.8) && (elevation[xidx][y] < (minElevation + 0.005) ) ) ? moisture - 0.5 : b * 0.8;
				if(IslandMode){
					if(distanceFromCtr > (IslandRadius + n)){
						colors[x][y].r = 0;
						colors[x][y].g = 0;
						colors[x][y].b = 0.4;
					}
				}

		}
	}
	x = 0;
}




