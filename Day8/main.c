#include <stdio.h>
#include "Utility.h"
#include "Antenna.h"

int main(void) {
    const char* filename = "../input.txt";
    int rows = 0;
    char** fileContents = getFileContents(filename, &rows);
    int cols = strlen(fileContents[0]);
    char map[rows][cols];
    Antenna* antennas = malloc(500 * sizeof(Antenna));
    int numAntennas = 0;

    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            char character = fileContents[i][j];
            map[i][j] = character;

            if(
                    (character >= '0' && character <= '9') ||
                    (character >= 'A' && character <= 'Z') ||
                    (character >= 'a' && character <= 'z'))
            {
                antennas[numAntennas] = createAntenna(character, i, j);
                ++numAntennas;
            }
        }
    }
    AntennaPair* antennaPairs = malloc(1000 * sizeof(AntennaPair));
    int numPairs = 0;
    for(int i = 0; i < numAntennas - 1; ++i){
        for(int j = i + 1; j < numAntennas; ++j){
            if(antennas[i].freq == antennas[j].freq){
                antennaPairs[numPairs] = createAntennaPair(&antennas[i], &antennas[j]);
                ++numPairs;
            }
        }
    }
    Position* antinodes = malloc(numPairs * 2 * sizeof(Position));
    int numAntinodes = 0;
    for(int i = 0; i < numPairs; ++i){
        Position* antinodePair = getAntinodePositions(antennaPairs[i]);
        antinodes[numAntinodes] = antinodePair[0];
        ++numAntinodes;
        antinodes[numAntinodes] = antinodePair[1];
        ++numAntinodes;
    }
    int part1 = 0;
    for(int i = 0; i < numAntinodes; ++i){
        //printf("%d: Antinode (%d, %d)", i, antinodes[i].x, antinodes[i].y);
        if(
                antinodes[i].x >= 0 && antinodes[i].x < cols &&
                antinodes[i].y >= 0 && antinodes[i].y < rows)
        {
            map[antinodes[i].y][antinodes[i].x] = '#';
        }
    }
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            if(map[i][j] == '#'){
                ++part1;
            }
        }
    }
    printf("Part1: %d\n", part1);
    // Resetting map
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            map[i][j] = '.';
        }
    }
    int part2 = 0;
    for(int i = 0; i < numPairs; ++i){
        drawAntinodePositionsP2(antennaPairs[i], rows, cols, map);
    }
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            if(map[i][j] == '#'){
                ++part2;
            }
        }
    }
    printf("Part2: %d\n", part2);
    return 0;
}