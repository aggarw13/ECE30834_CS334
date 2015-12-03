TARGET = IslandGen
LIBS = -lGLU -lGL -lglut -lGLEW -lCGAL -lgmp -lmpfr
FLAGS = -g -O0
CC = g++

.PHONY: default all clean

default: $(TARGET)

all: default
run: $(TARGET)
	./$(TARGET)

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)	

#Main.o: Main.cpp $(HEADERS)
#	g++ -c $(OPTIONS) Main.cpp -o Main.o $(FLAGS)

#PointsGeneration.o: PointsGeneration.cpp $(HEADERS)
#	g++ -c $(OPTIONS) PointsGeneration.cpp -o PointsGeneration.o $(FLAGS)

#Voronoi.o: Voronoi.cpp $(HEADERS)
#	g++ -c $(OPTIONS) Voronoi.cpp -o Voronoi.o $(FLAGS)

#Elevation.o: Elevation.cpp $(HEADERS)
#	g++ -c $(OPTIONS) Elevation.cpp -o Elevation.o $(FLAGS)





