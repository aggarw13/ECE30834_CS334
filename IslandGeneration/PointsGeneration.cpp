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
 	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 3);

 	srand((unsigned)time(0)); 
    int i = 0;
    while(i < numberofPoints * 3){
    	(*setPoints)[i++] = normalize((rand() % (int) windowHeight) + 1, windowHeight);
    	(*setPoints)[i++] = normalize((rand() % (int) windowWidth) + 1, windowWidth);
    	(*setPoints)[i++] = 0.0f;
   	} 
}

void generateSpiralPoints(float ** setPoints, int numberofPoints){
	// x = R * cos(theta), y = R * sin(theta)
	float x, y, R;
	float theta; // theta is in radians
	int i = 0;
 	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 3);
	x = 0; y = 0; R = 10;
	for(theta = 0; i + 1 < numberofPoints * 3; theta += (PI / DELTA)){
		x = R * cos(theta) + windowWidth / 2;
		y = R * sin(theta) + windowHeight / 2;
		R += 10;
		(*setPoints)[i++] = normalize(x, windowWidth);
		(*setPoints)[i++] = normalize(y, windowHeight);
		(*setPoints)[i++] = 0;
	}
}

void generateGridPoints(float ** setPoints, int numberofPoints){
	int squareArea = windowHeight * windowWidth / numberofPoints;
	int squareSide = (int) sqrt((double) squareArea);
	int columns = windowWidth / squareSide;
	int rows = windowHeight / squareSide;
	*setPoints = (float *) malloc(sizeof(float) * numberofPoints * 3);

	int r, c, i = 0;
	for(r = squareSide / 2; r < windowHeight && i < numberofPoints * 3; r += squareSide){
		for(c = squareSide / 2; c < windowWidth && i < numberofPoints * 3; c += squareSide){
			(*setPoints)[i++] = normalize(c, windowWidth);
			(*setPoints)[i++] = normalize(r, windowHeight);
			(*setPoints)[i++] = 0.0f;
		}
	}

}

