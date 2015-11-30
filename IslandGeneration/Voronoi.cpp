/*
 * Voronoi.cpp
 *
 *  Created on: Nov 23, 2015
 *      Author: aimanuslim
 */

#include <iostream>
#include "GL/glut.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "IslandGeneration.h"




VD generateVoronoi(float ** setPoints, int numberofPoints){
	VD vd;

	int i = 0;
	for(i = 0; i < numberofPoints * 2; i+=2){
		Site_2 p(Point_2((*setPoints)[i], (*setPoints)[i + 1]));
		vd.insert(p);

	}

	return vd;

}
