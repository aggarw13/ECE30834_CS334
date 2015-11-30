// IslandGeneration.h
#ifndef ISLAND_GENERATION
#define ISLAND_GENERATION

#define PI 3.14159 
#define DELTA 6 // angle step difference for spiral

/* Window information */
#define windowWidth 800
#define windowHeight 600


// Points generation modes
enum Mode {Random=1, Spiral=2, Grid=3};

// Main
int inputter(enum Mode *);





// Functions for point generations
void generateRandomPoints(float **, int);
void generateSpiralPoints(float **, int);
void generateGridPoints(float **, int);

// Lloyd relaxation Functions
float * LloydRelaxation(float *);


// Voronoi functions and definitions
void generateVoronoi(float **, int);



#endif
