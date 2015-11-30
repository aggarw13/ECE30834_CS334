#include <iostream>
#include "GL/glut.h"
#include <cstdlib> 
#include <ctime> 
#include <math.h>


#include "IslandGeneration.h"


using namespace std;

int inputter(enum Mode * pointMode){
	int m, numberofPoints;
	cout<< "Enter number of points: ";
 	cin >> numberofPoints;
 	cout<< "Enter point generation mode (Random=1, Spiral=2, Grid=3): ";
 	cin >> m;
 	*pointMode = (enum Mode) m;
 	cout << endl;
 	return numberofPoints;
}

void generateRandomPoints(float ** setPoints, int numberofPoints){
 	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 2);

 	srand((unsigned)time(0)); 
     
    for(int i=0; i<numberofPoints * 2; i++){
    	
    	if((i + 1) % 2 == 0){
    		(*setPoints)[i] = (rand() % (int) windowHeight) + 1;
    	} else{ 
    		(*setPoints)[i] = (rand() % (int) windowWidth) + 1;
        }
        //cout << setPoints[i] << " " ;
   	} 
}

void generateSpiralPoints(float ** setPoints, int numberofPoints){
	// x = R * cos(theta), y = R * sin(theta)
	float x, y, R;
	float theta; // theta is in radians
	int i = 0;
 	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 2);
	x = 0; y = 0; R = 10;
	for(theta = 0; i + 1 < numberofPoints * 2; theta += (PI / DELTA)){
		x = R * cos(theta) + windowWidth / 2;
		y = R * sin(theta) + windowHeight / 2;
		R += 10;
		(*setPoints)[i] = x;
		(*setPoints)[i + 1] = y;
		i += 2;
	}
}

void generateGridPoints(float ** setPoints, int numberofPoints){
	int squareArea = windowHeight * windowWidth / numberofPoints;
	int squareSide = (int) sqrt((double) squareArea);
	int columns = windowWidth / squareSide;
	int rows = windowHeight / squareSide;
	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 2);

	int r, c, i;
	for(r = squareSide / 2; r < windowHeight && i < numberofPoints * 2; r += squareSide){
		for(c = squareSide / 2; c < windowWidth && i < numberofPoints * 2; c += squareSide){
			(*setPoints)[i++] = c;
			(*setPoints)[i++] = r;
		}
	}

}

