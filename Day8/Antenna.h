#ifndef DAY8_ANTENNA_H
#define DAY8_ANTENNA_H
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int x;
    int y;
}Position;

typedef struct{
    Position* arr;
    int size;
    int maxSize;
}Positions;

typedef struct{
    char freq;
    Position pos;
}Antenna;

typedef struct{
    Antenna* ant1;
    Antenna* ant2;
}AntennaPair;

Antenna createAntenna(char freq, int y, int x);
AntennaPair createAntennaPair(Antenna* antenna1, Antenna* antenna2);
Position* getAntinodePositions(AntennaPair antennaPair);
void drawAntinodePositionsP2(AntennaPair antennaPair, int rows, int cols, char map[rows][cols]);
Position getNextPosition(Position current, int dX, int dY);

#endif
