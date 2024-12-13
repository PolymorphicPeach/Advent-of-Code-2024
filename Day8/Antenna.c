#include "Antenna.h"

Antenna createAntenna(char freq, int y, int x){
    Antenna antenna;
    antenna.freq = freq;
    antenna.pos.y = y;
    antenna.pos.x = x;
    return antenna;
}

AntennaPair createAntennaPair(Antenna* antenna1, Antenna* antenna2){
    AntennaPair pair;
    pair.ant1 = antenna1;
    pair.ant2 = antenna2;
    return pair;
}

Position* getAntinodePositions(AntennaPair antennaPair){
    Position* antinodes = malloc(2 * sizeof(Position));

    Position pos1 = antennaPair.ant1->pos;
    Position pos2 = antennaPair.ant2->pos;

    antinodes[0].x = (pos1.x - pos2.x) + pos1.x;
    antinodes[0].y = (pos1.y - pos2.y) + pos1.y;

    antinodes[1].x = (pos2.x - pos1.x) + pos2.x;
    antinodes[1].y = (pos2.y - pos1.y) + pos2.y;

    return antinodes;
}

void drawAntinodePositionsP2(AntennaPair antennaPair, int rows, int cols, char map[rows][cols]) {
    Positions positions;
    int initSize = 50;
    positions.arr = malloc(initSize * sizeof(Position));
    if (!positions.arr) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    positions.size = 0;
    positions.maxSize = initSize;

    Position antPos1 = antennaPair.ant1->pos;
    Position antPos2 = antennaPair.ant2->pos;

    map[antPos1.y][antPos1.x] = '#';
    map[antPos2.y][antPos2.x] = '#';

    int ant1dX = antPos1.x - antPos2.x;
    int ant1dY = antPos1.y - antPos2.y;

    Position nextPosition = getNextPosition(antPos1, ant1dX, ant1dY);

    // Process positions for the first antinode
    while (nextPosition.x >= 0 && nextPosition.x < cols &&
           nextPosition.y >= 0 && nextPosition.y < rows) {
        if (positions.size >= positions.maxSize) {
            Position* temp = realloc(positions.arr, positions.maxSize * 2 * sizeof(Position));
            if (!temp) {
                printf("Error: Couldn't reallocate Positions.arr.\n");
                free(positions.arr);
                return;
            }
            positions.arr = temp;
            positions.maxSize *= 2;
        }
        positions.arr[positions.size] = nextPosition;
        //printf("Position 1: (%d, %d)\n", nextPosition.x, nextPosition.y); // Debugging
        ++positions.size;
        nextPosition = getNextPosition(nextPosition, ant1dX, ant1dY);
    }

    int ant2dX = antPos2.x - antPos1.x;
    int ant2dY = antPos2.y - antPos1.y;

    nextPosition = getNextPosition(antPos2, ant2dX, ant2dY);

    // Process positions for the second antinode
    while (nextPosition.x >= 0 && nextPosition.x < cols &&
           nextPosition.y >= 0 && nextPosition.y < rows) {
        if (positions.size >= positions.maxSize) {
            Position* temp = realloc(positions.arr, positions.maxSize * 2 * sizeof(Position));
            if (!temp) {
                printf("Error: Couldn't reallocate Positions.arr.\n");
                free(positions.arr);
                return;
            }
            positions.arr = temp;
            positions.maxSize *= 2;
        }
        positions.arr[positions.size] = nextPosition;
        //printf("Position 2: (%d, %d)\n", nextPosition.x, nextPosition.y); // Debugging
        ++positions.size;
        nextPosition = getNextPosition(nextPosition, ant2dX, ant2dY);
    }

    // Update the map
    for (int i = 0; i < positions.size; ++i) {
        if (positions.arr[i].x >= 0 && positions.arr[i].x < cols &&
            positions.arr[i].y >= 0 && positions.arr[i].y < rows) {
            map[positions.arr[i].y][positions.arr[i].x] = '#';
        }
        else {
            //printf("Invalid position in map: (%d, %d)\n", positions.arr[i].x, positions.arr[i].y); // Debugging
        }
    }

    free(positions.arr);
}

Position getNextPosition(Position current, int dX, int dY){
    Position position;
    position.x = current.x + dX;
    position.y = current.y + dY;
    return position;
}