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
		{223,221,199}, // subtropdesert

};

Biome biomeTable[4][6] = {
		{SUBTROPDESERT, GRASSLAND, TROPSEASONF, TROPSEASONF, TROPRAINF, TROPRAINF},
		{TEMPDESERT, GRASSLAND, GRASSLAND, TEMPDECIDF, TEMPDECIDF, TEMPRAINF},
		{TEMPDESERT, TEMPDESERT, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW},
};

double elevationZoneDiv = 1 / 4;
double moistureZoneDiv = 1 / 6;
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
}

void biomesGeneration(double * colors, double elevation[windowWidth][windowHeight], double moisture[windowWidth][windowHeight], double biomesInfo[windowWidth][windowHeight]){
	int x, y;
	double r, g, b;
	for(x = 0; x < windowWidth; x++){
		for(y = 0; y < windowHeight; y++){
			// find category
			findCategory(elevation[x][y], moisture[x][y], category);

			// use category to index into biomeTable
			Biome thisBiome = biomeTable[category[0]][category[1]];

			// define colors using biomeColors
			r = biomeColors[thisBiome][0] / 255;
			g = biomeColors[thisBiome][1] / 255;
			b = biomeColors[thisBiome][2] / 255;

			// change colors
			if(x % 3 == 0){
				colors[(x + 0) + y * windowWidth * 3] = r;
				colors[(x + 1) + y * windowWidth * 3] = g;
				colors[(x + 2) + y * windowWidth * 3] = b;
			}
		}
	}

}



