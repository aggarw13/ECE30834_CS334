/*
 * Biome.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: aimanuslim
 */

#include "IslandGeneration.h"

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
		{255,221,199}, // subtropdesert

};

 Biome biomeTable[4][6] = {
		{SUBTROPDESERT, GRASSLAND, TROPSEASONF, TROPSEASONF, TROPRAINF, TROPRAINF},
		{TEMPDESERT, GRASSLAND, GRASSLAND, TEMPDECIDF, TEMPDECIDF, TEMPRAINF},
		{TEMPDESERT, TEMPDESERT, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW},
//		{TEMPDESERT, GRASSLAND, TROPSEASONF, TROPSEASONF, TROPRAINF, TROPRAINF},
//		{TEMPDESERT, GRASSLAND, GRASSLAND, TEMPDECIDF, TEMPDECIDF, TEMPRAINF},
//		{TEMPDESERT, TEMPDESERT, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
//		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW},
};

double elevationZoneDiv = (double) 1 / 4;
double moistureZoneDiv = (double) 1 / 6;
int category[2] = {0, 0};

void findCategory(double elevation, double moisture, int * category){

	if(elevation < 1 * elevationZoneDiv){
		(category)[0] = 0;
	}
	else if(elevation < 2 * elevationZoneDiv)
	{
		(category)[0] = 1;
	}
	else if(elevation < 3 * elevationZoneDiv){
		(category)[0] = 2;
	} else {
		(category)[0] = 3;
	}

	if(moisture < 1 * moistureZoneDiv){
		(category)[1] = 0;
	}
	else if(moisture < 2 * moistureZoneDiv)
	{
		(category)[1] = 1;
	}
	else if(moisture < 3 * moistureZoneDiv){
		(category)[1] = 2;
	}
	else if(moisture < 4 * moistureZoneDiv){
		(category)[1] = 3;
	}
	else if(moisture < 5 * moistureZoneDiv){
		(category)[1] = 4;
	}
	else {
		(category)[1] = 5;
	}
	//printf("Elevation: %f, idx=%d Moisture: %f, idx=%d\n", elevation, category[0], moisture, category[1]);
}

void biomesGeneration(double * colors, double elevation[windowWidth][windowHeight], terrain * waterLocations, int waterCt, double biomesInfo[windowWidth][windowHeight]){
	int x, y, xidx, waterIdx;
	double r, g, b;
	int isWater, isHighest;
	int rad = 90;

	double moisture, distFromCenter;
	for(y = 0; y < windowHeight; y++){
		for(x = 0; x < windowWidth * 3; x++){
			xidx = x / 3;

			moisture = 0;
			for(waterIdx = 0; waterIdx < waterCt; waterIdx++){
				// find moisture
				distFromCenter = sqrt( (double) pow(waterLocations[waterIdx].x - xidx, 2) + (double) pow(waterLocations[waterIdx].y - y, 2));
				distFromCenter = ((distFromCenter / rad) > 1) ? 1.0f : (distFromCenter / rad);
				moisture += waterLocations[waterIdx].waterValue * (1.0f - distFromCenter) - (elevation[xidx][y]);
				if(moisture > 1.0){moisture = 1.0f;}
				if(moisture < 0.0){moisture = 0.0f;}
			}

			moisture *= 0.6;

			// find category
			findCategory(elevation[xidx][y], moisture, category);
			//printf(" %f ", elevation[x][y]);
			// use category to index into biomeTable
			Biome thisBiome = biomeTable[category[0]][category[1]];


			// define colors using biomeColors
			r = (double) biomeColors[thisBiome][0] / 255;
			g = (double) biomeColors[thisBiome][1] / 255;
			b = (double) biomeColors[thisBiome][2] / 255;
//			r = (double) biomeColors[TROPRAINF][0] / 255;
//			g = (double) biomeColors[TROPRAINF][1] / 255;
//			b = (double) biomeColors[TROPRAINF][2] / 255;

//			switch(thisBiome){
//				case TROPRAINF: g *= 2; break;
//				case TEMPRAINF: g *= 2; break;
//				case TROPSEASONF: g *= 2; break;
//				case TEMPDECIDF: g *= 2; break;
//				case GRASSLAND: g *= 2; break;
//				case SHRUBLAND: g *= 2; break;
//				case TAIGA: g *= 2; break;
//			}

			if(moisture < 5 * moistureZoneDiv){ g *= 1.1;}
//			else{ b *= 1.5; }


			// change colors
			double m = (double) 1 / 3;
//			double m = (double) 1 / 2;
			if(x % 3 == 0){
//				colors[(x + 0) + y * windowWidth * 3] += r;
//				colors[(x + 0) + y * windowWidth * 3] = colors[(x + 0) + y * windowWidth * 3] / 2 + r;
				colors[(x + 0) + y * windowWidth * 3] = r * m  + elevation[xidx][y] * m;
//				colors[(x + 0) + y * windowWidth * 3] = r * m;
				colors[(x + 0) + y * windowWidth * 3] *= 2;
//				colors[(x + 1) + y * windowWidth * 3] += g;
//				colors[(x + 1) + y * windowWidth * 3] = colors[(x + 1) + y * windowWidth * 3] / 2 + g;
				colors[(x + 1) + y * windowWidth * 3] = g * m + elevation[xidx][y] * m;
//				colors[(x + 1) + y * windowWidth * 3] = g * m;
				colors[(x + 1) + y * windowWidth * 3] *= 2;
//				colors[(x + 2) + y * windowWidth * 3] += b;
//				colors[(x + 2) + y * windowWidth * 3] = colors[(x + 2) + y * windowWidth * 3] / 2 + b;
				colors[(x + 2) + y * windowWidth * 3] = b * m + elevation[xidx][y] * m + moisture * m;
//				colors[(x + 2) + y * windowWidth * 3] = b * m + moisture * 0.8;
				colors[(x + 2) + y * windowWidth * 3] *= 2;

				if(colors[(x + 0) + y * windowWidth * 3] > 1.0){colors[(x + 0) + y * windowWidth * 3] = 1.0;}
				if(colors[(x + 1) + y * windowWidth * 3] > 1.0){colors[(x + 1) + y * windowWidth * 3] = 1.0;}
				if(colors[(x + 2) + y * windowWidth * 3] > 1.0){colors[(x + 2) + y * windowWidth * 3] = 1.0;}

//				if(thisBiome == SUBTROPDESERT){
//							colors[(x + 0) + y * windowWidth * 3] = r * 0.9;
//							colors[(x + 1) + y * windowWidth * 3] = g * 0.9;
//							colors[(x + 2) + y * windowWidth * 3] = b * 0.9;
//						}
			}

		}
	}
	x = 0;
}



