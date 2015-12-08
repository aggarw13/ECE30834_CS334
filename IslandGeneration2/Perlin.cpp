/*
 * Perlin.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: aimanuslim
 *      Code implemented and taken from: https://github.com/sol-prog/Perlin_Noise
 */
#include "PerlinNoise.h"
#include "IslandGeneration.h"

void generatePerlinNoise(glm::vec3 perlinMatrix[IslandWidth][IslandHeight], double elevation[IslandWidth][IslandHeight]){

	PerlinNoise pn;

	int i = 0, j = 0, xtemp;
	double n;
	int n_int;
	while(j < IslandHeight){
		while(i < IslandWidth){
			n = 2.0 * pn.noise((double) i, (double) j, elevation[i][j]);
			perlinMatrix[i][j].x = 0;
			perlinMatrix[i][j].y = 0;
			perlinMatrix[i][j].z = n;
			i++;
		}
		i = 0;
		j++;
	}

}



